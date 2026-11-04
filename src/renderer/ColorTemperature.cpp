#include "ColorTemperature.h"
#include <cmath>
#include <algorithm>

namespace LumaEdge {

const ColorPreset ColorTemperature::PresetCandle   = {QStringLiteral("Candle"), 2700};
const ColorPreset ColorTemperature::PresetWarm     = {QStringLiteral("Warm"), 3200};
const ColorPreset ColorTemperature::PresetNeutral  = {QStringLiteral("Neutral"), 4000};
const ColorPreset ColorTemperature::PresetCool     = {QStringLiteral("Cool"), 5000};
const ColorPreset ColorTemperature::PresetDaylight = {QStringLiteral("Daylight"), 6500};

int ColorTemperature::clampKelvin(int kelvin)
{
    return std::clamp(kelvin, MinKelvin, MaxKelvin);
}

QColor ColorTemperature::kelvinToRgb(int kelvin)
{
    const auto norm = kelvinToRgbNormalized(kelvin);
    return QColor::fromRgbF(static_cast<float>(norm.x()),
                            static_cast<float>(norm.y()),
                            static_cast<float>(norm.z()),
                            1.0f);
}

QVector3D ColorTemperature::kelvinToRgbNormalized(int kelvin)
{
    const double k = static_cast<double>(clampKelvin(kelvin));
    const double temp = k / 100.0;

    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;

    // Red calculation
    if (temp <= 66.0) {
        red = 255.0;
    } else {
        red = 329.698727446 * std::pow(temp - 60.0, -0.1332047592);
        red = std::clamp(red, 0.0, 255.0);
    }

    // Green calculation
    if (temp <= 66.0) {
        green = 99.4708025861 * std::log(temp) - 161.1195681661;
        green = std::clamp(green, 0.0, 255.0);
    } else {
        green = 288.1221695283 * std::pow(temp - 60.0, -0.0755148492);
        green = std::clamp(green, 0.0, 255.0);
    }

    // Blue calculation
    if (temp >= 66.0) {
        blue = 255.0;
    } else if (temp <= 19.0) {
        blue = 0.0;
    } else {
        blue = 138.5177312231 * std::log(temp - 10.0) - 305.0447927307;
        blue = std::clamp(blue, 0.0, 255.0);
    }

    return QVector3D(static_cast<float>(red / 255.0),
                     static_cast<float>(green / 255.0),
                     static_cast<float>(blue / 255.0));
}

} // namespace LumaEdge
