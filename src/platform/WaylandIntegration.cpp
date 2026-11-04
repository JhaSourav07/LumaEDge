#include "WaylandIntegration.h"
#include <QDebug>

namespace LumaEdge {

WaylandIntegration::WaylandIntegration(QObject *parent)
    : QObject(parent)
{
}

void WaylandIntegration::configureOverlayWindow(QWindow *window)
{
    if (!window) return;

    // Apply Wayland-safe overlay flags
    window->setFlags(Qt::FramelessWindowHint |
                     Qt::WindowStaysOnTopHint |
                     Qt::WindowTransparentForInput |
                     Qt::WindowDoesNotAcceptFocus |
                     Qt::Tool);
}

} // namespace LumaEdge
