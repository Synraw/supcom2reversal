cmake_minimum_required (VERSION 3.0.0)
project (Supcom)

# D3D9 Lib for drawing
include_directories($ENV{DXSDK_DIR}/Include)
link_directories($ENV{DXSDK_DIR}/Lib/x86)

# Detours
include_directories(${CMAKE_SOURCE_DIR}/libs/detours/include)
link_directories(${CMAKE_SOURCE_DIR}/libs/detours/lib)

# Output our binaries to the /bin folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Multibyte characters pls
add_definitions(-D_MBCS -DMBCS )

# Build!
file(GLOB_RECURSE SOURCES "*.cpp")
add_library(Supcom SHARED ${SOURCES})
target_link_libraries(Supcom d3d9 d3dx9 detours )

