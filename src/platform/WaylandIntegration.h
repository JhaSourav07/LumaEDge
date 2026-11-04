#pragma once

#include <QObject>
#include <QWindow>

namespace LumaEdge {

class WaylandIntegration : public QObject {
    Q_OBJECT
public:
    explicit WaylandIntegration(QObject *parent = nullptr);

    void configureOverlayWindow(QWindow *window);
};

} // namespace LumaEdge
