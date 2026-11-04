# LumaEdge — Technical Feasibility Report

**Date:** November 2026  
**Target Environment:** Fedora Linux 44 (Workstation Edition), GNOME Shell 50.4, Wayland Session  
**Hardware:** AMD Radeon 780M Graphics (Mesa 26.1.8, OpenGL 4.6 Core / Vulkan), AMD Phoenix APU  

---

## 1. Executive Summary

This report evaluates the technical feasibility of building **LumaEdge**—a native Linux virtual ring light providing diffused edge illumination—targeting modern Fedora Wayland and X11 desktop environments.

Empirical verification on Fedora 44 with GNOME Shell 50.4 confirms that a transparent, non-blocking fullscreen edge lighting overlay is **fully feasible** using Qt 6.11 with custom GPU shader rendering. Specific Wayland security constraints apply to global pointer tracking, for which clean fallback reporting and multi-platform adapters are established.

---

## 2. Environment Audit Results

| Parameter | Detected Value | Status |
| :--- | :--- | :--- |
| **Operating System** | Fedora Linux 44 (Workstation Edition) | Confirmed primary target |
| **Desktop Environment** | GNOME Shell 50.4 (Mutter) | Active session |
| **Display Protocol** | Wayland (`wayland-0`), Xwayland (`:0`) | Primary: Wayland |
| **Compiler & Tools** | GCC 16.2.1, Clang 22.1.8, CMake 4.3.0, Ninja 1.13.2 | Production C++20 ready |
| **Qt 6 Framework** | Qt 6.11.2 (Core, Gui, Quick, Qml, ShaderTools, WaylandClient) | Fully installed |
| **Graphics Hardware** | AMD Radeon 780M Graphics (Mesa 26.1.8) | Hardware accelerated |
| **Camera Subsystem** | `/dev/video0`, `/dev/video1` with GStreamer 1.28.6 | Available with user ACL |

---

## 3. Wayland Feasibility & Architectural Findings

### 3.1 Transparent Fullscreen Overlay
- **Mechanism:** A frameless, transparent `QQuickView` / `QWindow` configured with:
  - `Qt::FramelessWindowHint`
  - `Qt::WindowStaysOnTopHint`
  - `Qt::WindowTransparentForInput`
  - `Qt::WindowDoesNotAcceptFocus`
  - `Qt::Tool` (or `Qt::BypassWindowManagerHint`)
- **Input Transparency:** When `Qt::WindowTransparentForInput` is set, Qt Wayland sets an empty `wl_region` on the surface via `wl_surface_set_input_region(surface, NULL)`. Under Mutter, this guarantees that 100% of mouse clicks, drags, and wheel events pass directly through to background applications (video conferencing windows, browser, code editor) without interference.
- **Focus Invariance:** `Qt::WindowDoesNotAcceptFocus` guarantees the overlay never steals keyboard focus from active user applications.

### 3.2 Display Scaling & Multi-Monitor
- Mutter supports `wp_fractional_scale_manager_v1` and `zxdg_output_manager_v1`.
- Qt 6 automatically manages High-DPI physical-to-device-pixel scaling via `QScreen::devicePixelRatio()`.
- An independent overlay window is mapped per connected `QScreen`, adapting dynamically to resolution changes and hotplug events.

### 3.3 Pointer Tracking & Mouse Awareness
- **Wayland Security Model:** On native GNOME Wayland, compositors do not allow unprivileged client applications to passively monitor global mouse coordinates when the mouse pointer is not focused on an active input surface. Because the overlay has an empty input region, global mouse coordinates cannot be snooped via standard `QCursor::pos()` without compositor privileges.
- **X11 / Xwayland:** `QCursor::pos()` and `XQueryPointer` function globally.
- **Architecture Solution:** 
  - On X11/Xwayland sessions, full pointer-aware edge cutout is enabled.
  - On GNOME Wayland sessions, the dashboard transparently informs the user that pointer cutout requires session support or Xwayland mode, preventing false expectations while maintaining zero-compromise security.

### 3.4 Camera-Based Ambient Sensing
- Local camera devices (`/dev/video0`) are accessible via GStreamer / V4L2.
- Frames are sampled strictly in local memory to compute average luma ($Y = 0.2126 R + 0.7152 G + 0.0722 B$) and apply smooth hysteresis. Zero frames or metadata are persisted or transmitted.

---

## 4. Conclusion & Milestone Roadmap

All core prerequisites for LumaEdge are satisfied. Development proceeds to Milestone 2 (Proof-of-Concept Overlay Window) and Milestone 3 (GPU Shader Edge Rendering Engine).
