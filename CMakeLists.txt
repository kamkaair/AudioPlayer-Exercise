# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

# set the project name
project(CitationFixer)

## C++ 17
set(CMAKE_CXX_STANDARD 17)
##set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Set the assets directory relative to the project root
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets")

# Add this as a preprocessor definition
add_compile_definitions(ASSET_DIR="${ASSET_DIR}")

# Files
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HDR_FILES "src/*.h")

add_executable(CitationFixer ${SRC_FILES} ${HDR_FILES})

