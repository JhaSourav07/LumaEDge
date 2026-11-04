#pragma once

#include <QObject>
#include <QString>
#include "../core/SettingsManager.h"

namespace LumaEdge {

class StartupManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isAutostartEnabled READ isAutostartEnabled WRITE setAutostartEnabled NOTIFY autostartChanged)

public:
    explicit StartupManager(SettingsManager *settings, QObject *parent = nullptr);

    bool isAutostartEnabled() const;
    void setAutostartEnabled(bool enabled);

signals:
    void autostartChanged();

private:
    SettingsManager *m_settings{nullptr};
    QString autostartFilePath() const;
};

} // namespace LumaEdge
