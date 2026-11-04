#pragma once

#include <QObject>
#include <QString>

namespace LumaEdge {

enum class SessionType {
    Wayland,
    X11,
    Unknown
};

enum class DesktopEnvironment {
    GNOME,
    KDE,
    Other
};

class PlatformDetector : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sessionTypeName READ sessionTypeName CONSTANT)
    Q_PROPERTY(QString desktopEnvironmentName READ desktopEnvironmentName CONSTANT)
    Q_PROPERTY(bool isWayland READ isWayland CONSTANT)
    Q_PROPERTY(bool isX11 READ isX11 CONSTANT)
    Q_PROPERTY(bool supportsGlobalCursorTracking READ supportsGlobalCursorTracking CONSTANT)
    Q_PROPERTY(QString cursorTrackingStatus READ cursorTrackingStatus CONSTANT)

public:
    explicit PlatformDetector(QObject *parent = nullptr);

    SessionType sessionType() const { return m_sessionType; }
    DesktopEnvironment desktopEnvironment() const { return m_desktopEnvironment; }

    QString sessionTypeName() const;
    QString desktopEnvironmentName() const;
    bool isWayland() const { return m_sessionType == SessionType::Wayland; }
    bool isX11() const { return m_sessionType == SessionType::X11; }
    bool supportsGlobalCursorTracking() const;
    QString cursorTrackingStatus() const;

private:
    SessionType m_sessionType{SessionType::Unknown};
    DesktopEnvironment m_desktopEnvironment{DesktopEnvironment::Other};
    void detect();
};

} // namespace LumaEdge
