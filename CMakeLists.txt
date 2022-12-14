cmake_minimum_required(VERSION 3.16)

# Sets project name
project(projects_realtime LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specifies required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)

# Allows you to include files from within those directories, without prefixing their filepaths
include_directories(src)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    src/main.cpp

    src/realtime.cpp
    src/mainwindow.cpp
    src/settings.cpp
    src/utils/scenefilereader.cpp
    src/utils/sceneparser.cpp
    src/camera/camera.cpp
    src/camera/transforms.cpp
    src/trimeshes/cube.cpp
    src/trimeshes/cone.cpp
    src/trimeshes/sphere.cpp
    src/trimeshes/cylinder.cpp
    src/trimeshes/obj_mesh.cpp
    src/renderer/primitive.cpp
    src/renderer/render.cpp

    src/shader/phong.cpp
    src/shader/texture.cpp
    src/shader/shadow.cpp
    src/raytracer/raytracer.cpp
    src/raytracer/raytracescene.cpp
    src/raytracer/raytracescene.cpp
    src/raytracer/utils.cpp
    src/raytracer/primitive.cpp

    src/raytracer_driver.h

    src/mainwindow.h
    src/realtime.h
    src/settings.h
    src/utils/scenedata.h
    src/utils/scenefilereader.h
    src/utils/sceneparser.h
    src/utils/shaderloader.h
    src/camera/camera.h
    src/camera/transforms.h
    src/trimeshes/trimesh.h
    src/trimeshes/cube.h
    src/trimeshes/cone.h
    src/trimeshes/sphere.h
    src/trimeshes/cylinder.h
    src/trimeshes/obj_mesh.h
    src/renderer/debug.h
    src/renderer/primitive.h
    src/filter/filter.h

    src/shader/phong.h
    src/shader/shadow.h
    src/shader/texture.h
    src/utils/rgba.h
    src/raytracer/raytracer.h
    src/raytracer/raytracescene.h
    src/raytracer/raytracescene.h
    src/raytracer/utils.h
    src/raytracer/primitive.h
    src/raytracer/implicit.h
    src/raytracer/bvh.h
)

# GLM: this creates its library and allows you to `#include "glm/..."`
add_subdirectory(glm)

# GLEW: this creates its library and allows you to `#include "GL/glew.h"`
add_library(StaticGLEW STATIC glew/src/glew.c)
include_directories(${PROJECT_NAME} PRIVATE glew/include)

# Specifies libraries to be linked (Qt components, glew, etc)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Xml
    StaticGLEW
)

# Specifies other files
qt6_add_resources(${PROJECT_NAME} "Resources"
    PREFIX
        "/"
    FILES
        resources/shaders/kitten.png
        resources/shaders/default.frag
        resources/shaders/default.vert
        resources/shaders/texture.frag
        resources/shaders/texture.vert
)

# GLEW: this provides support for Windows (including 64-bit)
if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    glu32
  )
endif()

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()
# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()
