#include "LightController.h"
#include <algorithm>

namespace LumaEdge {

LightController::LightController(SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    syncFromSettings();
    updateLightColor();
}

void LightController::syncFromSettings()
{
    if (!m_settings) return;

    m_active = m_settings->isActive();
    m_brightness = m_settings->brightness();
    m_widthPercent = m_settings->widthPercent();
    m_colorTemperature = m_settings->colorTemperature();

    m_topEdgeEnabled = m_settings->topEdgeEnabled();
    m_bottomEdgeEnabled = m_settings->bottomEdgeEnabled();
    m_leftEdgeEnabled = m_settings->leftEdgeEnabled();
    m_rightEdgeEnabled = m_settings->rightEdgeEnabled();

    m_topEdgeIntensity = m_settings->topEdgeIntensity();
    m_bottomEdgeIntensity = m_settings->bottomEdgeIntensity();
    m_leftEdgeIntensity = m_settings->leftEdgeIntensity();
    m_rightEdgeIntensity = m_settings->rightEdgeIntensity();

    m_cursorAwarenessEnabled = m_settings->cursorAwarenessEnabled();
    m_cursorCutoutRadius = m_settings->cursorCutoutRadius();
    m_cursorCutoutSoftness = m_settings->cursorCutoutSoftness();

    m_autoLowLightEnabled = m_settings->autoLowLightEnabled();
    m_autoLowLightSensitivity = m_settings->autoLowLightSensitivity();
    m_selectedScreenIndex = m_settings->selectedScreenIndex();
}

void LightController::updateLightColor()
{
    QColor color = ColorTemperature::kelvinToRgb(m_colorTemperature);
    if (color != m_lightColor) {
        m_lightColor = color;
        emit lightColorChanged();
    }
}

void LightController::setActive(bool v)
{
    if (m_active != v) {
        m_active = v;
        if (m_settings) m_settings->setActive(v);
        emit activeChanged();
    }
}

void LightController::togglePower()
{
    setActive(!m_active);
}

void LightController::setBrightness(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_brightness - v) > 0.001f) {
        m_brightness = v;
        if (m_settings) m_settings->setBrightness(v);
        emit brightnessChanged();
    }
}

void LightController::setWidthPercent(float v)
{
    v = std::clamp(v, 0.05f, 0.50f);
    if (std::abs(m_widthPercent - v) > 0.001f) {
        m_widthPercent = v;
        if (m_settings) m_settings->setWidthPercent(v);
        emit widthPercentChanged();
    }
}

void LightController::setColorTemperature(int v)
{
    v = ColorTemperature::clampKelvin(v);
    if (m_colorTemperature != v) {
        m_colorTemperature = v;
        if (m_settings) m_settings->setColorTemperature(v);
        updateLightColor();
        emit colorTemperatureChanged();
    }
}

void LightController::setPreset(int kelvin)
{
    setColorTemperature(kelvin);
}

void LightController::setTopEdgeEnabled(bool v)
{
    if (m_topEdgeEnabled != v) {
        m_topEdgeEnabled = v;
        if (m_settings) m_settings->setTopEdgeEnabled(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setBottomEdgeEnabled(bool v)
{
    if (m_bottomEdgeEnabled != v) {
        m_bottomEdgeEnabled = v;
        if (m_settings) m_settings->setBottomEdgeEnabled(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setLeftEdgeEnabled(bool v)
{
    if (m_leftEdgeEnabled != v) {
        m_leftEdgeEnabled = v;
        if (m_settings) m_settings->setLeftEdgeEnabled(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setRightEdgeEnabled(bool v)
{
    if (m_rightEdgeEnabled != v) {
        m_rightEdgeEnabled = v;
        if (m_settings) m_settings->setRightEdgeEnabled(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setTopEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_topEdgeIntensity - v) > 0.001f) {
        m_topEdgeIntensity = v;
        if (m_settings) m_settings->setTopEdgeIntensity(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setBottomEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_bottomEdgeIntensity - v) > 0.001f) {
        m_bottomEdgeIntensity = v;
        if (m_settings) m_settings->setBottomEdgeIntensity(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setLeftEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_leftEdgeIntensity - v) > 0.001f) {
        m_leftEdgeIntensity = v;
        if (m_settings) m_settings->setLeftEdgeIntensity(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setRightEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_rightEdgeIntensity - v) > 0.001f) {
        m_rightEdgeIntensity = v;
        if (m_settings) m_settings->setRightEdgeIntensity(v);
        emit edgeSettingsChanged();
    }
}

void LightController::setCursorAwarenessEnabled(bool v)
{
    if (m_cursorAwarenessEnabled != v) {
        m_cursorAwarenessEnabled = v;
        if (m_settings) m_settings->setCursorAwarenessEnabled(v);
        emit cursorSettingsChanged();
    }
}

void LightController::setCursorPos(const QPointF &pos)
{
    if (m_cursorPos != pos) {
        m_cursorPos = pos;
        emit cursorPosChanged();
    }
}

void LightController::setCursorCutoutRadius(float v)
{
    v = std::clamp(v, 40.0f, 400.0f);
    if (std::abs(m_cursorCutoutRadius - v) > 0.1f) {
        m_cursorCutoutRadius = v;
        if (m_settings) m_settings->setCursorCutoutRadius(v);
        emit cursorSettingsChanged();
    }
}

void LightController::setCursorCutoutSoftness(float v)
{
    v = std::clamp(v, 0.05f, 0.95f);
    if (std::abs(m_cursorCutoutSoftness - v) > 0.01f) {
        m_cursorCutoutSoftness = v;
        if (m_settings) m_settings->setCursorCutoutSoftness(v);
        emit cursorSettingsChanged();
    }
}

void LightController::setAutoLowLightEnabled(bool v)
{
    if (m_autoLowLightEnabled != v) {
        m_autoLowLightEnabled = v;
        if (m_settings) m_settings->setAutoLowLightEnabled(v);
        emit autoLowLightChanged();
    }
}

void LightController::setAutoLowLightSensitivity(float v)
{
    v = std::clamp(v, 0.1f, 1.0f);
    if (std::abs(m_autoLowLightSensitivity - v) > 0.01f) {
        m_autoLowLightSensitivity = v;
        if (m_settings) m_settings->setAutoLowLightSensitivity(v);
        emit autoLowLightChanged();
    }
}

void LightController::setSelectedScreenIndex(int v)
{
    if (m_selectedScreenIndex != v) {
        m_selectedScreenIndex = v;
        if (m_settings) m_settings->setSelectedScreenIndex(v);
        emit screenSelectionChanged();
    }
}

void LightController::resetToDefaults()
{
    setActive(false);
    setBrightness(0.8f);
    setWidthPercent(0.15f);
    setColorTemperature(4000);

    setTopEdgeEnabled(true);
    setBottomEdgeEnabled(true);
    setLeftEdgeEnabled(true);
    setRightEdgeEnabled(true);

    setTopEdgeIntensity(1.0f);
    setBottomEdgeIntensity(1.0f);
    setLeftEdgeIntensity(1.0f);
    setRightEdgeIntensity(1.0f);

    setCursorAwarenessEnabled(false);
    setCursorCutoutRadius(140.0f);
    setCursorCutoutSoftness(0.4f);

    setAutoLowLightEnabled(false);
    setAutoLowLightSensitivity(0.5f);
    setSelectedScreenIndex(-1);
}

void LightController::boostLight()
{
    setActive(true);
    setBrightness(1.0f);
    setWidthPercent(0.50f);
    setColorTemperature(6500);

    setTopEdgeEnabled(true);
    setBottomEdgeEnabled(true);
    setLeftEdgeEnabled(true);
    setRightEdgeEnabled(true);

    setTopEdgeIntensity(1.0f);
    setBottomEdgeIntensity(1.0f);
    setLeftEdgeIntensity(1.0f);
    setRightEdgeIntensity(1.0f);
}

LightParameters LightController::parameters() const
{
    LightParameters p;
    p.brightness = m_brightness;
    p.widthPercent = m_widthPercent;
    p.colorTemperature = m_colorTemperature;
    p.edges.topEnabled = m_topEdgeEnabled;
    p.edges.bottomEnabled = m_bottomEdgeEnabled;
    p.edges.leftEnabled = m_leftEdgeEnabled;
    p.edges.rightEnabled = m_rightEdgeEnabled;
    p.edges.topIntensity = m_topEdgeIntensity;
    p.edges.bottomIntensity = m_bottomEdgeIntensity;
    p.edges.leftIntensity = m_leftEdgeIntensity;
    p.edges.rightIntensity = m_rightEdgeIntensity;
    p.cursorAwareness = m_cursorAwarenessEnabled;
    p.cursorPos = m_cursorPos;
    p.cursorCutoutRadius = m_cursorCutoutRadius;
    p.cursorCutoutSoftness = m_cursorCutoutSoftness;
    return p;
}

} // namespace LumaEdge
