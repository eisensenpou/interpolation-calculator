cmake_minimum_required(VERSION 3.10)

project(interpolation_calculator LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type defaults
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    ui/*.cpp
)

# Create executable
add_executable(interpolation_calculator ${SRC_FILES})

# Include directories
target_include_directories(interpolation_calculator PRIVATE
    src
    ui
)

# Link SFML
target_link_libraries(interpolation_calculator
    sfml-graphics
    sfml-window
    sfml-system
)

# Custom target to copy assets (ensures assets are always up-to-date)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    DEPENDS ${CMAKE_SOURCE_DIR}/assets
    COMMENT "Copying assets to build directory"
)

# Make main target depend on assets
add_dependencies(interpolation_calculator copy_assets)

# Include GNUInstallDirs for standard paths
include(GNUInstallDirs)

# Install targets
install(TARGETS interpolation_calculator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY assets
    DESTINATION ${CMAKE_INSTALL_DATADIR}/interpolation_calculator
    FILES_MATCHING PATTERN "*.ttf"
)