cmake_minimum_required (VERSION 3.6)

# Extend the list of CMake module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Support VS Code
include(CMakeToolsHelpers OPTIONAL)

# Define project target
project (blokus)
file(GLOB_RECURSE BLOKUS_SOURCES "src/*.c")
file(GLOB_RECURSE DEPS_SOURCES "deps/*.c")
add_executable(blokus ${BLOKUS_SOURCES} ${DEPS_SOURCES})

# Resolve dependencies
find_package(SDL2 2.0.5 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(blokus PUBLIC
  "deps"
  ${SDL2_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
)

target_link_libraries(blokus
  ${SDL2_LIBRARY}
  ${SDL2_TTF_LIBRARIES}
)

# Additional compiler flags
if (CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(blokus PUBLIC -std=c99 -Wall -Wextra -pedantic)
endif ()

# Copy resources
file(COPY "resources" DESTINATION ".")
