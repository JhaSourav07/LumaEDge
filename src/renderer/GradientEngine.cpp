#include "GradientEngine.h"
#include <cmath>
#include <algorithm>

namespace LumaEdge {

GradientEngine::GradientEngine(QObject *parent)
    : QObject(parent)
{
}

float GradientEngine::smoothstep(float edge0, float edge1, float x)
{
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float GradientEngine::calculateAlphaAt(float x, float y, const QSizeF &screenSize, const LightParameters &params)
{
    const float width = static_cast<float>(screenSize.width());
    const float height = static_cast<float>(screenSize.height());
    if (width <= 0.0f || height <= 0.0f) {
        return 0.0f;
    }

    const float shortDim = std::min(width, height);
    const float margin = std::max(10.0f, shortDim * params.widthPercent);

    // Distance to each edge
    const float dTop = y;
    const float dBottom = height - y;
    const float dLeft = x;
    const float dRight = width - x;

    // Corner proximity handling
    float alphaTop = 0.0f;
    float alphaBottom = 0.0f;
    float alphaLeft = 0.0f;
    float alphaRight = 0.0f;

    if (params.edges.topEnabled && dTop < margin) {
        float norm = 1.0f - (dTop / margin);
        alphaTop = smoothstep(0.0f, 1.0f, norm) * params.edges.topIntensity;
    }

    if (params.edges.bottomEnabled && dBottom < margin) {
        float norm = 1.0f - (dBottom / margin);
        alphaBottom = smoothstep(0.0f, 1.0f, norm) * params.edges.bottomIntensity;
    }

    if (params.edges.leftEnabled && dLeft < margin) {
        float norm = 1.0f - (dLeft / margin);
        alphaLeft = smoothstep(0.0f, 1.0f, norm) * params.edges.leftIntensity;
    }

    if (params.edges.rightEnabled && dRight < margin) {
        float norm = 1.0f - (dRight / margin);
        alphaRight = smoothstep(0.0f, 1.0f, norm) * params.edges.rightIntensity;
    }

    // Combine edges smoothly with spherical/radial corner blending
    // Screen corner checks:
    auto blendCorner = [&](float dx, float dy) {
        if (dx < margin && dy < margin) {
            float distFromCorner = std::sqrt(dx * dx + dy * dy);
            float cornerNorm = 1.0f - std::clamp(distFromCorner / margin, 0.0f, 1.0f);
            return smoothstep(0.0f, 1.0f, cornerNorm);
        }
        return -1.0f;
    };

    float cTL = blendCorner(dLeft, dTop);
    float cTR = blendCorner(dRight, dTop);
    float cBL = blendCorner(dLeft, dBottom);
    float cBR = blendCorner(dRight, dBottom);

    float combinedAlpha = 0.0f;
    if (cTL >= 0.0f && params.edges.topEnabled && params.edges.leftEnabled) {
        combinedAlpha = cTL * std::max(params.edges.topIntensity, params.edges.leftIntensity);
    } else if (cTR >= 0.0f && params.edges.topEnabled && params.edges.rightEnabled) {
        combinedAlpha = cTR * std::max(params.edges.topIntensity, params.edges.rightIntensity);
    } else if (cBL >= 0.0f && params.edges.bottomEnabled && params.edges.leftEnabled) {
        combinedAlpha = cBL * std::max(params.edges.bottomIntensity, params.edges.leftIntensity);
    } else if (cBR >= 0.0f && params.edges.bottomEnabled && params.edges.rightEnabled) {
        combinedAlpha = cBR * std::max(params.edges.bottomIntensity, params.edges.rightIntensity);
    } else {
        // Linear union of edge contributions
        combinedAlpha = 1.0f - (1.0f - alphaTop) * (1.0f - alphaBottom) * (1.0f - alphaLeft) * (1.0f - alphaRight);
    }

    combinedAlpha *= params.brightness;

    // Apply cursor proximity cutout if enabled
    if (params.cursorAwareness && params.cursorPos.x() >= 0.0f && params.cursorPos.y() >= 0.0f) {
        float dx = x - static_cast<float>(params.cursorPos.x());
        float dy = y - static_cast<float>(params.cursorPos.y());
        float distToCursor = std::sqrt(dx * dx + dy * dy);

        if (distToCursor < params.cursorCutoutRadius) {
            float innerRadius = params.cursorCutoutRadius * (1.0f - params.cursorCutoutSoftness);
            float cutoutAtten = smoothstep(innerRadius, params.cursorCutoutRadius, distToCursor);
            combinedAlpha *= cutoutAtten;
        }
    }

    return std::clamp(combinedAlpha, 0.0f, 1.0f);
}

} // namespace LumaEdge
