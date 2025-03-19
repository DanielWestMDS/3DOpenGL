# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(3DOpenGL)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source directory
add_executable(3DOpenGL OpenGL_Project/Source.cpp)

# Include directories (if you have headers in include/)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Link ReactPhysics3D to your project
target_link_libraries(3DOpenGL reactphysics3d)