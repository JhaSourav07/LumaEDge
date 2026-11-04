#pragma once

#include <QObject>
#include <QColor>
#include <QPointF>
#include <QSizeF>

namespace LumaEdge {

struct EdgeConfig {
    bool topEnabled{true};
    bool bottomEnabled{true};
    bool leftEnabled{true};
    bool rightEnabled{true};

    float topIntensity{1.0f};
    float bottomIntensity{1.0f};
    float leftIntensity{1.0f};
    float rightIntensity{1.0f};
};

struct LightParameters {
    float brightness{0.8f};      // 0.0 to 1.0
    float widthPercent{0.15f};   // 0.05 to 0.35
    int colorTemperature{4000};  // 2700K to 6500K
    EdgeConfig edges;

    bool cursorAwareness{false};
    QPointF cursorPos{-1.0f, -1.0f};
    float cursorCutoutRadius{140.0f};
    float cursorCutoutSoftness{0.4f};
};

class GradientEngine : public QObject {
    Q_OBJECT
public:
    explicit GradientEngine(QObject *parent = nullptr);

    // Calculates illumination alpha in [0.0, 1.0] at point (x,y) given screen bounds
    static float calculateAlphaAt(float x, float y, const QSizeF &screenSize, const LightParameters &params);

    // Smoothstep cubic interpolation
    static float smoothstep(float edge0, float edge1, float x);
};

} // namespace LumaEdge
