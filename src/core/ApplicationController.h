#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>

#include "SettingsManager.h"
#include "LightController.h"
#include "../platform/PlatformDetector.h"
#include "../overlay/OverlayManager.h"
#include "../cursor/CursorTracker.h"
#include "../display/DisplayManager.h"
#include "../camera/CameraManager.h"
#include "../system/SystemTray.h"
#include "../system/StartupManager.h"

namespace LumaEdge {

class ApplicationController : public QObject {
    Q_OBJECT

    Q_PROPERTY(LightController* lightController READ lightController CONSTANT)
    Q_PROPERTY(PlatformDetector* platformDetector READ platformDetector CONSTANT)
    Q_PROPERTY(DisplayManager* displayManager READ displayManager CONSTANT)
    Q_PROPERTY(CameraManager* cameraManager READ cameraManager CONSTANT)
    Q_PROPERTY(StartupManager* startupManager READ startupManager CONSTANT)

public:
    explicit ApplicationController(QObject *parent = nullptr);
    ~ApplicationController() override;

    bool initialize(QQmlApplicationEngine &engine, bool startInBackground = false);

    LightController *lightController() const { return m_lightController.get(); }
    PlatformDetector *platformDetector() const { return m_platformDetector.get(); }
    DisplayManager *displayManager() const { return m_displayManager.get(); }
    CameraManager *cameraManager() const { return m_cameraManager.get(); }
    StartupManager *startupManager() const { return m_startupManager.get(); }

public slots:
    void showDashboard();
    void quitApplication();

private:
    std::unique_ptr<SettingsManager> m_settings;
    std::unique_ptr<PlatformDetector> m_platformDetector;
    std::unique_ptr<LightController> m_lightController;
    std::unique_ptr<OverlayManager> m_overlayManager;
    std::unique_ptr<CursorTracker> m_cursorTracker;
    std::unique_ptr<DisplayManager> m_displayManager;
    std::unique_ptr<CameraManager> m_cameraManager;
    std::unique_ptr<SystemTray> m_systemTray;
    std::unique_ptr<StartupManager> m_startupManager;
    QQmlApplicationEngine *m_engine{nullptr};
};

} // namespace LumaEdge
