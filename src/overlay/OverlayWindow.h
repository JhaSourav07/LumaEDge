#pragma once

#include <QQuickView>
#include <QScreen>
#include <memory>
#include "../core/LightController.h"

namespace LumaEdge {

class OverlayWindow : public QQuickView {
    Q_OBJECT

public:
    explicit OverlayWindow(QScreen *targetScreen, LightController *controller, QWindow *parent = nullptr);
    ~OverlayWindow() override;

    QScreen *targetScreen() const { return m_screen; }
    void updateParameters();

public slots:
    void onScreenGeometryChanged(const QRect &geometry);
    void onVisibilityChanged();

protected:
    void exposeEvent(QExposeEvent *event) override;

private:
    QScreen *m_screen{nullptr};
    LightController *m_controller{nullptr};

    void setupWindow();
    void loadQml();
};

} // namespace LumaEdge
