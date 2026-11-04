#pragma once

#include <QColor>
#include <QVector3D>
#include <QString>

namespace LumaEdge {

struct ColorPreset {
    QString name;
    int kelvin;
};

class ColorTemperature {
public:
    static constexpr int MinKelvin = 2700;
    static constexpr int MaxKelvin = 6500;
    static constexpr int DefaultKelvin = 4000;

    static const ColorPreset PresetCandle;
    static const ColorPreset PresetWarm;
    static const ColorPreset PresetNeutral;
    static const ColorPreset PresetCool;
    static const ColorPreset PresetDaylight;

    // Converts Kelvin temperature (2700 - 6500) to calibrated sRGB QColor
    static QColor kelvinToRgb(int kelvin);

    // Converts Kelvin to normalized RGB float components in [0.0, 1.0]
    static QVector3D kelvinToRgbNormalized(int kelvin);

    // Validates and clamps Kelvin input to valid range
    static int clampKelvin(int kelvin);
};

} // namespace LumaEdge
