# Contributing to LumaEdge

Thank you for your interest in contributing to LumaEdge!

## Development Philosophy
- Modern C++20 and Qt 6.
- Wayland-native design with reliable X11 compatibility.
- Privacy-first: strictly zero cloud telemetry, local hardware processing only.
- Lean code: minimal overhead, GPU-accelerated rendering, zero CPU frame loops.

## Building from Source

### Prerequisites
On Fedora Linux:
```bash
sudo dnf install -y \
    gcc-c++ cmake ninja-build \
    qt6-qtbase-devel \
    qt6-qtdeclarative-devel \
    qt6-qtshadertools-devel \
    gstreamer1-devel \
    gstreamer1-plugins-base-devel \
    wayland-devel
```

### Build Instructions
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running Tests
```bash
ctest --test-dir build --output-on-failure
```

## Pull Request Guidelines
- Follow Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`, `refactor:`, `build:`).
- Ensure all automated unit and integration tests pass.
- Do not introduce Python runtime, Electron, or web wrapper dependencies.
