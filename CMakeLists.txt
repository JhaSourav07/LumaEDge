cmake_minimum_required(VERSION 3.20)
project(LumaEdge VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

# Qt 6 Packages
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Widgets
    Test
)

qt_policy(SET QTP0004 NEW)
qt_standard_project_setup(REQUIRES 6.5)

# Optional GStreamer for camera ambient sensing
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(GST gstreamer-1.0 gstreamer-app-1.0)
    pkg_check_modules(X11 x11 xfixes)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/overlay
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cursor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/display
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system
)

# Source files
set(SOURCES
    src/main.cpp
    src/core/ApplicationController.cpp
    src/core/SettingsManager.cpp
    src/core/LightController.cpp
    src/renderer/ColorTemperature.cpp
    src/renderer/GradientEngine.cpp
    src/renderer/EdgeLightRenderer.cpp
    src/overlay/OverlayWindow.cpp
    src/overlay/OverlayManager.cpp
    src/cursor/CursorTracker.cpp
    src/display/DisplayManager.cpp
    src/camera/CameraManager.cpp
    src/platform/PlatformDetector.cpp
    src/platform/WaylandIntegration.cpp
    src/platform/X11Integration.cpp
    src/system/SystemTray.cpp
    src/system/StartupManager.cpp
)

set(HEADERS
    src/core/ApplicationController.h
    src/core/SettingsManager.h
    src/core/LightController.h
    src/renderer/ColorTemperature.h
    src/renderer/GradientEngine.h
    src/renderer/EdgeLightRenderer.h
    src/overlay/OverlayWindow.h
    src/overlay/OverlayManager.h
    src/cursor/CursorTracker.h
    src/display/DisplayManager.h
    src/camera/CameraManager.h
    src/platform/PlatformDetector.h
    src/platform/WaylandIntegration.h
    src/platform/X11Integration.h
    src/system/SystemTray.h
    src/system/StartupManager.h
)

# QML & Resources
qt_add_executable(lumaedge
    ${SOURCES}
    ${HEADERS}
    resources/resources.qrc
)

qt_add_qml_module(lumaedge
    URI LumaEdge
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/LumaSlider.qml
        qml/components/LumaSwitch.qml
        qml/components/LumaPresetButton.qml
        qml/components/EdgePreview.qml
        qml/components/EdgeControlRow.qml
        qml/themes/LumaTheme.qml
)

target_link_libraries(lumaedge PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)

if(GST_FOUND)
    target_compile_definitions(lumaedge PRIVATE HAVE_GSTREAMER=1)
    target_include_directories(lumaedge PRIVATE ${GST_INCLUDE_DIRS})
    target_link_libraries(lumaedge PRIVATE ${GST_LIBRARIES})
endif()

if(X11_FOUND)
    target_compile_definitions(lumaedge PRIVATE HAVE_X11=1)
    target_include_directories(lumaedge PRIVATE ${X11_INCLUDE_DIRS})
    target_link_libraries(lumaedge PRIVATE ${X11_LIBRARIES})
endif()

set_target_properties(lumaedge PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Automated Unit Tests
enable_testing()
add_subdirectory(tests)

# Installation targets
include(GNUInstallDirs)
install(TARGETS lumaedge RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES resources/lumaedge.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES resources/icons/lumaedge.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
install(FILES resources/com.lumaedge.LumaEdge.metainfo.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
