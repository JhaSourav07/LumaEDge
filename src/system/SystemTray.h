#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <memory>
#include "../core/LightController.h"

namespace LumaEdge {

class SystemTray : public QObject {
    Q_OBJECT

public:
    explicit SystemTray(LightController *controller, QObject *parent = nullptr);
    ~SystemTray() override;

    bool isAvailable() const;
    void show();
    void hide();

signals:
    void showDashboardRequested();
    void quitRequested();

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void updateMenuState();

private:
    LightController *m_controller{nullptr};
    std::unique_ptr<QSystemTrayIcon> m_trayIcon;
    std::unique_ptr<QMenu> m_trayMenu;
    QAction *m_powerAction{nullptr};

    void setupTray();
};

} // namespace LumaEdge
