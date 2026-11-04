import QtQuick
import QtQuick.Controls.Basic
import "../themes"

Item {
    id: root

    property string label: ""
    property string valueText: ""
    property alias from: slider.from
    property alias to: slider.to
    property alias stepSize: slider.stepSize
    property alias value: slider.value
    property alias pressed: slider.pressed
    property color activeColor: LumaTheme.accentPrimary

    signal moved()

    implicitWidth: 320
    implicitHeight: 52

    Column {
        anchors.fill: parent
        spacing: 6

        // Label and Value Row
        Row {
            width: parent.width

            Text {
                text: root.label
                color: LumaTheme.textSecondary
                font.pixelSize: 13
                font.weight: Font.Medium
            }

            Item {
                width: parent.width - childrenRect.width
                height: 1
            }

            Text {
                text: root.valueText
                color: LumaTheme.textPrimary
                font.pixelSize: 13
                font.weight: Font.DemiBold
            }
        }

        // Custom Slider Control
        Slider {
            id: slider
            width: parent.width
            height: 24
            padding: 0

            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: slider.availableWidth
                height: implicitHeight
                radius: 3
                color: LumaTheme.bgElevated
                border.color: LumaTheme.borderSubtle
                border.width: 1

                // Active progress bar
                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    color: root.activeColor
                    radius: 3
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: slider.pressed ? "#ffffff" : LumaTheme.textPrimary
                border.color: root.activeColor
                border.width: 2

                Behavior on scale {
                    NumberAnimation { duration: 100 }
                }
                scale: slider.hovered || slider.pressed ? 1.15 : 1.0
            }

            onMoved: root.moved()
        }
    }
}
