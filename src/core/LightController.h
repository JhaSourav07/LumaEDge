#pragma once

#include <QObject>
#include <QColor>
#include <QPointF>
#include "SettingsManager.h"
#include "renderer/ColorTemperature.h"
#include "renderer/GradientEngine.h"

namespace LumaEdge {

class LightController : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
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

    Q_PROPERTY(bool cursorAwarenessEnabled READ cursorAwarenessEnabled WRITE setCursorAwarenessEnabled NOTIFY cursorSettingsChanged)
    Q_PROPERTY(QPointF cursorPos READ cursorPos WRITE setCursorPos NOTIFY cursorPosChanged)
    Q_PROPERTY(float cursorCutoutRadius READ cursorCutoutRadius WRITE setCursorCutoutRadius NOTIFY cursorSettingsChanged)
    Q_PROPERTY(float cursorCutoutSoftness READ cursorCutoutSoftness WRITE setCursorCutoutSoftness NOTIFY cursorSettingsChanged)

    Q_PROPERTY(bool autoLowLightEnabled READ autoLowLightEnabled WRITE setAutoLowLightEnabled NOTIFY autoLowLightChanged)
    Q_PROPERTY(float autoLowLightSensitivity READ autoLowLightSensitivity WRITE setAutoLowLightSensitivity NOTIFY autoLowLightChanged)
    Q_PROPERTY(int selectedScreenIndex READ selectedScreenIndex WRITE setSelectedScreenIndex NOTIFY screenSelectionChanged)

public:
    explicit LightController(SettingsManager *settings, QObject *parent = nullptr);

    bool isActive() const { return m_active; }
    void setActive(bool v);

    float brightness() const { return m_brightness; }
    void setBrightness(float v);

    float widthPercent() const { return m_widthPercent; }
    void setWidthPercent(float v);

    int colorTemperature() const { return m_colorTemperature; }
    void setColorTemperature(int v);

    QColor lightColor() const { return m_lightColor; }

    bool topEdgeEnabled() const { return m_topEdgeEnabled; }
    void setTopEdgeEnabled(bool v);

    bool bottomEdgeEnabled() const { return m_bottomEdgeEnabled; }
    void setBottomEdgeEnabled(bool v);

    bool leftEdgeEnabled() const { return m_leftEdgeEnabled; }
    void setLeftEdgeEnabled(bool v);

    bool rightEdgeEnabled() const { return m_rightEdgeEnabled; }
    void setRightEdgeEnabled(bool v);

    float topEdgeIntensity() const { return m_topEdgeIntensity; }
    void setTopEdgeIntensity(float v);

    float bottomEdgeIntensity() const { return m_bottomEdgeIntensity; }
    void setBottomEdgeIntensity(float v);

    float leftEdgeIntensity() const { return m_leftEdgeIntensity; }
    void setLeftEdgeIntensity(float v);

    float rightEdgeIntensity() const { return m_rightEdgeIntensity; }
    void setRightEdgeIntensity(float v);

    bool cursorAwarenessEnabled() const { return m_cursorAwarenessEnabled; }
    void setCursorAwarenessEnabled(bool v);

    QPointF cursorPos() const { return m_cursorPos; }
    void setCursorPos(const QPointF &pos);

    float cursorCutoutRadius() const { return m_cursorCutoutRadius; }
    void setCursorCutoutRadius(float v);

    float cursorCutoutSoftness() const { return m_cursorCutoutSoftness; }
    void setCursorCutoutSoftness(float v);

    bool autoLowLightEnabled() const { return m_autoLowLightEnabled; }
    void setAutoLowLightEnabled(bool v);

    float autoLowLightSensitivity() const { return m_autoLowLightSensitivity; }
    void setAutoLowLightSensitivity(float v);

    int selectedScreenIndex() const { return m_selectedScreenIndex; }
    void setSelectedScreenIndex(int v);

    LightParameters parameters() const;

public slots:
    void togglePower();
    void setPreset(int kelvin);
    void resetToDefaults();

signals:
    void activeChanged();
    void brightnessChanged();
    void widthPercentChanged();
    void colorTemperatureChanged();
    void lightColorChanged();
    void edgeSettingsChanged();
    void cursorSettingsChanged();
    void cursorPosChanged();
    void autoLowLightChanged();
    void screenSelectionChanged();

private:
    SettingsManager *m_settings{nullptr};

    bool m_active{false};
    float m_brightness{0.8f};
    float m_widthPercent{0.15f};
    int m_colorTemperature{4000};
    QColor m_lightColor;

    bool m_topEdgeEnabled{true};
    bool m_bottomEdgeEnabled{true};
    bool m_leftEdgeEnabled{true};
    bool m_rightEdgeEnabled{true};

    float m_topEdgeIntensity{1.0f};
    float m_bottomEdgeIntensity{1.0f};
    float m_leftEdgeIntensity{1.0f};
    float m_rightEdgeIntensity{1.0f};

    bool m_cursorAwarenessEnabled{false};
    QPointF m_cursorPos{-1.0f, -1.0f};
    float m_cursorCutoutRadius{140.0f};
    float m_cursorCutoutSoftness{0.4f};

    bool m_autoLowLightEnabled{false};
    float m_autoLowLightSensitivity{0.5f};
    int m_selectedScreenIndex{-1};

    void updateLightColor();
    void syncFromSettings();
};

} // namespace LumaEdge
