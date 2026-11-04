import QtQuick
import "../themes"

Rectangle {
    id: root

    property string name: ""
    property int kelvin: 4000
    property color swatchColor: "#ffffff"
    property bool selected: false

    signal clicked()

    implicitWidth: 84
    implicitHeight: 48
    radius: LumaTheme.radiusSm

    color: selected ? LumaTheme.bgElevated : (mouseArea.containsMouse ? LumaTheme.bgCard : "transparent")
    border.color: selected ? LumaTheme.accentPrimary : LumaTheme.borderSubtle
    border.width: selected ? 1.5 : 1

    Behavior on color { ColorAnimation { duration: 120 } }
    Behavior on border.color { ColorAnimation { duration: 120 } }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }

    Column {
        anchors.centerIn: parent
        spacing: 3

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: root.swatchColor
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: root.name
                color: root.selected ? LumaTheme.textPrimary : LumaTheme.textSecondary
                font.pixelSize: 11
                font.weight: root.selected ? Font.DemiBold : Font.Medium
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.kelvin + " K"
            color: LumaTheme.textMuted
            font.pixelSize: 10
        }
    }
}
