cmake_minimum_required (VERSION 3.0.0)
project (Supcom)

# Output our binaries to the /bin folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Multibyte characters pls
add_definitions(-D_MBCS -DMBCS )

# Build!
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Supcom SHARED ${SOURCES})
