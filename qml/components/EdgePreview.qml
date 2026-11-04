import QtQuick

Item {
    id: root

    property color lightColor: Qt.rgba(1, 1, 1, 1)
    property real brightness: 0.8
    property real widthPercent: 0.15
    property vector4d edgeIntensities: Qt.vector4d(1, 1, 1, 1)
    property point cursorPos: Qt.point(-1, -1)
    property real cursorRadius: 140
    property real cursorSoftness: 0.4
    property real cursorEnabled: 0.0

    ShaderEffect {
        id: shader
        anchors.fill: parent
        visible: root.width > 0 && root.height > 0

        property color lightColor: root.lightColor
        property vector2d resolution: Qt.vector2d(Math.max(1, root.width), Math.max(1, root.height))
        property vector4d edgeIntensities: root.edgeIntensities
        property vector2d cursorPos: Qt.vector2d(root.cursorPos.x, root.cursorPos.y)
        property real brightness: root.brightness
        property real widthPercent: root.widthPercent
        property real cursorRadius: root.cursorRadius
        property real cursorSoftness: root.cursorSoftness
        property real cursorEnabled: root.cursorEnabled

        vertexShader: "qrc:/shaders/edge_light.vert.qsb"
        fragmentShader: "qrc:/shaders/edge_light.frag.qsb"
    }
}
