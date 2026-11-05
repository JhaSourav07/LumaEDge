#include "CameraManager.h"
#include <QDir>
#include <QFileInfo>
#include <QVariantMap>
#include <QDebug>
#include <algorithm>
#include <cmath>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

namespace LumaEdge {

CameraManager::CameraManager(LightController *controller, QObject *parent)
    : QObject(parent)
    , m_controller(controller)
{
    // Sample ambient illumination every 1000ms (1 Hz) to minimize CPU usage to near 0%
    m_sampleTimer.setInterval(1000);
    connect(&m_sampleTimer, &QTimer::timeout, this, &CameraManager::onSampleTimer);

    if (m_controller) {
        connect(m_controller, &LightController::autoLowLightChanged,
                this, &CameraManager::updateCameraState);
        connect(m_controller, &LightController::activeChanged,
                this, &CameraManager::updateCameraState);
        connect(m_controller, &LightController::brightnessChanged, this, [this]() {
            if (m_controller) {
                m_smoothedTargetBrightness = m_controller->brightness();
            }
        });
    }

    refreshCameras();
    updateCameraState();
}

CameraManager::~CameraManager()
{
    stopCapture();
}

void CameraManager::refreshCameras()
{
    m_cameraList.clear();

    QDir devDir(QStringLiteral("/dev"));
    QStringList videoDevices = devDir.entryList({QStringLiteral("video*")}, QDir::System);

    for (const QString &devName : videoDevices) {
        QString devPath = QStringLiteral("/dev/") + devName;
        int fd = open(devPath.toUtf8().constData(), O_RDWR | O_NONBLOCK, 0);
        if (fd < 0) continue;

        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == 0) {
            // Verify device supports video capture
            if (cap.device_caps & V4L2_CAP_VIDEO_CAPTURE) {
                QVariantMap cam;
                cam[QStringLiteral("path")] = devPath;
                cam[QStringLiteral("name")] = QString::fromUtf8(reinterpret_cast<char*>(cap.card));
                cam[QStringLiteral("driver")] = QString::fromUtf8(reinterpret_cast<char*>(cap.driver));
                m_cameraList.append(cam);
            }
        }
        close(fd);
    }

    if (m_selectedDevice.isEmpty() && !m_cameraList.isEmpty()) {
        m_selectedDevice = m_cameraList.first().toMap().value(QStringLiteral("path")).toString();
    }

    emit camerasListChanged();
}

void CameraManager::setSelectedCameraDevice(const QString &device)
{
    if (m_selectedDevice != device) {
        m_selectedDevice = device;
        emit selectedCameraChanged();
        if (m_active) {
            stopCapture();
            startCapture();
        }
    }
}

void CameraManager::updateCameraState()
{
    const bool shouldBeActive = m_controller &&
                                m_controller->isActive() &&
                                m_controller->autoLowLightEnabled() &&
                                !m_selectedDevice.isEmpty();

    if (shouldBeActive && !m_active) {
        startCapture();
    } else if (!shouldBeActive && m_active) {
        stopCapture();
    }
}

void CameraManager::startCapture()
{
    m_active = true;
    m_sampleTimer.start();
    m_statusText = QStringLiteral("Active (Sampling ambient room luma)");
    emit cameraStateChanged();
    emit statusChanged();
    onSampleTimer();
}

void CameraManager::stopCapture()
{
    m_active = false;
    m_sampleTimer.stop();
    m_statusText = QStringLiteral("Idle (Camera closed)");
    emit cameraStateChanged();
    emit statusChanged();
}

void CameraManager::onSampleTimer()
{
    if (!m_active || m_selectedDevice.isEmpty()) return;

    int fd = open(m_selectedDevice.toUtf8().constData(), O_RDWR | O_NONBLOCK, 0);
    if (fd < 0) {
        m_statusText = QStringLiteral("Failed to open camera device");
        emit statusChanged();
        return;
    }

    struct v4l2_format fmt{};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 160;
    fmt.fmt.pix.height = 120;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    ioctl(fd, VIDIOC_S_FMT, &fmt);

    struct v4l2_requestbuffers req{};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0 || req.count < 1) {
        close(fd);
        return;
    }

    struct v4l2_buffer buf{};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
        close(fd);
        return;
    }

    void *buffer_start = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    if (buffer_start == MAP_FAILED) {
        close(fd);
        return;
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_QBUF, &buf);
    ioctl(fd, VIDIOC_STREAMON, &type);

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv{0, 200000}; // 200ms timeout
    int r = select(fd + 1, &fds, nullptr, nullptr, &tv);

    if (r > 0 && ioctl(fd, VIDIOC_DQBUF, &buf) == 0) {
        processFrameData(static_cast<const unsigned char*>(buffer_start), 160, 120);
    }

    ioctl(fd, VIDIOC_STREAMOFF, &type);
    munmap(buffer_start, buf.length);
    close(fd);
}

void CameraManager::processFrameData(const unsigned char *data, int width, int height)
{
    if (!data || width <= 0 || height <= 0) return;

    // In YUYV 4:2:2, every 4 bytes contain 2 pixels: Y0 U Y1 V
    // Y represents pure luminance in [0, 255]
    unsigned long long totalY = 0;
    int pixelCount = width * height;
    int byteCount = pixelCount * 2;

    for (int i = 0; i < byteCount; i += 2) {
        totalY += data[i];
    }

    float avgLuma = static_cast<float>(totalY) / static_cast<float>(pixelCount);
    float normalizedLuma = std::clamp(avgLuma / 255.0f, 0.0f, 1.0f);

    m_currentLuma = normalizedLuma;
    emit lumaChanged();

    // Invert luma: darker ambient room requires higher ring light brightness
    // Maintain a high floor (0.70f) so auto-adjustment never leaves user underlit or in negative feedback loop
    float targetBrightness = std::clamp(1.0f - (normalizedLuma * 0.35f), 0.70f, 1.0f);

    // Apply sensitivity scaling
    float sensitivity = m_controller ? m_controller->autoLowLightSensitivity() : 0.5f;
    float adjustedTarget = (targetBrightness * sensitivity) + (m_smoothedTargetBrightness * (1.0f - sensitivity));

    // Hysteresis threshold to prevent light hunting/oscillations
    constexpr float hysteresis = 0.04f;
    if (std::abs(adjustedTarget - m_smoothedTargetBrightness) > hysteresis) {
        m_smoothedTargetBrightness = adjustedTarget;
        if (m_controller) {
            m_controller->setBrightness(m_smoothedTargetBrightness);
        }
    }
}

} // namespace LumaEdge
