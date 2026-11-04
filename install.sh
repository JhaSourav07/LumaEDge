#!/usr/bin/env bash
set -euo pipefail

# LumaEdge One-Command Local / System Installer
# Automatically installs binary, desktop launcher, and high-res icon into the Linux App Drawer.

NAME="lumaedge"
VERSION="0.1.0"
APP_NAME="LumaEdge"

echo "=========================================="
echo "    Installing ${APP_NAME} v${VERSION}"
echo "    Virtual Lighting for Linux"
echo "=========================================="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" 2>/dev/null && pwd || echo "")"

# Determine target directories (user-level by default, zero root required)
BIN_DIR="${HOME}/.local/bin"
DESKTOP_DIR="${HOME}/.local/share/applications"
ICON_DIR="${HOME}/.local/share/icons/hicolor/scalable/apps"
METAINFO_DIR="${HOME}/.local/share/metainfo"

mkdir -p "${BIN_DIR}" "${DESKTOP_DIR}" "${ICON_DIR}" "${METAINFO_DIR}"

# Check if binary is already built locally
SOURCE_BIN=""
if [[ -n "${SCRIPT_DIR}" && -f "${SCRIPT_DIR}/build/bin/${NAME}" ]]; then
    SOURCE_BIN="${SCRIPT_DIR}/build/bin/${NAME}"
elif [[ -n "${SCRIPT_DIR}" && -f "${SCRIPT_DIR}/CMakeLists.txt" ]]; then
    echo "-> Building ${APP_NAME} with CMake & Ninja..."
    cmake -B "${SCRIPT_DIR}/build" -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build "${SCRIPT_DIR}/build"
    SOURCE_BIN="${SCRIPT_DIR}/build/bin/${NAME}"
else
    echo "-> Fetching latest pre-built release binary from GitHub..."
    TMP_DIR="$(mktemp -d)"
    trap 'rm -rf "${TMP_DIR}"' EXIT
    
    RPM_URL="https://github.com/JhaSourav07/ringlight/releases/download/v${VERSION}/lumaedge-${VERSION}-1.fc44.x86_64.rpm"
    curl -fsSL -o "${TMP_DIR}/${NAME}.rpm" "${RPM_URL}"
    
    (cd "${TMP_DIR}" && rpm2cpio "${NAME}.rpm" | cpio -idmv > /dev/null 2>&1)
    SOURCE_BIN="${TMP_DIR}/usr/bin/${NAME}"
    
    # Extract desktop assets
    cp "${TMP_DIR}/usr/share/icons/hicolor/scalable/apps/lumaedge.svg" "${ICON_DIR}/"
    cp "${TMP_DIR}/usr/share/applications/lumaedge.desktop" "${DESKTOP_DIR}/"
    if [[ -f "${TMP_DIR}/usr/share/metainfo/com.lumaedge.LumaEdge.metainfo.xml" ]]; then
        cp "${TMP_DIR}/usr/share/metainfo/com.lumaedge.LumaEdge.metainfo.xml" "${METAINFO_DIR}/"
    fi
fi

# Copy binary
echo "-> Installing binary to ${BIN_DIR}/${NAME}..."
cp -f "${SOURCE_BIN}" "${BIN_DIR}/${NAME}"
chmod 755 "${BIN_DIR}/${NAME}"

# If installing from repository source tree, copy resources
if [[ -n "${SCRIPT_DIR}" && -d "${SCRIPT_DIR}/resources" ]]; then
    echo "-> Installing application icon and metadata..."
    cp -f "${SCRIPT_DIR}/resources/icons/lumaedge.svg" "${ICON_DIR}/"
    cp -f "${SCRIPT_DIR}/resources/lumaedge.desktop" "${DESKTOP_DIR}/"
    cp -f "${SCRIPT_DIR}/resources/com.lumaedge.LumaEdge.metainfo.xml" "${METAINFO_DIR}/"
fi

# Set proper execution path in desktop file
sed -i "s|^Exec=.*|Exec=${BIN_DIR}/${NAME} %u|" "${DESKTOP_DIR}/lumaedge.desktop"
chmod 644 "${DESKTOP_DIR}/lumaedge.desktop"
chmod 644 "${ICON_DIR}/lumaedge.svg"

# Refresh desktop and icon databases for GNOME / KDE App Drawer
echo "-> Updating desktop application databases..."
if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "${DESKTOP_DIR}" 2>/dev/null || true
fi

if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache -f -t "${HOME}/.local/share/icons/hicolor" 2>/dev/null || true
fi

# Check PATH
if [[ ":$PATH:" != *":${BIN_DIR}:"* ]]; then
    echo ""
    echo "NOTE: ${BIN_DIR} is not currently in your PATH."
    echo "Add the following line to your ~/.bashrc or ~/.zshrc:"
    echo "  export PATH=\"${BIN_DIR}:\$PATH\""
fi

echo ""
echo "============================================================"
echo "  Installation Complete!"
echo "  LumaEdge is now installed in your Linux App Drawer."
echo "  Launch it from your Applications menu or run:"
echo "    ${BIN_DIR}/${NAME}"
echo "============================================================"
