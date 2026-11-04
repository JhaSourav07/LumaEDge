#pragma once

#include <QObject>
#include <QSettings>
#include <memory>

namespace LumaEdge {

class SettingsManager : public QObject {
    Q_OBJECT

public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager() override;

    void save();
    void load();

    // Values
    bool isActive() const { return m_active; }
    void setActive(bool v);

    float brightness() const { return m_brightness; }
    void setBrightness(float v);

    float widthPercent() const { return m_widthPercent; }
    void setWidthPercent(float v);

    int colorTemperature() const { return m_colorTemperature; }
    void setColorTemperature(int v);

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

    bool autostartEnabled() const { return m_autostartEnabled; }
    void setAutostartEnabled(bool v);

signals:
    void settingsChanged();

private:
    std::unique_ptr<QSettings> m_settings;

    bool m_active{false};
    float m_brightness{0.8f};
    float m_widthPercent{0.15f};
    int m_colorTemperature{4000};

    bool m_topEdgeEnabled{true};
    bool m_bottomEdgeEnabled{true};
    bool m_leftEdgeEnabled{true};
    bool m_rightEdgeEnabled{true};

    float m_topEdgeIntensity{1.0f};
    float m_bottomEdgeIntensity{1.0f};
    float m_leftEdgeIntensity{1.0f};
    float m_rightEdgeIntensity{1.0f};

    bool m_cursorAwarenessEnabled{false};
    float m_cursorCutoutRadius{140.0f};
    float m_cursorCutoutSoftness{0.4f};

    bool m_autoLowLightEnabled{false};
    float m_autoLowLightSensitivity{0.5f};

    int m_selectedScreenIndex{-1}; // -1 = All screens
    bool m_autostartEnabled{false};
};

} // namespace LumaEdge
