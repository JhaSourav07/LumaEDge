#include "CursorTracker.h"
#include <QCursor>
#include <cmath>

namespace LumaEdge {

CursorTracker::CursorTracker(LightController *controller, PlatformDetector *platform, QObject *parent)
    : QObject(parent)
    , m_controller(controller)
    , m_platform(platform)
{
    // 60 Hz polling timer for smooth tracking
    m_timer.setInterval(16);
    connect(&m_timer, &QTimer::timeout, this, &CursorTracker::onPollTimer);

    if (m_controller) {
        connect(m_controller, &LightController::activeChanged, this, &CursorTracker::updateTrackingState);
        connect(m_controller, &LightController::cursorSettingsChanged, this, &CursorTracker::updateTrackingState);
    }

    updateTrackingState();
}

CursorTracker::~CursorTracker()
{
    stopTracking();
}

bool CursorTracker::isTrackingSupported() const
{
    return m_platform && m_platform->supportsGlobalCursorTracking();
}

void CursorTracker::updateTrackingState()
{
    const bool shouldTrack = m_controller &&
                             m_controller->isActive() &&
                             m_controller->cursorAwarenessEnabled() &&
                             isTrackingSupported();

    if (shouldTrack && !m_timer.isActive()) {
        startTracking();
    } else if (!shouldTrack && m_timer.isActive()) {
        stopTracking();
    }
}

void CursorTracker::startTracking()
{
    m_timer.start();
    emit trackingStateChanged();
}

void CursorTracker::stopTracking()
{
    m_timer.stop();
    m_currentPos = QPointF(-1.0f, -1.0f);
    m_targetPos = QPointF(-1.0f, -1.0f);
    if (m_controller) {
        m_controller->setCursorPos(m_currentPos);
    }
    emit trackingStateChanged();
}

void CursorTracker::onPollTimer()
{
    if (!isTrackingSupported()) {
        stopTracking();
        return;
    }

    QPoint rawPos = QCursor::pos();
    m_targetPos = QPointF(rawPos.x(), rawPos.y());

    // Exponential smoothing for natural, flicker-free light response
    if (m_currentPos.x() < 0.0f) {
        m_currentPos = m_targetPos;
    } else {
        constexpr float smoothingFactor = 0.25f;
        qreal smoothedX = m_currentPos.x() + (m_targetPos.x() - m_currentPos.x()) * smoothingFactor;
        qreal smoothedY = m_currentPos.y() + (m_targetPos.y() - m_currentPos.y()) * smoothingFactor;
        m_currentPos = QPointF(smoothedX, smoothedY);
    }

    if (m_controller) {
        m_controller->setCursorPos(m_currentPos);
    }
}

} // namespace LumaEdge
