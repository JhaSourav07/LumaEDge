#include "SettingsManager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <algorithm>

namespace LumaEdge {

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    QString configPath = configDir + QStringLiteral("/lumaedge.conf");
    m_settings = std::make_unique<QSettings>(configPath, QSettings::IniFormat);

    load();
}

SettingsManager::~SettingsManager()
{
    save();
}

void SettingsManager::load()
{
    if (!m_settings) return;

    m_active = m_settings->value(QStringLiteral("lighting/active"), false).toBool();
    m_brightness = m_settings->value(QStringLiteral("lighting/brightness"), 0.8f).toFloat();
    m_widthPercent = m_settings->value(QStringLiteral("lighting/widthPercent"), 0.15f).toFloat();
    m_colorTemperature = m_settings->value(QStringLiteral("lighting/colorTemperature"), 4000).toInt();

    m_topEdgeEnabled = m_settings->value(QStringLiteral("edges/topEnabled"), true).toBool();
    m_bottomEdgeEnabled = m_settings->value(QStringLiteral("edges/bottomEnabled"), true).toBool();
    m_leftEdgeEnabled = m_settings->value(QStringLiteral("edges/leftEnabled"), true).toBool();
    m_rightEdgeEnabled = m_settings->value(QStringLiteral("edges/rightEnabled"), true).toBool();

    m_topEdgeIntensity = m_settings->value(QStringLiteral("edges/topIntensity"), 1.0f).toFloat();
    m_bottomEdgeIntensity = m_settings->value(QStringLiteral("edges/bottomIntensity"), 1.0f).toFloat();
    m_leftEdgeIntensity = m_settings->value(QStringLiteral("edges/leftIntensity"), 1.0f).toFloat();
    m_rightEdgeIntensity = m_settings->value(QStringLiteral("edges/rightIntensity"), 1.0f).toFloat();

    m_cursorAwarenessEnabled = m_settings->value(QStringLiteral("cursor/enabled"), false).toBool();
    m_cursorCutoutRadius = m_settings->value(QStringLiteral("cursor/cutoutRadius"), 140.0f).toFloat();
    m_cursorCutoutSoftness = m_settings->value(QStringLiteral("cursor/cutoutSoftness"), 0.4f).toFloat();

    m_autoLowLightEnabled = m_settings->value(QStringLiteral("camera/autoLowLightEnabled"), false).toBool();
    m_autoLowLightSensitivity = m_settings->value(QStringLiteral("camera/sensitivity"), 0.5f).toFloat();

    m_selectedScreenIndex = m_settings->value(QStringLiteral("display/selectedScreenIndex"), -1).toInt();
    m_autostartEnabled = m_settings->value(QStringLiteral("system/autostart"), false).toBool();
}

void SettingsManager::save()
{
    if (!m_settings) return;

    m_settings->setValue(QStringLiteral("lighting/active"), m_active);
    m_settings->setValue(QStringLiteral("lighting/brightness"), m_brightness);
    m_settings->setValue(QStringLiteral("lighting/widthPercent"), m_widthPercent);
    m_settings->setValue(QStringLiteral("lighting/colorTemperature"), m_colorTemperature);

    m_settings->setValue(QStringLiteral("edges/topEnabled"), m_topEdgeEnabled);
    m_settings->setValue(QStringLiteral("edges/bottomEnabled"), m_bottomEdgeEnabled);
    m_settings->setValue(QStringLiteral("edges/leftEnabled"), m_leftEdgeEnabled);
    m_settings->setValue(QStringLiteral("edges/rightEnabled"), m_rightEdgeEnabled);

    m_settings->setValue(QStringLiteral("edges/topIntensity"), m_topEdgeIntensity);
    m_settings->setValue(QStringLiteral("edges/bottomIntensity"), m_bottomEdgeIntensity);
    m_settings->setValue(QStringLiteral("edges/leftIntensity"), m_leftEdgeIntensity);
    m_settings->setValue(QStringLiteral("edges/rightIntensity"), m_rightEdgeIntensity);

    m_settings->setValue(QStringLiteral("cursor/enabled"), m_cursorAwarenessEnabled);
    m_settings->setValue(QStringLiteral("cursor/cutoutRadius"), m_cursorCutoutRadius);
    m_settings->setValue(QStringLiteral("cursor/cutoutSoftness"), m_cursorCutoutSoftness);

    m_settings->setValue(QStringLiteral("camera/autoLowLightEnabled"), m_autoLowLightEnabled);
    m_settings->setValue(QStringLiteral("camera/sensitivity"), m_autoLowLightSensitivity);

    m_settings->setValue(QStringLiteral("display/selectedScreenIndex"), m_selectedScreenIndex);
    m_settings->setValue(QStringLiteral("system/autostart"), m_autostartEnabled);

    m_settings->sync();
}

void SettingsManager::setActive(bool v)
{
    if (m_active != v) {
        m_active = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setBrightness(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_brightness - v) > 0.001f) {
        m_brightness = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setWidthPercent(float v)
{
    v = std::clamp(v, 0.05f, 0.50f);
    if (std::abs(m_widthPercent - v) > 0.001f) {
        m_widthPercent = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setColorTemperature(int v)
{
    v = std::clamp(v, 2700, 6500);
    if (m_colorTemperature != v) {
        m_colorTemperature = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setTopEdgeEnabled(bool v)
{
    if (m_topEdgeEnabled != v) {
        m_topEdgeEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setBottomEdgeEnabled(bool v)
{
    if (m_bottomEdgeEnabled != v) {
        m_bottomEdgeEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setLeftEdgeEnabled(bool v)
{
    if (m_leftEdgeEnabled != v) {
        m_leftEdgeEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setRightEdgeEnabled(bool v)
{
    if (m_rightEdgeEnabled != v) {
        m_rightEdgeEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setTopEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_topEdgeIntensity - v) > 0.001f) {
        m_topEdgeIntensity = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setBottomEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_bottomEdgeIntensity - v) > 0.001f) {
        m_bottomEdgeIntensity = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setLeftEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_leftEdgeIntensity - v) > 0.001f) {
        m_leftEdgeIntensity = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setRightEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_rightEdgeIntensity - v) > 0.001f) {
        m_rightEdgeIntensity = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setCursorAwarenessEnabled(bool v)
{
    if (m_cursorAwarenessEnabled != v) {
        m_cursorAwarenessEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setCursorCutoutRadius(float v)
{
    v = std::clamp(v, 40.0f, 400.0f);
    if (std::abs(m_cursorCutoutRadius - v) > 0.1f) {
        m_cursorCutoutRadius = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setCursorCutoutSoftness(float v)
{
    v = std::clamp(v, 0.05f, 0.95f);
    if (std::abs(m_cursorCutoutSoftness - v) > 0.01f) {
        m_cursorCutoutSoftness = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setAutoLowLightEnabled(bool v)
{
    if (m_autoLowLightEnabled != v) {
        m_autoLowLightEnabled = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setAutoLowLightSensitivity(float v)
{
    v = std::clamp(v, 0.1f, 1.0f);
    if (std::abs(m_autoLowLightSensitivity - v) > 0.01f) {
        m_autoLowLightSensitivity = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setSelectedScreenIndex(int v)
{
    if (m_selectedScreenIndex != v) {
        m_selectedScreenIndex = v;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::setAutostartEnabled(bool v)
{
    if (m_autostartEnabled != v) {
        m_autostartEnabled = v;
        save();
        emit settingsChanged();
    }
}

} // namespace LumaEdge
