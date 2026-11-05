# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.1] - 2026-11-05
### Added
- **Studio Boost Mode**: One-click action in dashboard and system tray setting 100% brightness, 50% width coverage, and 6500K daylight.
- **Extended Screen Coverage**: Light width slider now supports up to 50% of screen dimensions, allowing edge lights to meet seamlessly in the center.

### Changed
- **Solid-Core Shader Profile**: Upgraded GPU fragment shader and CPU gradient engine to maintain a 100% solid opacity outer core (outer 40% of the light band) before feathering, preventing dark desktop bleed-through and significantly increasing emitted lumens.
- **Camera Auto-Dimming Fix**: Synchronized ambient low-light detection with user manual adjustments and added a 70% brightness floor to prevent aggressive dimming feedback loops.

## [0.1.0] - 2026-11-04
### Added
- Environment inspection and feasibility audit on Fedora 44 with GNOME 50 Wayland.
- Foundation architecture: C++20 and Qt 6 application structure.
- Transparent click-through overlay architecture with input transparency and focus protection.
- GPU shader engine for natural edge lighting falloff and corner blending.
- Precise colour temperature algorithm (2700K - 6500K).
- Modern charcoal dark-theme dashboard with live interactive preview.
- Multi-monitor management and dynamic display geometry tracking.
- Optional ambient low-light camera estimation.
- Fedora RPM spec and desktop integration packaging.
