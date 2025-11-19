# Cocoa Delay (JUCE Port)

This is a port of Cocoa Delay to the JUCE framework.

## Building

### Prerequisites
- CMake 3.15 or higher
- C++ compiler (MSVC, Clang, GCC)
- JUCE (automatically fetched via CMake)

### Steps

1. Navigate to this directory:
   ```bash
   cd source/juce
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project with CMake:
   ```bash
   cmake ..
   ```

4. Build the plugin:
   ```bash
   cmake --build . --config Release
   ```

The built VST3/AU plugin will be in the `build/CocoaDelay_artefacts` directory (structure depends on OS).

