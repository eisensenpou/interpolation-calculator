# Interpolation Calculator

A sophisticated interpolation calculator built with C++ and SFML that supports multiple interpolation modes with real-time visualization and physics-based interpretations.

## What It Does

The Interpolation Calculator provides three different interpolation modes with intuitive visual feedback:

- **Linear Interpolation**: Basic mathematical interpolation between two values
- **Temperature Gradient**: Physics-based temperature interpolation with intuitive color mapping
- **Custom Scale**: Normalized scaling with percentage display

Each mode provides context-appropriate units, validation, and visual representation to make interpolation calculations intuitive and educational.

## Dependencies

- **SFML 2.5+** (Simple and Fast Multimedia Library)
  - Components: graphics, window, system
- **C++17** compatible compiler (g++, clang++, MSVC)

## Building

### Prerequisites

Install SFML development libraries:

**Ubuntu/Debian:**
```bash
sudo apt-get install libsfml-dev
```

**macOS (Homebrew):**
```bash
brew install sfml
```

**Windows:**
Download SFML from [sfml-dev.org](https://www.sfml-dev.org/) and follow the setup instructions.

### Build Commands

**Release Build (Recommended):**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

**Debug Build:**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Install (Optional)

```bash
make install
```

This installs the binary and assets to system directories.

## Running

### From Build Directory

```bash
cd build
./interpolation_calculator
```

### After Installation

```bash
interpolation_calculator
```

## Controls & Modes

### Mode Switching
- **Tab**: Cycle through modes (Linear → Temperature → Custom → Linear)
- **1**: Jump to Linear Interpolation mode
- **2**: Jump to Temperature Gradient mode  
- **3**: Jump to Custom Scale mode

### Input Controls
- **Click Input Boxes**: Enter numerical values using keyboard
- **Drag Slider**: Adjust interpolation parameter in real-time
- **Reset Button**: Reset values to mode-specific defaults

### General Controls
- **H**: Toggle help overlay with mode descriptions and controls
- **Close Window**: Exit application

## Mode Details

### 1. Linear Interpolation
- **Purpose**: Basic mathematical interpolation `result = A + t(B - A)`
- **Inputs**: A (start value), B (end value), t (interpolation parameter [0,1])
- **Visualization**: White line with red interpolated point
- **Units**: None (pure mathematical values)

### 2. Temperature Gradient
- **Purpose**: Physics-based temperature interpolation at position x
- **Inputs**: T₀ (start temperature), T₁ (end temperature), x (position [0,1])
- **Visualization**: Blue→red gradient line with temperature-colored point
- **Units**: Degrees Celsius (°C)
- **Range**: -50°C to 200°C with automatic clamping
- **Color Mapping**: Blue (cold) → Cyan → Green → Yellow → Red (hot)

### 3. Custom Scale
- **Purpose**: Apply normalized scale factor to value ranges
- **Inputs**: A (scale minimum), B (scale maximum), s (scale factor [0,1])
- **Visualization**: Purple-themed line with colored interpolated point
- **Units**: Percentage (%) display for scale factor
- **Features**: Shows both absolute result and percentage of scale

## Input Validation

The application includes comprehensive input validation:

- **Float Validation**: Rejects invalid characters, multiple decimal points
- **Range Clamping**: Automatically enforces mode-specific ranges
- **Visual Feedback**: Warning messages for out-of-range inputs
- **Graceful Recovery**: Preserves last valid values when invalid input occurs

## Project Structure

```
interpolation-calculator/
├── src/                    # Core logic and math
│   ├── main.cpp            # Main application and UI
│   ├── interpolation.cpp    # Mathematical functions
│   └── interpolation.h      # Header declarations
├── ui/                     # User interface components
│   ├── Button.cpp/.h        # Reusable button component
│   ├── InputBox.cpp/.h      # Text input with validation
│   ├── Slider.cpp/.h        # Interactive slider control
│   └── UIColors.h          # Color scheme definitions
├── assets/                  # Application resources
│   └── font.ttf           # UI font
├── CMakeLists.txt          # Build configuration
└── README.md               # This file
```

## Architecture

- **Strict Separation**: UI logic (ui/) vs core math (src/)
- **Centralized Mode System**: All interpolation logic dispatched from main
- **Reusable Components**: Button, InputBox, Slider are generic UI elements
- **Physics Interpretation**: Temperature mode provides real-world context
- **Error Handling**: Comprehensive validation with user-friendly feedback

## License

This project is provided as educational and portfolio work. Please refer to the license file for usage terms.

## Contributing

This is a portfolio project demonstrating clean architecture and modern C++ practices. For educational purposes or collaborative development, please follow existing coding conventions:

- Use camelCase for methods
- Use PascalCase for classes and enums  
- Keep UI components generic and reusable
- Maintain separation between UI and business logic
- Use consistent naming and code style

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

This means you are free to:
- Use the software commercially
- Modify and distribute the software
- Use it in private projects
- Include it in larger works

Under the conditions that you:
- Include the copyright notice
- Include the license file
- Hold the original author harmless

---

*Built with modern C++17 and SFML for cross-platform desktop applications.*