#include "OverlayWindow.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>

namespace LumaEdge {

OverlayWindow::OverlayWindow(QScreen *targetScreen, LightController *controller, QWindow *parent)
    : QQuickView(parent)
    , m_screen(targetScreen)
    , m_controller(controller)
{
    setupWindow();
    loadQml();

    if (m_controller) {
        connect(m_controller, &LightController::activeChanged, this, &OverlayWindow::onVisibilityChanged);
        connect(m_controller, &LightController::brightnessChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::widthPercentChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::colorTemperatureChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::lightColorChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::edgeSettingsChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::cursorSettingsChanged, this, &OverlayWindow::updateParameters);
        connect(m_controller, &LightController::cursorPosChanged, this, &OverlayWindow::updateParameters);
    }

    if (m_screen) {
        connect(m_screen, &QScreen::geometryChanged, this, &OverlayWindow::onScreenGeometryChanged);
    }

    onVisibilityChanged();
}

OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::setupWindow()
{
    setColor(QColor(0, 0, 0, 0)); // Pure alpha transparency

    setFlags(Qt::FramelessWindowHint |
             Qt::WindowStaysOnTopHint |
             Qt::WindowTransparentForInput |
             Qt::WindowDoesNotAcceptFocus |
             Qt::Tool);

    if (m_screen) {
        setScreen(m_screen);
        setGeometry(m_screen->geometry());
    }

    setResizeMode(QQuickView::SizeRootObjectToView);
}

void OverlayWindow::loadQml()
{
    rootContext()->setContextProperty(QStringLiteral("lightController"), m_controller);
    setSource(QUrl(QStringLiteral("qrc:/qt/qml/LumaEdge/qml/components/EdgePreview.qml")));
    updateParameters();
}

void OverlayWindow::exposeEvent(QExposeEvent *event)
{
    QQuickView::exposeEvent(event);
    if (isExposed()) {
        updateParameters();
    }
}

void OverlayWindow::onScreenGeometryChanged(const QRect &geometry)
{
    setGeometry(geometry);
    updateParameters();
}

void OverlayWindow::onVisibilityChanged()
{
    if (!m_controller) return;

    if (m_controller->isActive()) {
        if (m_screen) {
            setGeometry(m_screen->geometry());
        }
        show();
        raise();
        updateParameters();
    } else {
        hide();
    }
}

void OverlayWindow::updateParameters()
{
    if (!rootObject() || !m_controller) return;

    rootObject()->setProperty("lightColor", m_controller->lightColor());
    rootObject()->setProperty("brightness", m_controller->brightness());
    rootObject()->setProperty("widthPercent", m_controller->widthPercent());

    QVector4D edgeIntensities(
        m_controller->topEdgeEnabled() ? m_controller->topEdgeIntensity() : 0.0f,
        m_controller->bottomEdgeEnabled() ? m_controller->bottomEdgeIntensity() : 0.0f,
        m_controller->leftEdgeEnabled() ? m_controller->leftEdgeIntensity() : 0.0f,
        m_controller->rightEdgeEnabled() ? m_controller->rightEdgeIntensity() : 0.0f
    );
    rootObject()->setProperty("edgeIntensities", edgeIntensities);

    // Map global cursor to local screen coordinate
    QPointF localCursor = m_controller->cursorPos();
    if (m_screen && localCursor.x() >= 0.0f && localCursor.y() >= 0.0f) {
        localCursor.setX(localCursor.x() - m_screen->geometry().x());
        localCursor.setY(localCursor.y() - m_screen->geometry().y());
    }
    rootObject()->setProperty("cursorPos", localCursor);
    rootObject()->setProperty("cursorRadius", m_controller->cursorCutoutRadius());
    rootObject()->setProperty("cursorSoftness", m_controller->cursorCutoutSoftness());
    rootObject()->setProperty("cursorEnabled", m_controller->cursorAwarenessEnabled() ? 1.0f : 0.0f);
}

} // namespace LumaEdge
