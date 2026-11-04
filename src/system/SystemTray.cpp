#include "SystemTray.h"
#include <QIcon>
#include <QPainter>
#include <QPixmap>

namespace LumaEdge {

SystemTray::SystemTray(LightController *controller, QObject *parent)
    : QObject(parent)
    , m_controller(controller)
{
    setupTray();

    if (m_controller) {
        connect(m_controller, &LightController::activeChanged, this, &SystemTray::updateMenuState);
        connect(m_controller, &LightController::brightnessChanged, this, &SystemTray::updateMenuState);
    }
}

SystemTray::~SystemTray() = default;

bool SystemTray::isAvailable() const
{
    return QSystemTrayIcon::isSystemTrayAvailable();
}

void SystemTray::setupTray()
{
    if (!isAvailable()) {
        return;
    }

    // Generate crisp icon for tray
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);
    {
        QPainter p(&pixmap);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor(255, 255, 255, 220), 6));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(6, 6, 52, 52, 12, 12);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(56, 189, 248)); // Cyan accent
        p.drawEllipse(26, 26, 12, 12);
    }

    m_trayIcon = std::make_unique<QSystemTrayIcon>(QIcon(pixmap), this);
    m_trayMenu = std::make_unique<QMenu>();

    m_powerAction = m_trayMenu->addAction(QStringLiteral("Enable Edge Light"), [this]() {
        if (m_controller) m_controller->togglePower();
    });

    m_trayMenu->addSeparator();

    auto *brightMenu = m_trayMenu->addMenu(QStringLiteral("Brightness"));
    brightMenu->addAction(QStringLiteral("25%"), [this]() { if (m_controller) m_controller->setBrightness(0.25f); });
    brightMenu->addAction(QStringLiteral("50%"), [this]() { if (m_controller) m_controller->setBrightness(0.50f); });
    brightMenu->addAction(QStringLiteral("75%"), [this]() { if (m_controller) m_controller->setBrightness(0.75f); });
    brightMenu->addAction(QStringLiteral("100%"), [this]() { if (m_controller) m_controller->setBrightness(1.0f); });

    m_trayMenu->addSeparator();

    m_trayMenu->addAction(QStringLiteral("Open Dashboard"), [this]() {
        emit showDashboardRequested();
    });

    m_trayMenu->addAction(QStringLiteral("Quit LumaEdge"), [this]() {
        emit quitRequested();
    });

    m_trayIcon->setContextMenu(m_trayMenu.get());
    m_trayIcon->setToolTip(QStringLiteral("LumaEdge - Virtual Ring Light"));

    connect(m_trayIcon.get(), &QSystemTrayIcon::activated, this, &SystemTray::onActivated);

    updateMenuState();
    m_trayIcon->show();
}

void SystemTray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        emit showDashboardRequested();
    }
}

void SystemTray::updateMenuState()
{
    if (!m_powerAction || !m_controller) return;

    if (m_controller->isActive()) {
        m_powerAction->setText(QStringLiteral("Turn Off Light"));
    } else {
        m_powerAction->setText(QStringLiteral("Turn On Light"));
    }
}

void SystemTray::show()
{
    if (m_trayIcon) m_trayIcon->show();
}

void SystemTray::hide()
{
    if (m_trayIcon) m_trayIcon->hide();
}

} // namespace LumaEdge
