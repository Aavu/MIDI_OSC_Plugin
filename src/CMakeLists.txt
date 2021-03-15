juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME Techitunes
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Aavu
        PLUGIN_CODE Osc0
        FORMATS AU Standalone
        PRODUCT_NAME "MIDI OSC Plugin")

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        RobotComponent.cpp
        Robot.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

juce_add_binary_data(${PROJECT_NAME}-Data SOURCES ${CMAKE_SOURCE_DIR}/assets/power-off-solid.svg)
#juce_add_bundle_resources_directory(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/assets)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_NAME}-Data
        juce::juce_audio_utils
        juce::juce_osc
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)