cmake_minimum_required(VERSION 3.16)
project(DWC_simulation)

# Tell CMake to prioritize your Miniconda libraries where Geant4 and ZLIB 1.3.2 live
list(APPEND CMAKE_PREFIX_PATH "/home/aniket/miniconda3")

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Garfield++
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /home/aniket/garfield/install/include
)

link_directories(
    /home/aniket/garfield/install/lib
)

# Sources
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Executable
add_executable(
    DWC_simulation
    main.cc
    ${sources}
    ${headers}
)

# Libraries
target_link_libraries(
    DWC_simulation
    ${Geant4_LIBRARIES}
    Garfield
)
