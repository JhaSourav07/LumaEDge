#include <QtTest>
#include "PlatformDetector.h"

class TestPlatform : public QObject {
    Q_OBJECT

private slots:
    void testDetection();
};

void TestPlatform::testDetection()
{
    using namespace LumaEdge;
    PlatformDetector detector;

    // Verify detector reports valid non-empty session and desktop names
    QVERIFY(!detector.sessionTypeName().isEmpty());
    QVERIFY(!detector.desktopEnvironmentName().isEmpty());
    QVERIFY(!detector.cursorTrackingStatus().isEmpty());
}

QTEST_MAIN(TestPlatform)
#include "test_platform.moc"
