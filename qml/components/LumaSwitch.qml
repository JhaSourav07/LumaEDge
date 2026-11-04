import QtQuick
import "../themes"

Item {
    id: root

    property string title: ""
    property string description: ""
    property bool checked: false

    signal toggled(bool newState)

    implicitWidth: 320
    implicitHeight: description.length > 0 ? 46 : 32

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: {
            if (root.enabled) {
                root.checked = !root.checked
                root.toggled(root.checked)
            }
        }
    }

    Row {
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        spacing: 12

        Column {
            width: parent.width - toggleRect.width - parent.spacing
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Text {
                text: root.title
                color: root.enabled ? LumaTheme.textPrimary : LumaTheme.textMuted
                font.pixelSize: 13
                font.weight: Font.Medium
            }

            Text {
                visible: root.description.length > 0
                text: root.description
                color: LumaTheme.textMuted
                font.pixelSize: 11
                wrapMode: Text.WordWrap
                width: parent.width
            }
        }

        // Toggle Pill
        Rectangle {
            id: toggleRect
            width: 38
            height: 22
            radius: 11
            color: !root.enabled ? LumaTheme.bgElevated : (root.checked ? LumaTheme.accentPrimary : LumaTheme.bgElevated)
            border.color: root.checked ? LumaTheme.accentPrimary : LumaTheme.borderSubtle
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter

            Behavior on color {
                ColorAnimation { duration: 150 }
            }

            Rectangle {
                id: thumb
                width: 16
                height: 16
                radius: 8
                color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
                x: root.checked ? toggleRect.width - width - 3 : 3

                Behavior on x {
                    NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
                }
            }
        }
    }
}
