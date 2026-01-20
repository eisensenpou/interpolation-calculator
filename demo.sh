#!/bin/bash

# Interpolation Calculator Demo Script
# This script demonstrates the application for portfolio purposes

echo "=== INTERPOLATION CALCULATOR DEMO ==="
echo ""
echo "This script runs the interpolation calculator with different modes"
echo "to demonstrate the full feature set for portfolio presentation."
echo ""

# Check if build exists
if [ ! -f "build/interpolation_calculator" ]; then
    echo "Building application first..."
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    cd ..
fi

echo "Starting interpolation calculator..."
echo ""
echo "=== CONTROLS FOR DEMO ==="
echo "Tab: Switch between modes"
echo "1-3: Jump to specific mode"
echo "H: Toggle help overlay"
echo "Click: Enter values in input boxes"
echo "Drag: Adjust slider"
echo "Click Reset: Reset to mode defaults"
echo ""
echo "=== MODES TO DEMONSTRATE ==="
echo "1. Linear Interpolation - Basic mathematical interpolation"
echo "2. Temperature Gradient - Physics with blue->red color mapping"
echo "3. Custom Scale - Normalized scaling with percentages"
echo ""
echo "Press Ctrl+C to exit demo"
echo ""

# Run the application
cd build
./interpolation_calculator