# Unit Tests
add_executable(test_colortemperature
    test_colortemperature.cpp
    ../src/renderer/ColorTemperature.cpp
)
target_link_libraries(test_colortemperature PRIVATE Qt6::Core Qt6::Gui Qt6::Test)
target_include_directories(test_colortemperature PRIVATE ../src/renderer)
add_test(NAME ColorTemperatureTest COMMAND test_colortemperature)

add_executable(test_gradient
    test_gradient.cpp
    ../src/renderer/GradientEngine.cpp
    ../src/renderer/ColorTemperature.cpp
)
target_link_libraries(test_gradient PRIVATE Qt6::Core Qt6::Gui Qt6::Test)
target_include_directories(test_gradient PRIVATE ../src/renderer)
add_test(NAME GradientEngineTest COMMAND test_gradient)

add_executable(test_settings
    test_settings.cpp
    ../src/core/SettingsManager.cpp
)
target_link_libraries(test_settings PRIVATE Qt6::Core Qt6::Test)
target_include_directories(test_settings PRIVATE ../src/core)
add_test(NAME SettingsManagerTest COMMAND test_settings)

add_executable(test_platform
    test_platform.cpp
    ../src/platform/PlatformDetector.cpp
)
target_link_libraries(test_platform PRIVATE Qt6::Core Qt6::Gui Qt6::Test)
target_include_directories(test_platform PRIVATE ../src/platform)
add_test(NAME PlatformDetectorTest COMMAND test_platform)

# Ensure tests execute cleanly in headless CI environments
set_tests_properties(
    ColorTemperatureTest
    GradientEngineTest
    SettingsManagerTest
    PlatformDetectorTest
    PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)
