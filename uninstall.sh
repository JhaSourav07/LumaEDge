#!/usr/bin/env bash
set -euo pipefail

# LumaEdge Uninstaller

NAME="lumaedge"

echo "Uninstalling LumaEdge..."

rm -f "${HOME}/.local/bin/${NAME}"
rm -f "${HOME}/.local/share/applications/lumaedge.desktop"
rm -f "${HOME}/.local/share/icons/hicolor/scalable/apps/lumaedge.svg"
rm -f "${HOME}/.local/share/metainfo/com.lumaedge.LumaEdge.metainfo.xml"
rm -f "${HOME}/.config/autostart/lumaedge.desktop"

if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "${HOME}/.local/share/applications" 2>/dev/null || true
fi

if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache -f -t "${HOME}/.local/share/icons/hicolor" 2>/dev/null || true
fi

echo "LumaEdge has been completely uninstalled from your user environment."
