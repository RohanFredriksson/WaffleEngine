#windows
cmake_minimum_required(VERSION 3.14)

project(waffle)

add_subdirectory(dependencies/glfw/)
add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/cglm/)

include_directories(dependencies)
include_directories(include)
include_directories(include/components)
include_directories(include/core)
include_directories(include/renderer)
include_directories(include/scenes)
include_directories(include/util)

set(SOURCE
    src/main.c
    src/components/cameracontroller.c
    src/components/spriterenderer.c
    src/core/component.c
    src/core/gameobject.c
    src/core/listener.c
    src/core/scene.c
    src/core/transform.c
    src/core/window.c
    src/renderer/camera.c
    src/renderer/framebuffer.c
    src/renderer/renderer.c
    src/renderer/shader.c
    src/renderer/sprite.c
    src/renderer/texture.c
    src/scenes/title.c
    src/util/assetpool.c
    src/util/shaderpool.c
    src/util/spritepool.c
    src/util/texturepool.c
    src/util/transformpool.c
    src/util/util.c
)

add_executable(${PROJECT_NAME} ${SOURCE})
#add_executable(${PROJECT_NAME} WIN32 ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

target_include_directories(${PROJECT_NAME} PUBLIC glad)
target_include_directories(${PROJECT_NAME} PUBLIC glfw)
target_include_directories(${PROJECT_NAME} PUBLIC cglm)

target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC cglm)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")