import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "themes"
import "components"

ApplicationWindow {
    id: window
    title: "LumaEdge — Virtual Lighting for Linux"
    width: 520
    height: 780
    minimumWidth: 480
    minimumHeight: 680
    visible: true
    color: LumaTheme.bgBase

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: 20
        clip: true
        contentWidth: availableWidth

        Column {
            width: parent.width
            spacing: 20

            // -------------------------------------------------------------
            // 1. HEADER SECTION
            // -------------------------------------------------------------
            RowLayout {
                width: parent.width

                Row {
                    spacing: 12
                    Layout.alignment: Qt.AlignVCenter

                    Rectangle {
                        width: 36
                        height: 36
                        radius: 8
                        color: LumaTheme.bgElevated
                        border.color: LumaTheme.borderSubtle
                        border.width: 1

                        Image {
                            anchors.fill: parent
                            anchors.margins: 4
                            source: "qrc:/icons/lumaedge.svg"
                            sourceSize: Qt.size(32, 32)
                        }
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2

                        Text {
                            text: "LumaEdge"
                            color: LumaTheme.textPrimary
                            font.pixelSize: 17
                            font.weight: Font.Bold
                            font.letterSpacing: -0.3
                        }

                        Text {
                            text: "Virtual lighting for Linux"
                            color: LumaTheme.textMuted
                            font.pixelSize: 11
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Active status pill
                Rectangle {
                    radius: LumaTheme.radiusFull
                    height: 26
                    width: statusRow.width + 20
                    color: lightController.active ? Qt.rgba(16/255, 185/255, 129/255, 0.15) : LumaTheme.bgElevated
                    border.color: lightController.active ? Qt.rgba(16/255, 185/255, 129/255, 0.4) : LumaTheme.borderSubtle
                    border.width: 1

                    Row {
                        id: statusRow
                        anchors.centerIn: parent
                        spacing: 6

                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: lightController.active ? LumaTheme.accentSuccess : LumaTheme.textMuted
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: lightController.active ? "LIGHTING ACTIVE" : "STANDBY"
                            color: lightController.active ? LumaTheme.accentSuccess : LumaTheme.textMuted
                            font.pixelSize: 10
                            font.weight: Font.Bold
                            font.letterSpacing: 0.5
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }

            // -------------------------------------------------------------
            // 2. HERO POWER TOGGLE & INTERACTIVE LIVE PREVIEW
            // -------------------------------------------------------------
            Rectangle {
                width: parent.width
                height: 210
                radius: LumaTheme.radiusLg
                color: LumaTheme.bgSurface
                border.color: lightController.active ? Qt.rgba(56/255, 189/255, 248/255, 0.3) : LumaTheme.borderSubtle
                border.width: 1

                // Outer ambient glow effect when active
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: -1
                    radius: LumaTheme.radiusLg
                    color: "transparent"
                    border.color: lightController.lightColor
                    border.width: 2
                    opacity: lightController.active ? (lightController.brightness * 0.4) : 0.0
                    visible: opacity > 0.0

                    Behavior on opacity { NumberAnimation { duration: 250 } }
                }

                Column {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12

                    // Mini Screen Frame with Real-Time Shader Edge Illumination
                    Rectangle {
                        id: previewFrame
                        width: parent.width
                        height: 110
                        radius: LumaTheme.radiusMd
                        color: "#07080a"
                        border.color: LumaTheme.borderSubtle
                        border.width: 1
                        clip: true

                        EdgePreview {
                            anchors.fill: parent
                            lightColor: lightController.lightColor
                            brightness: lightController.active ? lightController.brightness : 0.0
                            widthPercent: lightController.widthPercent
                            edgeIntensities: Qt.vector4d(
                                lightController.topEdgeEnabled ? lightController.topEdgeIntensity : 0.0,
                                lightController.bottomEdgeEnabled ? lightController.bottomEdgeIntensity : 0.0,
                                lightController.leftEdgeEnabled ? lightController.leftEdgeIntensity : 0.0,
                                lightController.rightEdgeEnabled ? lightController.rightEdgeIntensity : 0.0
                            )
                            cursorPos: previewMouseArea.containsMouse ? Qt.point(previewMouseArea.mouseX, previewMouseArea.mouseY) : Qt.point(-1, -1)
                            cursorRadius: 40
                            cursorSoftness: 0.5
                            cursorEnabled: previewMouseArea.containsMouse ? 1.0 : 0.0

                            Behavior on brightness { NumberAnimation { duration: 200 } }
                        }

                        // Central preview watermark
                        Column {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: lightController.active ? "LIVE PREVIEW" : "LIGHTING DISABLED"
                                color: lightController.active ? "#ffffff" : LumaTheme.textMuted
                                font.pixelSize: 11
                                font.weight: Font.Bold
                                font.letterSpacing: 1.0
                                opacity: 0.8
                            }

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: lightController.colorTemperature + " K • " + Math.round(lightController.brightness * 100) + "% Intensity"
                                color: LumaTheme.textSecondary
                                font.pixelSize: 10
                                opacity: 0.7
                            }
                        }

                        MouseArea {
                            id: previewMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }

                    // Master Power Button
                    Rectangle {
                        id: powerBtn
                        width: parent.width
                        height: 48
                        radius: LumaTheme.radiusMd
                        color: lightController.active ? LumaTheme.accentPrimary : LumaTheme.bgElevated
                        border.color: lightController.active ? LumaTheme.accentPrimary : LumaTheme.borderHover
                        border.width: 1

                        Behavior on color { ColorAnimation { duration: 150 } }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: lightController.togglePower()
                        }

                        Row {
                            anchors.centerIn: parent
                            spacing: 8

                            Text {
                                text: "⏻"
                                color: lightController.active ? "#090d16" : LumaTheme.textPrimary
                                font.pixelSize: 16
                                font.bold: true
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Text {
                                text: lightController.active ? "Disable Virtual Ring Light" : "Enable Virtual Ring Light"
                                color: lightController.active ? "#090d16" : LumaTheme.textPrimary
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }

            // -------------------------------------------------------------
            // 3. CORE LIGHTING CONTROLS
            // -------------------------------------------------------------
            Rectangle {
                width: parent.width
                height: coreControlsCol.height + 32
                radius: LumaTheme.radiusLg
                color: LumaTheme.bgSurface
                border.color: LumaTheme.borderSubtle
                border.width: 1

                Column {
                    id: coreControlsCol
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 16
                    spacing: 16

                    // Brightness Slider
                    LumaSlider {
                        width: parent.width
                        label: "Brightness"
                        valueText: Math.round(value * 100) + "%"
                        from: 0.05
                        to: 1.0
                        stepSize: 0.01
                        value: lightController.brightness
                        onMoved: lightController.brightness = value
                    }

                    // Light Width Slider
                    LumaSlider {
                        width: parent.width
                        label: "Light Width"
                        valueText: Math.round(value * 100) + "% of screen"
                        from: 0.05
                        to: 0.35
                        stepSize: 0.01
                        value: lightController.widthPercent
                        onMoved: lightController.widthPercent = value
                    }

                    // Colour Temperature Slider
                    LumaSlider {
                        width: parent.width
                        label: "Colour Temperature"
                        valueText: Math.round(value) + " K"
                        from: 2700
                        to: 6500
                        stepSize: 50
                        value: lightController.colorTemperature
                        activeColor: lightController.lightColor
                        onMoved: lightController.colorTemperature = Math.round(value)
                    }

                    // Presets Row
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Calibrated Presets"
                            color: LumaTheme.textMuted
                            font.pixelSize: 11
                            font.weight: Font.Medium
                        }

                        RowLayout {
                            width: parent.width
                            spacing: 8

                            LumaPresetButton {
                                Layout.fillWidth: true
                                name: "Candle"
                                kelvin: 2700
                                swatchColor: "#ff9329"
                                selected: lightController.colorTemperature === 2700
                                onClicked: lightController.setPreset(2700)
                            }

                            LumaPresetButton {
                                Layout.fillWidth: true
                                name: "Warm"
                                kelvin: 3200
                                swatchColor: "#ffb46b"
                                selected: lightController.colorTemperature === 3200
                                onClicked: lightController.setPreset(3200)
                            }

                            LumaPresetButton {
                                Layout.fillWidth: true
                                name: "Neutral"
                                kelvin: 4000
                                swatchColor: "#ffe4ce"
                                selected: lightController.colorTemperature === 4000
                                onClicked: lightController.setPreset(4000)
                            }

                            LumaPresetButton {
                                Layout.fillWidth: true
                                name: "Cool"
                                kelvin: 5000
                                swatchColor: "#ffffff"
                                selected: lightController.colorTemperature === 5000
                                onClicked: lightController.setPreset(5000)
                            }

                            LumaPresetButton {
                                Layout.fillWidth: true
                                name: "Daylight"
                                kelvin: 6500
                                swatchColor: "#dbeafe"
                                selected: lightController.colorTemperature === 6500
                                onClicked: lightController.setPreset(6500)
                            }
                        }
                    }
                }
            }

            // -------------------------------------------------------------
            // 4. INDEPENDENT EDGES COLLAPSIBLE
            // -------------------------------------------------------------
            Rectangle {
                width: parent.width
                height: edgesContentCol.height + 32
                radius: LumaTheme.radiusLg
                color: LumaTheme.bgSurface
                border.color: LumaTheme.borderSubtle
                border.width: 1

                Column {
                    id: edgesContentCol
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 16
                    spacing: 12

                    Row {
                        width: parent.width
                        Text {
                            text: "Independent Edge Controls"
                            color: LumaTheme.textPrimary
                            font.pixelSize: 13
                            font.weight: Font.DemiBold
                        }
                    }

                    EdgeControlRow {
                        width: parent.width
                        edgeName: "Top"
                        edgeEnabled: lightController.topEdgeEnabled
                        intensity: lightController.topEdgeIntensity
                        onEnabledToggled: (e) => lightController.topEdgeEnabled = e
                        onIntensityChanged: (i) => lightController.topEdgeIntensity = i
                    }

                    EdgeControlRow {
                        width: parent.width
                        edgeName: "Bottom"
                        edgeEnabled: lightController.bottomEdgeEnabled
                        intensity: lightController.bottomEdgeIntensity
                        onEnabledToggled: (e) => lightController.bottomEdgeEnabled = e
                        onIntensityChanged: (i) => lightController.bottomEdgeIntensity = i
                    }

                    EdgeControlRow {
                        width: parent.width
                        edgeName: "Left"
                        edgeEnabled: lightController.leftEdgeEnabled
                        intensity: lightController.leftEdgeIntensity
                        onEnabledToggled: (e) => lightController.leftEdgeEnabled = e
                        onIntensityChanged: (i) => lightController.leftEdgeIntensity = i
                    }

                    EdgeControlRow {
                        width: parent.width
                        edgeName: "Right"
                        edgeEnabled: lightController.rightEdgeEnabled
                        intensity: lightController.rightEdgeIntensity
                        onEnabledToggled: (e) => lightController.rightEdgeEnabled = e
                        onIntensityChanged: (i) => lightController.rightEdgeIntensity = i
                    }
                }
            }

            // -------------------------------------------------------------
            // 5. INTELLIGENT LIGHTING & DESKTOP INTEGRATION
            // -------------------------------------------------------------
            Rectangle {
                width: parent.width
                height: advancedCol.height + 32
                radius: LumaTheme.radiusLg
                color: LumaTheme.bgSurface
                border.color: LumaTheme.borderSubtle
                border.width: 1

                Column {
                    id: advancedCol
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 16
                    spacing: 16

                    Text {
                        text: "Intelligent Lighting & Display"
                        color: LumaTheme.textPrimary
                        font.pixelSize: 13
                        font.weight: Font.DemiBold
                    }

                    // Mouse-Aware Pointer Cutout
                    Column {
                        width: parent.width
                        spacing: 8

                        LumaSwitch {
                            width: parent.width
                            title: "Pointer-Aware Edge Cutout"
                            description: platformDetector.supportsGlobalCursorTracking
                                ? "Dims lighting smoothly around the cursor near screen edges."
                                : "Session note: " + platformDetector.cursorTrackingStatus
                            checked: lightController.cursorAwarenessEnabled
                            enabled: platformDetector.supportsGlobalCursorTracking
                            onToggled: (val) => lightController.cursorAwarenessEnabled = val
                        }

                        // Cutout Radius & Softness (visible if enabled)
                        Column {
                            width: parent.width
                            spacing: 10
                            visible: lightController.cursorAwarenessEnabled && platformDetector.supportsGlobalCursorTracking

                            LumaSlider {
                                width: parent.width
                                label: "Cutout Radius"
                                valueText: Math.round(value) + " px"
                                from: 60
                                to: 280
                                stepSize: 5
                                value: lightController.cursorCutoutRadius
                                onMoved: lightController.cursorCutoutRadius = value
                            }
                        }
                    }

                    Rectangle { width: parent.width; height: 1; color: LumaTheme.borderSubtle }

                    // Automatic Ambient Low-Light Mode
                    Column {
                        width: parent.width
                        spacing: 8

                        LumaSwitch {
                            width: parent.width
                            title: "Automatic Low-Light Mode"
                            description: "Estimates ambient brightness locally via camera to auto-adjust light."
                            checked: lightController.autoLowLightEnabled
                            onToggled: (val) => lightController.autoLowLightEnabled = val
                        }

                        // Camera controls & status (visible if enabled)
                        Column {
                            width: parent.width
                            spacing: 8
                            visible: lightController.autoLowLightEnabled

                            Row {
                                spacing: 8
                                Text {
                                    text: "Camera Status:"
                                    color: LumaTheme.textMuted
                                    font.pixelSize: 11
                                }
                                Text {
                                    text: cameraManager.statusText
                                    color: cameraManager.isCameraActive ? LumaTheme.accentSuccess : LumaTheme.textSecondary
                                    font.pixelSize: 11
                                    font.weight: Font.Medium
                                }
                            }

                            LumaSlider {
                                width: parent.width
                                label: "Auto Sensitivity"
                                valueText: Math.round(value * 100) + "%"
                                from: 0.1
                                to: 1.0
                                stepSize: 0.05
                                value: lightController.autoLowLightSensitivity
                                onMoved: lightController.autoLowLightSensitivity = value
                            }
                        }
                    }

                    Rectangle { width: parent.width; height: 1; color: LumaTheme.borderSubtle }

                    // Display Selection
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Active Displays"
                            color: LumaTheme.textSecondary
                            font.pixelSize: 12
                            font.weight: Font.Medium
                        }

                        RowLayout {
                            width: parent.width
                            spacing: 8

                            Repeater {
                                model: displayManager.screens

                                Rectangle {
                                    Layout.fillWidth: true
                                    height: 38
                                    radius: LumaTheme.radiusSm
                                    color: (displayManager.selectedIndex === -1 || displayManager.selectedIndex === modelData.index)
                                        ? LumaTheme.bgElevated : "transparent"
                                    border.color: (displayManager.selectedIndex === -1 || displayManager.selectedIndex === modelData.index)
                                        ? LumaTheme.accentPrimary : LumaTheme.borderSubtle
                                    border.width: 1

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            if (displayManager.selectedIndex === modelData.index) {
                                                displayManager.selectedIndex = -1 // All displays
                                            } else {
                                                displayManager.selectedIndex = modelData.index
                                            }
                                        }
                                    }

                                    Row {
                                        anchors.centerIn: parent
                                        spacing: 6

                                        Text {
                                            text: "🖥 " + modelData.name
                                            color: LumaTheme.textPrimary
                                            font.pixelSize: 11
                                            font.weight: Font.Medium
                                        }

                                        Text {
                                            text: modelData.width + "x" + modelData.height
                                            color: LumaTheme.textMuted
                                            font.pixelSize: 10
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Rectangle { width: parent.width; height: 1; color: LumaTheme.borderSubtle }

                    // Autostart toggle
                    LumaSwitch {
                        width: parent.width
                        title: "Start on Desktop Login"
                        description: "Launch LumaEdge in the background on startup."
                        checked: startupManager.isAutostartEnabled
                        onToggled: (val) => startupManager.setAutostartEnabled(val)
                    }
                }
            }

            // -------------------------------------------------------------
            // 6. FOOTER SECTION
            // -------------------------------------------------------------
            RowLayout {
                width: parent.width

                Text {
                    text: "LumaEdge v0.1.0 • " + platformDetector.sessionTypeName + " (" + platformDetector.desktopEnvironmentName + ")"
                    color: LumaTheme.textMuted
                    font.pixelSize: 11
                    Layout.alignment: Qt.AlignVCenter
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    width: 70
                    height: 28
                    radius: LumaTheme.radiusSm
                    color: LumaTheme.bgElevated
                    border.color: LumaTheme.borderSubtle
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: appController.quitApplication()
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "Quit"
                        color: LumaTheme.textSecondary
                        font.pixelSize: 11
                        font.weight: Font.Medium
                    }
                }
            }

            Item { width: 1; height: 10 }
        }
    }
}
