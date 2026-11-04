#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec4 lightColor;       // rgb, 1.0
    vec2 resolution;       // width, height in pixels
    vec4 edgeIntensities;  // x=top, y=bottom, z=left, w=right
    vec2 cursorPos;        // in pixels
    float brightness;      // 0.0 to 1.0
    float widthPercent;    // 0.05 to 0.35
    float cursorRadius;    // in pixels
    float cursorSoftness;  // 0.0 to 1.0
    float cursorEnabled;   // 1.0 if enabled, 0.0 otherwise
};

float smoothstep_hermite(float e0, float e1, float x) {
    float t = clamp((x - e0) / (e1 - e0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

void main() {
    vec2 pixelPos = qt_TexCoord0 * resolution;
    float shortDim = min(resolution.x, resolution.y);
    float margin = max(8.0, shortDim * widthPercent);

    // Distances to screen edges
    float dTop = pixelPos.y;
    float dBottom = resolution.y - pixelPos.y;
    float dLeft = pixelPos.x;
    float dRight = resolution.x - pixelPos.x;

    // Edge falloffs
    float aTop = (dTop < margin) ? smoothstep_hermite(0.0, 1.0, 1.0 - (dTop / margin)) * edgeIntensities.x : 0.0;
    float aBottom = (dBottom < margin) ? smoothstep_hermite(0.0, 1.0, 1.0 - (dBottom / margin)) * edgeIntensities.y : 0.0;
    float aLeft = (dLeft < margin) ? smoothstep_hermite(0.0, 1.0, 1.0 - (dLeft / margin)) * edgeIntensities.z : 0.0;
    float aRight = (dRight < margin) ? smoothstep_hermite(0.0, 1.0, 1.0 - (dRight / margin)) * edgeIntensities.w : 0.0;

    // Corner radial blending
    float alpha = 0.0;
    bool isCorner = false;

    if (dLeft < margin && dTop < margin && edgeIntensities.x > 0.0 && edgeIntensities.z > 0.0) {
        float r = length(vec2(dLeft, dTop));
        if (r < margin) {
            float cornerNorm = 1.0 - clamp(r / margin, 0.0, 1.0);
            alpha = smoothstep_hermite(0.0, 1.0, cornerNorm) * max(edgeIntensities.x, edgeIntensities.z);
            isCorner = true;
        }
    } else if (dRight < margin && dTop < margin && edgeIntensities.x > 0.0 && edgeIntensities.w > 0.0) {
        float r = length(vec2(dRight, dTop));
        if (r < margin) {
            float cornerNorm = 1.0 - clamp(r / margin, 0.0, 1.0);
            alpha = smoothstep_hermite(0.0, 1.0, cornerNorm) * max(edgeIntensities.x, edgeIntensities.w);
            isCorner = true;
        }
    } else if (dLeft < margin && dBottom < margin && edgeIntensities.y > 0.0 && edgeIntensities.z > 0.0) {
        float r = length(vec2(dLeft, dBottom));
        if (r < margin) {
            float cornerNorm = 1.0 - clamp(r / margin, 0.0, 1.0);
            alpha = smoothstep_hermite(0.0, 1.0, cornerNorm) * max(edgeIntensities.y, edgeIntensities.z);
            isCorner = true;
        }
    } else if (dRight < margin && dBottom < margin && edgeIntensities.y > 0.0 && edgeIntensities.w > 0.0) {
        float r = length(vec2(dRight, dBottom));
        if (r < margin) {
            float cornerNorm = 1.0 - clamp(r / margin, 0.0, 1.0);
            alpha = smoothstep_hermite(0.0, 1.0, cornerNorm) * max(edgeIntensities.y, edgeIntensities.w);
            isCorner = true;
        }
    }

    if (!isCorner) {
        alpha = 1.0 - (1.0 - aTop) * (1.0 - aBottom) * (1.0 - aLeft) * (1.0 - aRight);
    }

    // Apply brightness
    alpha *= brightness;

    // Intelligent cursor proximity cutout
    if (cursorEnabled > 0.5 && cursorPos.x >= 0.0 && cursorPos.y >= 0.0) {
        float distToCursor = length(pixelPos - cursorPos);
        if (distToCursor < cursorRadius) {
            float innerRadius = cursorRadius * (1.0 - cursorSoftness);
            float cutoutAtten = smoothstep_hermite(innerRadius, cursorRadius, distToCursor);
            alpha *= cutoutAtten;
        }
    }

    alpha = clamp(alpha * qt_Opacity, 0.0, 1.0);
    // Pre-multiplied alpha output for Qt Quick compositing
    fragColor = vec4(lightColor.rgb * alpha, alpha);
}
