#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantList>
#include <QTimer>
#include <memory>
#include "../core/LightController.h"

namespace LumaEdge {

class CameraManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isCameraActive READ isCameraActive NOTIFY cameraStateChanged)
    Q_PROPERTY(QVariantList availableCameras READ availableCameras NOTIFY camerasListChanged)
    Q_PROPERTY(QString selectedCameraDevice READ selectedCameraDevice WRITE setSelectedCameraDevice NOTIFY selectedCameraChanged)
    Q_PROPERTY(float currentLuma READ currentLuma NOTIFY lumaChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusChanged)

public:
    explicit CameraManager(LightController *controller, QObject *parent = nullptr);
    ~CameraManager() override;

    bool isCameraActive() const { return m_active; }
    QVariantList availableCameras() const { return m_cameraList; }
    QString selectedCameraDevice() const { return m_selectedDevice; }
    void setSelectedCameraDevice(const QString &device);

    float currentLuma() const { return m_currentLuma; }
    QString statusText() const { return m_statusText; }

public slots:
    void refreshCameras();
    void updateCameraState();

signals:
    void cameraStateChanged();
    void camerasListChanged();
    void selectedCameraChanged();
    void lumaChanged();
    void statusChanged();

private slots:
    void onSampleTimer();

private:
    LightController *m_controller{nullptr};
    bool m_active{false};
    QVariantList m_cameraList;
    QString m_selectedDevice;
    float m_currentLuma{0.5f};
    float m_smoothedTargetBrightness{0.8f};
    QString m_statusText{QStringLiteral("Idle (Camera closed)")};
    QTimer m_sampleTimer;

    void startCapture();
    void stopCapture();
    void processFrameData(const unsigned char *data, int width, int height);
};

} // namespace LumaEdge
