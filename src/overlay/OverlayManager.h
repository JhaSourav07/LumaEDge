#pragma once

#include <QObject>
#include <vector>
#include <memory>
#include "OverlayWindow.h"
#include "../core/LightController.h"

namespace LumaEdge {

class OverlayManager : public QObject {
    Q_OBJECT

public:
    explicit OverlayManager(LightController *controller, QObject *parent = nullptr);
    ~OverlayManager() override;

    void updateScreenOverlays();

private slots:
    void onScreenAdded(QScreen *screen);
    void onScreenRemoved(QScreen *screen);
    void onScreenSelectionChanged();

private:
    LightController *m_controller{nullptr};
    std::vector<std::unique_ptr<OverlayWindow>> m_overlays;
};

} // namespace LumaEdge
