#include <QtTest>
#include "SettingsManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>

class TestSettings : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testDefaultsAndMutations();
    void cleanupTestCase();
};

void TestSettings::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void TestSettings::testDefaultsAndMutations()
{
    using namespace LumaEdge;
    SettingsManager mgr;

    // Verify initial sensible defaults
    QCOMPARE(mgr.isActive(), false);
    QCOMPARE(mgr.brightness(), 0.8f);
    QCOMPARE(mgr.widthPercent(), 0.15f);
    QCOMPARE(mgr.colorTemperature(), 4000);
    QCOMPARE(mgr.topEdgeEnabled(), true);

    // Mutate
    mgr.setActive(true);
    mgr.setBrightness(0.95f);
    mgr.setColorTemperature(5000);
    mgr.setWidthPercent(0.20f);
    mgr.save();

    // Verify values stick in a second manager instance
    SettingsManager mgr2;
    QCOMPARE(mgr2.isActive(), true);
    QVERIFY(std::abs(mgr2.brightness() - 0.95f) < 0.01f);
    QCOMPARE(mgr2.colorTemperature(), 5000);
    QVERIFY(std::abs(mgr2.widthPercent() - 0.20f) < 0.01f);
}

void TestSettings::cleanupTestCase()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QFile::remove(configDir + QStringLiteral("/lumaedge.conf"));
}

QTEST_MAIN(TestSettings)
#include "test_settings.moc"
