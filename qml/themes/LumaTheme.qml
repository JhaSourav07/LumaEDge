pragma Singleton
import QtQuick

QtObject {
    // Charcoal and Near-Black Visual Identity
    readonly property color bgBase: "#0c0e12"
    readonly property color bgSurface: "#14171e"
    readonly property color bgElevated: "#1d212a"
    readonly property color bgCard: "#181b23"

    // Borders
    readonly property color borderSubtle: "#262b37"
    readonly property color borderHover: "#373e4f"
    readonly property color borderActive: "#38bdf8"

    // Typography
    readonly property color textPrimary: "#f8fafc"
    readonly property color textSecondary: "#94a3b8"
    readonly property color textMuted: "#64748b"

    // Accents
    readonly property color accentPrimary: "#38bdf8"
    readonly property color accentGlow: "#0ea5e9"
    readonly property color accentSuccess: "#10b981"
    readonly property color accentWarning: "#f59e0b"

    // Spacing & Radius
    readonly property int radiusSm: 6
    readonly property int radiusMd: 10
    readonly property int radiusLg: 16
    readonly property int radiusFull: 9999
}
