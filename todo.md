PROJECT_NAME: interpolation-calculator
LANGUAGE: C++
STANDARD: C++17
BUILD_SYSTEM: CMake
TARGET_OS: cross-platform (Linux, Windows, macOS)
COMPILER: g++, clang++, MSVC

DEPENDENCIES:
- SFML (graphics, window, system)

PROJECT_STRUCTURE:
- CMakeLists.txt
- src/
  - main.cpp
  - interpolation.cpp
  - interpolation.h
- ui/
  - InputBox.cpp
  - InputBox.h
  - Slider.cpp
  - Slider.h
  - UIColors.h
- assets/
  - font.ttf
- todo.md

PROJECT_STATE:
- SFML integrated and stable
- CMake build works
- Assets load correctly
- Core interpolation (lerp) implemented
- UI system (InputBox, Slider) implemented
- Temperature gradient mode implemented
- Current development focus: mode system

ROADMAP:
FOUNDATION:
- DONE: Project structure, SFML integration, CMake build, assets

CORE_MATH:
- DONE: Linear interpolation (lerp)
- TODO: Optional alternative interpolation functions

BASE_UI:
- DONE: InputBox, Slider, theme, input handling
- TODO: Improve input validation (no invalid floats)

PHYSICS_INTERPRETATION:
- DONE: Temperature gradient mapping
- TODO: Optional x0, x1 input support

MODES_SYSTEM (CURRENT FOCUS):
- TODO: Add Mode enum:
  - LinearInterpolation
  - TemperatureGradient
  - CustomScale
- TODO: Centralize result computation based on mode
- TODO: Make UI semantics mode-dependent
- TODO: Add dynamic labels (A/B/t → T0/T1/x)
- TODO: Display active mode on screen

MODE_SWITCHING:
- TODO: Keyboard switching (Tab / number keys)
- TODO: Visual feedback
- TODO: Prevent invalid transitions

UI_OPTIONS:
- TODO: Button UI class
- TODO: Mode buttons
- TODO: Reset button
- TODO: Help overlay

VISUALIZATION:
- DONE: Interpolation line and point
- TODO: Temperature → color mapping
- TODO: Mode-specific visualization

SCALING:
- TODO: Unit labels
- TODO: Normalized vs real ranges
- TODO: Clamping feedback

ROBUSTNESS:
- TODO: Error handling
- TODO: UI alignment
- TODO: Reduce magic numbers
- TODO: Consistent naming

BUILD_AND_DISTRIBUTION:
- DONE: Stable CMake build
- TODO: Debug / Release presets
- TODO: Automatic asset copying (partial)
- TODO: Install target
- TODO: Prepare GitHub repository

GOALS:
1. Maintain strict separation: core logic (src/) vs UI (ui/).
2. Keep build artifacts confined to build/.
3. Ensure full cross-platform support.
4. Make modes extensible and centralized.
5. Keep UI semantics decoupled from math logic.
6. Avoid hardcoded absolute paths.
7. Keep CMake minimal, portable, and modern.

RULES:
- Never write generated files into source directories.
- Never modify build/ or commit build artifacts.
- Do not hardcode absolute paths.
- Do not modify assets unless explicitly requested.
- Prefer std::vector, std::string, std::optional over raw pointers.
- Avoid global variables unless required for UI state.
- Follow consistent naming:
  - Classes: PascalCase
  - Methods: camelCase
  - Enums: PascalCase
- Use const correctness.
- No using namespace std;

TASK_TYPES_ALLOWED:
- Add new UI components under ui/
- Add new interpolation or physics logic under src/
- Add or refactor modes
- Modify CMakeLists.txt when new files are added
- Refactor for clarity or performance

STYLE_GUIDELINES:
- Header files: #pragma once or include guards
- Source files: include only what is needed
- Minimal coupling between UI and math
- Avoid magic numbers (use constants)

OUTPUT_REQUIREMENTS:
- List all new files.
- List all modified files.
- Provide a short explanation for each change.

FAIL_CONDITIONS:
- Writing files into build/
- Hardcoding absolute paths
- Removing existing features
- Breaking cross-platform compatibility
