#include "PlatformDetector.h"
#include <QGuiApplication>
#include <QProcessEnvironment>

namespace LumaEdge {

PlatformDetector::PlatformDetector(QObject *parent)
    : QObject(parent)
{
    detect();
}

void PlatformDetector::detect()
{
    const auto env = QProcessEnvironment::systemEnvironment();
    const QString sessionTypeEnv = env.value(QStringLiteral("XDG_SESSION_TYPE")).toLower();
    const QString waylandDisplay = env.value(QStringLiteral("WAYLAND_DISPLAY"));
    const QString platformName = QGuiApplication::platformName().toLower();

    if (platformName.contains(QStringLiteral("wayland")) || sessionTypeEnv == QStringLiteral("wayland") || !waylandDisplay.isEmpty()) {
        m_sessionType = SessionType::Wayland;
    } else if (platformName.contains(QStringLiteral("xcb")) || sessionTypeEnv == QStringLiteral("x11")) {
        m_sessionType = SessionType::X11;
    } else {
        m_sessionType = SessionType::Unknown;
    }

    const QString desktopEnv = env.value(QStringLiteral("XDG_CURRENT_DESKTOP")).toUpper();
    if (desktopEnv.contains(QStringLiteral("GNOME"))) {
        m_desktopEnvironment = DesktopEnvironment::GNOME;
    } else if (desktopEnv.contains(QStringLiteral("KDE")) || desktopEnv.contains(QStringLiteral("PLASMA"))) {
        m_desktopEnvironment = DesktopEnvironment::KDE;
    } else {
        m_desktopEnvironment = DesktopEnvironment::Other;
    }
}

QString PlatformDetector::sessionTypeName() const
{
    switch (m_sessionType) {
    case SessionType::Wayland: return QStringLiteral("Wayland");
    case SessionType::X11: return QStringLiteral("X11");
    default: return QStringLiteral("Unknown");
    }
}

QString PlatformDetector::desktopEnvironmentName() const
{
    switch (m_desktopEnvironment) {
    case DesktopEnvironment::GNOME: return QStringLiteral("GNOME");
    case DesktopEnvironment::KDE: return QStringLiteral("KDE Plasma");
    default: return QStringLiteral("Standard Desktop");
    }
}

bool PlatformDetector::supportsGlobalCursorTracking() const
{
    // On X11/Xwayland, global cursor position is readily queryable.
    // On native Wayland with an input-transparent overlay window, unprivileged
    // cursor snooping is restricted by compositor security.
    return (m_sessionType == SessionType::X11);
}

QString PlatformDetector::cursorTrackingStatus() const
{
    if (supportsGlobalCursorTracking()) {
        return QStringLiteral("Active (Global pointer tracking enabled)");
    }
    if (m_sessionType == SessionType::Wayland) {
        return QStringLiteral("Restricted by Wayland security (requires Xwayland or privileged portal)");
    }
    return QStringLiteral("Unavailable in current session");
}

} // namespace LumaEdge
