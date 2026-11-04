#include "DisplayManager.h"
#include <QGuiApplication>
#include <QVariantMap>

namespace LumaEdge {

DisplayManager::DisplayManager(LightController *controller, QObject *parent)
    : QObject(parent)
    , m_controller(controller)
{
    connect(qGuiApp, &QGuiApplication::screenAdded, this, &DisplayManager::onScreenAdded);
    connect(qGuiApp, &QGuiApplication::screenRemoved, this, &DisplayManager::onScreenRemoved);

    if (m_controller) {
        connect(m_controller, &LightController::screenSelectionChanged,
                this, &DisplayManager::selectionChanged);
    }

    refreshScreens();
}

int DisplayManager::selectedIndex() const
{
    return m_controller ? m_controller->selectedScreenIndex() : -1;
}

void DisplayManager::setSelectedIndex(int index)
{
    if (m_controller) {
        m_controller->setSelectedScreenIndex(index);
    }
}

QString DisplayManager::primaryScreenName() const
{
    QScreen *primary = QGuiApplication::primaryScreen();
    return primary ? primary->name() : QStringLiteral("Default");
}

void DisplayManager::refreshScreens()
{
    m_screenList.clear();
    const auto screens = QGuiApplication::screens();
    const QScreen *primary = QGuiApplication::primaryScreen();

    for (int i = 0; i < screens.size(); ++i) {
        QScreen *s = screens.at(i);
        QVariantMap screenInfo;
        screenInfo[QStringLiteral("index")] = i;
        screenInfo[QStringLiteral("name")] = s->name();
        screenInfo[QStringLiteral("manufacturer")] = s->manufacturer().isEmpty() ? QStringLiteral("Display") : s->manufacturer();
        screenInfo[QStringLiteral("model")] = s->model().isEmpty() ? s->name() : s->model();
        screenInfo[QStringLiteral("width")] = s->geometry().width();
        screenInfo[QStringLiteral("height")] = s->geometry().height();
        screenInfo[QStringLiteral("refreshRate")] = static_cast<int>(s->refreshRate());
        screenInfo[QStringLiteral("dpr")] = s->devicePixelRatio();
        screenInfo[QStringLiteral("isPrimary")] = (s == primary);
        screenInfo[QStringLiteral("displayName")] = QStringLiteral("%1 (%2x%3, %4Hz)")
            .arg(s->name())
            .arg(s->geometry().width())
            .arg(s->geometry().height())
            .arg(static_cast<int>(s->refreshRate()));

        m_screenList.append(screenInfo);
    }

    emit screensChanged();
}

void DisplayManager::onScreenAdded(QScreen *screen)
{
    Q_UNUSED(screen)
    refreshScreens();
}

void DisplayManager::onScreenRemoved(QScreen *screen)
{
    Q_UNUSED(screen)
    refreshScreens();
}

} // namespace LumaEdge
