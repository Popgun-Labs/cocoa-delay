cmake_minimum_required(VERSION 3.15)

project(CocoaDelay VERSION 0.0.1)

# Helper to fetch JUCE if not provided
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(juce)

# Create the plugin target
juce_add_plugin(CocoaDelay
    COMPANY_NAME "Tesselode"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Coco
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Cocoa Delay"
    MICROPHONE_PERMISSION_ENABLED TRUE
)

# Generate JuceHeader.h for compatibility
juce_generate_juce_header(CocoaDelay)

# Add source files
target_sources(CocoaDelay
    PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
        Filter.cpp
        Filter.h
        StatefulDrive.cpp
        StatefulDrive.h
        Util.h
        Parameters.h
)

# Link with JUCE modules
target_link_libraries(CocoaDelay
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(CocoaDelay PUBLIC
    JUCE_VST3_CAN_REPLACE_VST2=0
)

