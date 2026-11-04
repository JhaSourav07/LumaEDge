#include <QtTest>
#include "GradientEngine.h"

class TestGradientEngine : public QObject {
    Q_OBJECT

private slots:
    void testSmoothstep();
    void testCenterAlphaZero();
    void testEdgeAlphaHigh();
    void testCursorCutout();
};

void TestGradientEngine::testSmoothstep()
{
    using namespace LumaEdge;
    QCOMPARE(GradientEngine::smoothstep(0.0f, 1.0f, 0.0f), 0.0f);
    QCOMPARE(GradientEngine::smoothstep(0.0f, 1.0f, 0.5f), 0.5f);
    QCOMPARE(GradientEngine::smoothstep(0.0f, 1.0f, 1.0f), 1.0f);
    QCOMPARE(GradientEngine::smoothstep(0.0f, 1.0f, -0.5f), 0.0f);
    QCOMPARE(GradientEngine::smoothstep(0.0f, 1.0f, 1.5f), 1.0f);
}

void TestGradientEngine::testCenterAlphaZero()
{
    using namespace LumaEdge;
    LightParameters params;
    params.brightness = 1.0f;
    params.widthPercent = 0.15f; // 15% margin on 1000px = 150px
    QSizeF screen(1000, 1000);

    // Center of the screen (500, 500) is far away from edges -> alpha must be 0
    float centerAlpha = GradientEngine::calculateAlphaAt(500.0f, 500.0f, screen, params);
    QCOMPARE(centerAlpha, 0.0f);
}

void TestGradientEngine::testEdgeAlphaHigh()
{
    using namespace LumaEdge;
    LightParameters params;
    params.brightness = 1.0f;
    params.widthPercent = 0.15f; // 150px
    QSizeF screen(1000, 1000);

    // Top edge at y = 5px should have high alpha (> 0.8)
    float edgeAlpha = GradientEngine::calculateAlphaAt(500.0f, 5.0f, screen, params);
    QVERIFY(edgeAlpha > 0.8f);

    // Disabled edge should yield 0
    params.edges.topEnabled = false;
    float disabledAlpha = GradientEngine::calculateAlphaAt(500.0f, 5.0f, screen, params);
    QCOMPARE(disabledAlpha, 0.0f);
}

void TestGradientEngine::testCursorCutout()
{
    using namespace LumaEdge;
    LightParameters params;
    params.brightness = 1.0f;
    params.widthPercent = 0.15f;
    QSizeF screen(1000, 1000);

    // Without cursor awareness
    float baseAlpha = GradientEngine::calculateAlphaAt(500.0f, 5.0f, screen, params);
    QVERIFY(baseAlpha > 0.8f);

    // Enable cursor cutout right at (500, 5)
    params.cursorAwareness = true;
    params.cursorPos = QPointF(500.0f, 5.0f);
    params.cursorCutoutRadius = 100.0f;
    params.cursorCutoutSoftness = 0.5f;

    float cutoutAlpha = GradientEngine::calculateAlphaAt(500.0f, 5.0f, screen, params);
    // At cursor center, illumination should be dimmed to 0
    QCOMPARE(cutoutAlpha, 0.0f);
}

QTEST_MAIN(TestGradientEngine)
#include "test_gradient.moc"
