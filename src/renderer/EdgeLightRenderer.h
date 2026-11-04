#pragma once

#include <QQuickItem>
#include <QColor>
#include <QPointF>
#include "ColorTemperature.h"
#include "GradientEngine.h"

namespace LumaEdge {

class EdgeLightRenderer : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(float brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(float widthPercent READ widthPercent WRITE setWidthPercent NOTIFY widthPercentChanged)
    Q_PROPERTY(int colorTemperature READ colorTemperature WRITE setColorTemperature NOTIFY colorTemperatureChanged)
    Q_PROPERTY(QColor lightColor READ lightColor NOTIFY lightColorChanged)

    Q_PROPERTY(bool topEdgeEnabled READ topEdgeEnabled WRITE setTopEdgeEnabled NOTIFY edgeSettingsChanged)
    Q_PROPERTY(bool bottomEdgeEnabled READ bottomEdgeEnabled WRITE setBottomEdgeEnabled NOTIFY edgeSettingsChanged)
    Q_PROPERTY(bool leftEdgeEnabled READ leftEdgeEnabled WRITE setLeftEdgeEnabled NOTIFY edgeSettingsChanged)
    Q_PROPERTY(bool rightEdgeEnabled READ rightEdgeEnabled WRITE setRightEdgeEnabled NOTIFY edgeSettingsChanged)

    Q_PROPERTY(float topEdgeIntensity READ topEdgeIntensity WRITE setTopEdgeIntensity NOTIFY edgeSettingsChanged)
    Q_PROPERTY(float bottomEdgeIntensity READ bottomEdgeIntensity WRITE setBottomEdgeIntensity NOTIFY edgeSettingsChanged)
    Q_PROPERTY(float leftEdgeIntensity READ leftEdgeIntensity WRITE setLeftEdgeIntensity NOTIFY edgeSettingsChanged)
    Q_PROPERTY(float rightEdgeIntensity READ rightEdgeIntensity WRITE setRightEdgeIntensity NOTIFY edgeSettingsChanged)

    Q_PROPERTY(bool cursorAwareness READ cursorAwareness WRITE setCursorAwareness NOTIFY cursorSettingsChanged)
    Q_PROPERTY(QPointF cursorPos READ cursorPos WRITE setCursorPos NOTIFY cursorSettingsChanged)
    Q_PROPERTY(float cursorCutoutRadius READ cursorCutoutRadius WRITE setCursorCutoutRadius NOTIFY cursorSettingsChanged)
    Q_PROPERTY(float cursorCutoutSoftness READ cursorCutoutSoftness WRITE setCursorCutoutSoftness NOTIFY cursorSettingsChanged)

public:
    explicit EdgeLightRenderer(QQuickItem *parent = nullptr);

    float brightness() const { return m_params.brightness; }
    void setBrightness(float b);

    float widthPercent() const { return m_params.widthPercent; }
    void setWidthPercent(float w);

    int colorTemperature() const { return m_params.colorTemperature; }
    void setColorTemperature(int kelvin);

    QColor lightColor() const { return m_lightColor; }

    bool topEdgeEnabled() const { return m_params.edges.topEnabled; }
    void setTopEdgeEnabled(bool v);

    bool bottomEdgeEnabled() const { return m_params.edges.bottomEnabled; }
    void setBottomEdgeEnabled(bool v);

    bool leftEdgeEnabled() const { return m_params.edges.leftEnabled; }
    void setLeftEdgeEnabled(bool v);

    bool rightEdgeEnabled() const { return m_params.edges.rightEnabled; }
    void setRightEdgeEnabled(bool v);

    float topEdgeIntensity() const { return m_params.edges.topIntensity; }
    void setTopEdgeIntensity(float v);

    float bottomEdgeIntensity() const { return m_params.edges.bottomIntensity; }
    void setBottomEdgeIntensity(float v);

    float leftEdgeIntensity() const { return m_params.edges.leftIntensity; }
    void setLeftEdgeIntensity(float v);

    float rightEdgeIntensity() const { return m_params.edges.rightIntensity; }
    void setRightEdgeIntensity(float v);

    bool cursorAwareness() const { return m_params.cursorAwareness; }
    void setCursorAwareness(bool v);

    QPointF cursorPos() const { return m_params.cursorPos; }
    void setCursorPos(const QPointF &pos);

    float cursorCutoutRadius() const { return m_params.cursorCutoutRadius; }
    void setCursorCutoutRadius(float r);

    float cursorCutoutSoftness() const { return m_params.cursorCutoutSoftness; }
    void setCursorCutoutSoftness(float s);

    const LightParameters &parameters() const { return m_params; }

signals:
    void brightnessChanged();
    void widthPercentChanged();
    void colorTemperatureChanged();
    void lightColorChanged();
    void edgeSettingsChanged();
    void cursorSettingsChanged();

private:
    LightParameters m_params;
    QColor m_lightColor;

    void updateLightColor();
};

} // namespace LumaEdge
