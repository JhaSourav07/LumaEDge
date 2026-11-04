#pragma once

#include <QObject>
#include <QPointF>
#include <QTimer>
#include "../core/LightController.h"
#include "../platform/PlatformDetector.h"

namespace LumaEdge {

class CursorTracker : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isTrackingSupported READ isTrackingSupported CONSTANT)
    Q_PROPERTY(bool isTrackingActive READ isTrackingActive NOTIFY trackingStateChanged)

public:
    explicit CursorTracker(LightController *controller, PlatformDetector *platform, QObject *parent = nullptr);
    ~CursorTracker() override;

    bool isTrackingSupported() const;
    bool isTrackingActive() const { return m_timer.isActive(); }

public slots:
    void updateTrackingState();

signals:
    void trackingStateChanged();

private slots:
    void onPollTimer();

private:
    LightController *m_controller{nullptr};
    PlatformDetector *m_platform{nullptr};
    QTimer m_timer;

    QPointF m_currentPos{-1.0f, -1.0f};
    QPointF m_targetPos{-1.0f, -1.0f};

    void startTracking();
    void stopTracking();
};

} // namespace LumaEdge
