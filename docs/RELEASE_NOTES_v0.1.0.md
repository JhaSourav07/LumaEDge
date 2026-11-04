# LumaEdge v0.1.0 Release Notes

**Release Date:** November 4, 2026  
**Target Environment:** Fedora Linux 44 (Workstation Edition), GNOME Shell 50.4, Wayland/X11  

---

## Highlights

LumaEdge transforms computer monitors into soft, diffused virtual ring lights for Linux desktops, inspired by Apple's macOS Edge Light.

### Core Features
- **GPU-Accelerated Shader Lighting:** Soft cubic-Hermite gradient falloff from screen margins with smooth radial corner blending.
- **Calibrated Color Temperature:** Planckian blackbody locus approximation spanning 2700K (Candle) to 6500K (Daylight) with presets.
- **Zero Disruption Overlay:** Input-transparent overlay windows (`Qt::WindowTransparentForInput`) on Wayland and X11 passing 100% of user clicks and keystrokes to background applications.
- **Independent Edge Controls:** Configure top, bottom, left, and right illumination independently.
- **Pointer Awareness:** Smooth darkened cutout following the mouse cursor on supported sessions.
- **Ambient Low-Light Mode:** Optional local webcam luminance estimation using V4L2 with zero persistent frames and full privacy.
- **Desktop Integration:** Multi-monitor detection, XDG autostart support, and System Tray icon.

### Packaging
- Binary RPM: `build/rpm/lumaedge-0.1.0-1.fc44.x86_64.rpm`
- Source RPM: `build/rpm/lumaedge-0.1.0-1.fc44.src.rpm`
- Includes desktop launcher, scalable vector SVG icon, and AppStream metadata.
