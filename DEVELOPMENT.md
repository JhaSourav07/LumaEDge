# LumaEdge Development Status & Tracker

## 1. Project Overview
- **Product:** LumaEdge (Virtual Ring Light for Linux)
- **Architecture:** C++20, Qt 6.11, QML, Custom GPU Shaders, Wayland & X11 integration
- **Target OS:** Fedora Linux 44 Workstation (GNOME Shell, Wayland/X11)

---

## 2. Milestone Roadmap & Progress

| Milestone | Description | Status |
| :--- | :--- | :--- |
| **Milestone 1** | Environment inspection & Wayland feasibility audit | **Completed** |
| **Milestone 2** | Proof of concept: transparent click-through overlay window | **In Progress** |
| **Milestone 3** | Dedicated GPU shader edge-lighting rendering engine | Scheduled |
| **Milestone 4** | Premium QML dashboard (Charcoal theme, live preview) | Scheduled |
| **Milestone 5** | Intelligent lighting (Pointer cutout & optional camera sensor) | Scheduled |
| **Milestone 6** | Linux desktop integration (Multi-monitor, tray, autostart) | Scheduled |
| **Milestone 7** | Quality assurance, test automation, RPM packaging, release | Scheduled |

---

## 3. Environment Summary
- **OS:** Fedora Linux 44 (Workstation Edition)
- **Compositor:** GNOME Shell 50.4 (Mutter)
- **Primary Display:** eDP-1 (1920x1080)
- **Graphics:** AMD Radeon 780M (Mesa 26.1.8)
- **Compiler:** GCC 16.2.1 / Clang 22.1.8, C++20
- **Qt Version:** Qt 6.11.2 (Core, Gui, Quick, Qml, ShaderTools, WaylandClient)

---

## 4. Current Work & Next Steps
- Implementing project build configuration (`CMakeLists.txt`).
- Constructing C++ core architecture: `PlatformDetector`, `OverlayWindow`, `OverlayManager`.
- Validating click-through and non-focus stealing overlay.
