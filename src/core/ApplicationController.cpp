#include "ApplicationController.h"
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickWindow>
#include <QDebug>

namespace LumaEdge {

ApplicationController::ApplicationController(QObject *parent)
    : QObject(parent)
{
    m_settings = std::make_unique<SettingsManager>(this);
    m_platformDetector = std::make_unique<PlatformDetector>(this);
    m_lightController = std::make_unique<LightController>(m_settings.get(), this);
    m_overlayManager = std::make_unique<OverlayManager>(m_lightController.get(), this);
    m_cursorTracker = std::make_unique<CursorTracker>(m_lightController.get(), m_platformDetector.get(), this);
    m_displayManager = std::make_unique<DisplayManager>(m_lightController.get(), this);
    m_cameraManager = std::make_unique<CameraManager>(m_lightController.get(), this);
    m_systemTray = std::make_unique<SystemTray>(m_lightController.get(), this);
    m_startupManager = std::make_unique<StartupManager>(m_settings.get(), this);

    connect(m_systemTray.get(), &SystemTray::showDashboardRequested,
            this, &ApplicationController::showDashboard);
    connect(m_systemTray.get(), &SystemTray::quitRequested,
            this, &ApplicationController::quitApplication);
}

ApplicationController::~ApplicationController() = default;

bool ApplicationController::initialize(QQmlApplicationEngine &engine, bool startInBackground)
{
    m_engine = &engine;

    engine.rootContext()->setContextProperty(QStringLiteral("appController"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("lightController"), m_lightController.get());
    engine.rootContext()->setContextProperty(QStringLiteral("platformDetector"), m_platformDetector.get());
    engine.rootContext()->setContextProperty(QStringLiteral("displayManager"), m_displayManager.get());
    engine.rootContext()->setContextProperty(QStringLiteral("cameraManager"), m_cameraManager.get());
    engine.rootContext()->setContextProperty(QStringLiteral("startupManager"), m_startupManager.get());

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/LumaEdge/qml/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML root object";
        return false;
    }

    if (startInBackground) {
        auto *dashboard = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
        if (dashboard) {
            dashboard->hide();
        }
    }

    return true;
}

void ApplicationController::showDashboard()
{
    if (!m_engine || m_engine->rootObjects().isEmpty()) return;

    auto *dashboard = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if (dashboard) {
        dashboard->show();
        dashboard->raise();
        dashboard->requestActivate();
    }
}

void ApplicationController::quitApplication()
{
    qGuiApp->quit();
}

} // namespace LumaEdge
