#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QIcon>
#include <QDebug>

#include "core/ApplicationController.h"
#include "renderer/EdgeLightRenderer.h"

int main(int argc, char *argv[])
{
    // Use QApplication for system tray and standard desktop integration
    QApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("LumaEdge"));
    app.setApplicationDisplayName(QStringLiteral("LumaEdge"));
    app.setOrganizationName(QStringLiteral("LumaEdge"));
    app.setOrganizationDomain(QStringLiteral("lumaedge.org"));
    app.setApplicationVersion(QStringLiteral("0.1.1"));
    app.setWindowIcon(QIcon(QStringLiteral(":/qt/qml/LumaEdge/resources/icons/lumaedge.svg")));

    // Command-line parser
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("LumaEdge — Virtual Lighting for Linux"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption backgroundOption(
        QStringList() << QStringLiteral("b") << QStringLiteral("background"),
        QStringLiteral("Start application in background without opening dashboard.")
    );
    parser.addOption(backgroundOption);

    parser.process(app);

    const bool startInBackground = parser.isSet(backgroundOption);

    // Register custom C++ renderer to QML
    qmlRegisterType<LumaEdge::EdgeLightRenderer>("LumaEdge", 1, 0, "EdgeLightRenderer");

    QQmlApplicationEngine engine;

    LumaEdge::ApplicationController controller;
    if (!controller.initialize(engine, startInBackground)) {
        return -1;
    }

    return app.exec();
}
