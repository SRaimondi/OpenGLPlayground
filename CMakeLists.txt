cmake_minimum_required(VERSION 3.10)
project(OpenGLPlayground)

# CMake standard
set(CMAKE_CXX_STANDARD 14)

# Set verbose makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wextra")

# Add files to exectuable
add_executable(OpenGLPlayground
        main.cpp
        GLUtils.cpp GLUtils.hpp
        Shader.cpp Shader.hpp
        FileIO.cpp FileIO.hpp
        Mesh.cpp Mesh.hpp
        Model.cpp Model.hpp
        FrameCounter.cpp FrameCounter.hpp UniformBlock.cpp UniformBlock.hpp)

# Find GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(OpenGLPlayground ${GLEW_LIBRARIES})
endif ()

# Find GLFW
find_package(glfw3 3.3 REQUIRED)
if (glfw3_FOUND)
    target_link_libraries(OpenGLPlayground glfw)
endif ()

# Find OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_link_libraries(OpenGLPlayground OpenGL::GL)
endif ()

# Link assimp
find_package(assimp REQUIRED)
if (assimp_FOUND)
    target_link_libraries(OpenGLPlayground assimp)
endif ()


