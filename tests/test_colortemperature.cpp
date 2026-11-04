#include <QtTest>
#include "ColorTemperature.h"

class TestColorTemperature : public QObject {
    Q_OBJECT

private slots:
    void testClamping();
    void testWarmPresets();
    void testCoolPresets();
    void testNormalizedComponents();
};

void TestColorTemperature::testClamping()
{
    using namespace LumaEdge;
    QCOMPARE(ColorTemperature::clampKelvin(1000), 2700);
    QCOMPARE(ColorTemperature::clampKelvin(2700), 2700);
    QCOMPARE(ColorTemperature::clampKelvin(4000), 4000);
    QCOMPARE(ColorTemperature::clampKelvin(6500), 6500);
    QCOMPARE(ColorTemperature::clampKelvin(10000), 6500);
}

void TestColorTemperature::testWarmPresets()
{
    using namespace LumaEdge;
    QColor candle = ColorTemperature::kelvinToRgb(2700);
    // Candlelight should have high red and lower blue
    QVERIFY(candle.red() > candle.blue());
    QVERIFY(candle.red() >= 240);
    QVERIFY(candle.blue() < 160);

    QColor warm = ColorTemperature::kelvinToRgb(3200);
    QVERIFY(warm.red() > warm.blue());
    QVERIFY(warm.blue() > candle.blue()); // Warm has more blue than Candle
}

void TestColorTemperature::testCoolPresets()
{
    using namespace LumaEdge;
    QColor daylightColor = ColorTemperature::kelvinToRgb(6500);
    // Daylight 6500K is near pure balanced white/slight blue
    QVERIFY(daylightColor.red() > 200);
    QVERIFY(daylightColor.green() > 200);
    QVERIFY(daylightColor.blue() > 200);
}

void TestColorTemperature::testNormalizedComponents()
{
    using namespace LumaEdge;
    for (int k = 2700; k <= 6500; k += 100) {
        QVector3D norm = ColorTemperature::kelvinToRgbNormalized(k);
        QVERIFY(norm.x() >= 0.0f && norm.x() <= 1.0f);
        QVERIFY(norm.y() >= 0.0f && norm.y() <= 1.0f);
        QVERIFY(norm.z() >= 0.0f && norm.z() <= 1.0f);
    }
}

QTEST_GUILESS_MAIN(TestColorTemperature)
#include "test_colortemperature.moc"
