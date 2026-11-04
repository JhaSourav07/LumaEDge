#pragma once

#include <QObject>
#include <QWindow>
#include <QPoint>

namespace LumaEdge {

class X11Integration : public QObject {
    Q_OBJECT
public:
    explicit X11Integration(QObject *parent = nullptr);

    void configureOverlayWindow(QWindow *window);
    QPoint queryPointerPosition();
};

} // namespace LumaEdge
