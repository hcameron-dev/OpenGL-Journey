cmake_minimum_required(VERSION 3.10.0)
project(TexturesinOpenGL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include(CTest)
enable_testing()

set(PROJECT_SOURCES
    src/main.cpp
    src/config.h
    src/glad.c
    headers/shader.h
    headers/stb_image.h
)

add_executable(TexturesinOpenGL ${PROJECT_SOURCES})

target_include_directories(TexturesinOpenGL PUBLIC 
    Implementation
    Headers
    dependencies
    src
    bin
)

set_target_properties(TexturesinOpenGL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

target_link_libraries(TexturesinOpenGL ${GLFW3_LIBRARY} OpenGL::GL)

if(MSVC)
    target_link_options(TexturesinOpenGL PRIVATE "/NODEFAULTLIB:MSVCRT" "/NODEFAULTLIB:LIBCMT")
endif()