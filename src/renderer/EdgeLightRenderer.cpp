#include "EdgeLightRenderer.h"
#include <algorithm>

namespace LumaEdge {

EdgeLightRenderer::EdgeLightRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    updateLightColor();
}

void EdgeLightRenderer::updateLightColor()
{
    QColor newColor = ColorTemperature::kelvinToRgb(m_params.colorTemperature);
    if (newColor != m_lightColor) {
        m_lightColor = newColor;
        emit lightColorChanged();
        update();
    }
}

void EdgeLightRenderer::setBrightness(float b)
{
    b = std::clamp(b, 0.0f, 1.0f);
    if (std::abs(m_params.brightness - b) > 0.001f) {
        m_params.brightness = b;
        emit brightnessChanged();
        update();
    }
}

void EdgeLightRenderer::setWidthPercent(float w)
{
    w = std::clamp(w, 0.05f, 0.50f);
    if (std::abs(m_params.widthPercent - w) > 0.001f) {
        m_params.widthPercent = w;
        emit widthPercentChanged();
        update();
    }
}

void EdgeLightRenderer::setColorTemperature(int kelvin)
{
    kelvin = ColorTemperature::clampKelvin(kelvin);
    if (m_params.colorTemperature != kelvin) {
        m_params.colorTemperature = kelvin;
        updateLightColor();
        emit colorTemperatureChanged();
    }
}

void EdgeLightRenderer::setTopEdgeEnabled(bool v)
{
    if (m_params.edges.topEnabled != v) {
        m_params.edges.topEnabled = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setBottomEdgeEnabled(bool v)
{
    if (m_params.edges.bottomEnabled != v) {
        m_params.edges.bottomEnabled = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setLeftEdgeEnabled(bool v)
{
    if (m_params.edges.leftEnabled != v) {
        m_params.edges.leftEnabled = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setRightEdgeEnabled(bool v)
{
    if (m_params.edges.rightEnabled != v) {
        m_params.edges.rightEnabled = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setTopEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_params.edges.topIntensity - v) > 0.001f) {
        m_params.edges.topIntensity = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setBottomEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_params.edges.bottomIntensity - v) > 0.001f) {
        m_params.edges.bottomIntensity = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setLeftEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_params.edges.leftIntensity - v) > 0.001f) {
        m_params.edges.leftIntensity = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setRightEdgeIntensity(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    if (std::abs(m_params.edges.rightIntensity - v) > 0.001f) {
        m_params.edges.rightIntensity = v;
        emit edgeSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setCursorAwareness(bool v)
{
    if (m_params.cursorAwareness != v) {
        m_params.cursorAwareness = v;
        emit cursorSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setCursorPos(const QPointF &pos)
{
    if (m_params.cursorPos != pos) {
        m_params.cursorPos = pos;
        emit cursorSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setCursorCutoutRadius(float r)
{
    r = std::clamp(r, 40.0f, 400.0f);
    if (std::abs(m_params.cursorCutoutRadius - r) > 0.1f) {
        m_params.cursorCutoutRadius = r;
        emit cursorSettingsChanged();
        update();
    }
}

void EdgeLightRenderer::setCursorCutoutSoftness(float s)
{
    s = std::clamp(s, 0.05f, 0.95f);
    if (std::abs(m_params.cursorCutoutSoftness - s) > 0.01f) {
        m_params.cursorCutoutSoftness = s;
        emit cursorSettingsChanged();
        update();
    }
}

} // namespace LumaEdge
