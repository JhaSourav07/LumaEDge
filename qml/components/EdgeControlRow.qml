import QtQuick
import QtQuick.Controls.Basic
import "../themes"

Rectangle {
    id: root

    property string edgeName: ""
    property bool edgeEnabled: true
    property real intensity: 1.0

    signal edgeToggled(bool enabled)
    signal intensityAdjusted(real newIntensity)

    implicitWidth: 320
    implicitHeight: 38
    radius: LumaTheme.radiusSm
    color: LumaTheme.bgElevated
    border.color: LumaTheme.borderSubtle
    border.width: 1

    Row {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 12
        anchors.verticalCenter: parent.verticalCenter

        // Checkbox/Toggle dot
        Rectangle {
            width: 16
            height: 16
            radius: 4
            anchors.verticalCenter: parent.verticalCenter
            color: root.edgeEnabled ? LumaTheme.accentPrimary : "transparent"
            border.color: root.edgeEnabled ? LumaTheme.accentPrimary : LumaTheme.borderHover
            border.width: 1.5

            Text {
                anchors.centerIn: parent
                visible: root.edgeEnabled
                text: "✓"
                font.pixelSize: 10
                font.bold: true
                color: "#ffffff"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.edgeToggled(!root.edgeEnabled)
            }
        }

        // Edge Name
        Text {
            width: 50
            text: root.edgeName
            color: root.edgeEnabled ? LumaTheme.textPrimary : LumaTheme.textMuted
            font.pixelSize: 12
            font.weight: Font.Medium
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.edgeToggled(!root.edgeEnabled)
            }
        }

        // Slider
        Slider {
            id: slider
            width: parent.width - 50 - 16 - 36 - 36
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            enabled: root.edgeEnabled
            opacity: root.edgeEnabled ? 1.0 : 0.35
            from: 0.0
            to: 1.0
            stepSize: 0.05
            value: root.intensity

            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 100
                implicitHeight: 4
                width: slider.availableWidth
                height: 4
                radius: 2
                color: LumaTheme.bgSurface

                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    color: LumaTheme.accentPrimary
                    radius: 2
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 12
                implicitHeight: 12
                radius: 6
                color: "#ffffff"
                border.color: LumaTheme.accentPrimary
                border.width: 1.5
            }

            onMoved: root.intensityAdjusted(slider.value)
        }

        // Percent Text
        Text {
            width: 32
            horizontalAlignment: Text.AlignRight
            text: Math.round(slider.value * 100) + "%"
            color: root.edgeEnabled ? LumaTheme.textSecondary : LumaTheme.textMuted
            font.pixelSize: 11
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
