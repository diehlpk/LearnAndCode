cmake_minimum_required(VERSION 3.19)

project(gmsh_read 
    DESCRIPTION  "Read a msh file using gmsh and store the nnodes and connectivity"
    LANGUAGES CXX)

# Add own cmake scripts
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Set director for all exectuables
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

#Gmsh
find_package(Gmsh REQUIRED)

# BLAZE 
find_package(blaze REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
include("${blaze_DIR}/blaze-config-version.cmake")
link_libraries(blaze::blaze)

add_executable(mesh main.cpp)
target_link_libraries(mesh ${GMSH_LIB})

set_property(TARGET mesh PROPERTY CXX_STANDARD 17)
