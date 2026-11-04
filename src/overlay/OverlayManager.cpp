#include "OverlayManager.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

namespace LumaEdge {

OverlayManager::OverlayManager(LightController *controller, QObject *parent)
    : QObject(parent)
    , m_controller(controller)
{
    if (m_controller) {
        connect(m_controller, &LightController::screenSelectionChanged,
                this, &OverlayManager::onScreenSelectionChanged);
    }

    connect(qGuiApp, &QGuiApplication::screenAdded, this, &OverlayManager::onScreenAdded);
    connect(qGuiApp, &QGuiApplication::screenRemoved, this, &OverlayManager::onScreenRemoved);

    updateScreenOverlays();
}

OverlayManager::~OverlayManager()
{
    m_overlays.clear();
}

void OverlayManager::updateScreenOverlays()
{
    m_overlays.clear();

    const auto screens = QGuiApplication::screens();
    int selectedIndex = m_controller ? m_controller->selectedScreenIndex() : -1;

    for (int i = 0; i < screens.size(); ++i) {
        if (selectedIndex >= 0 && selectedIndex != i) {
            continue; // Skip screens not selected
        }

        auto overlay = std::make_unique<OverlayWindow>(screens.at(i), m_controller);
        m_overlays.push_back(std::move(overlay));
    }
}

void OverlayManager::onScreenAdded(QScreen *screen)
{
    Q_UNUSED(screen)
    updateScreenOverlays();
}

void OverlayManager::onScreenRemoved(QScreen *screen)
{
    Q_UNUSED(screen)
    updateScreenOverlays();
}

void OverlayManager::onScreenSelectionChanged()
{
    updateScreenOverlays();
}

} // namespace LumaEdge
