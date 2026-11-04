#pragma once

#include <QObject>
#include <QVariantList>
#include <QScreen>
#include "../core/LightController.h"

namespace LumaEdge {

class DisplayManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList screens READ screens NOTIFY screensChanged)
    Q_PROPERTY(int screenCount READ screenCount NOTIFY screensChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectionChanged)
    Q_PROPERTY(QString primaryScreenName READ primaryScreenName NOTIFY screensChanged)

public:
    explicit DisplayManager(LightController *controller, QObject *parent = nullptr);

    QVariantList screens() const { return m_screenList; }
    int screenCount() const { return static_cast<int>(m_screenList.size()); }
    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString primaryScreenName() const;

public slots:
    void refreshScreens();

signals:
    void screensChanged();
    void selectionChanged();

private slots:
    void onScreenAdded(QScreen *screen);
    void onScreenRemoved(QScreen *screen);

private:
    LightController *m_controller{nullptr};
    QVariantList m_screenList;
};

} // namespace LumaEdge
