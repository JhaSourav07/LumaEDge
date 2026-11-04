#include "X11Integration.h"
#include <QCursor>

#if defined(HAVE_X11)
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#endif

namespace LumaEdge {

X11Integration::X11Integration(QObject *parent)
    : QObject(parent)
{
}

void X11Integration::configureOverlayWindow(QWindow *window)
{
    if (!window) return;

    window->setFlags(Qt::FramelessWindowHint |
                     Qt::WindowStaysOnTopHint |
                     Qt::WindowTransparentForInput |
                     Qt::WindowDoesNotAcceptFocus |
                     Qt::BypassWindowManagerHint);

#if defined(HAVE_X11)
    auto winId = window->winId();
    if (winId != 0) {
        Display *display = XOpenDisplay(nullptr);
        if (display) {
            XserverRegion region = XFixesCreateRegion(display, nullptr, 0);
            XFixesSetWindowShapeRegion(display, winId, ShapeInput, 0, 0, region);
            XFixesDestroyRegion(display, region);
            XFlush(display);
            XCloseDisplay(display);
        }
    }
#endif
}

QPoint X11Integration::queryPointerPosition()
{
    return QCursor::pos();
}

} // namespace LumaEdge
