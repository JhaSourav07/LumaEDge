#include "StartupManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

namespace LumaEdge {

StartupManager::StartupManager(SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
}

QString StartupManager::autostartFilePath() const
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return configDir + QStringLiteral("/autostart/lumaedge.desktop");
}

bool StartupManager::isAutostartEnabled() const
{
    return QFile::exists(autostartFilePath());
}

void StartupManager::setAutostartEnabled(bool enabled)
{
    QString filePath = autostartFilePath();

    if (enabled) {
        QFileInfo fi(filePath);
        QDir().mkpath(fi.dir().absolutePath());

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "[Desktop Entry]\n";
            out << "Type=Application\n";
            out << "Name=LumaEdge\n";
            out << "Comment=Virtual Lighting for Linux\n";
            out << "Exec=" << QCoreApplication::applicationFilePath() << " --background\n";
            out << "Icon=lumaedge\n";
            out << "Terminal=false\n";
            out << "Categories=Utility;Graphics;\n";
            out << "X-GNOME-Autostart-enabled=true\n";
            file.close();
        }
    } else {
        QFile::remove(filePath);
    }

    if (m_settings) {
        m_settings->setAutostartEnabled(enabled);
    }

    emit autostartChanged();
}

} // namespace LumaEdge
