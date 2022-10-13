#windows
cmake_minimum_required(VERSION 3.14)

project(waffle)

add_subdirectory(dependencies/glfw/)
add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/cglm/)

include_directories(dependencies)
include_directories(include)

set(SOURCE
    src/assetpool.c
    src/camera.c
    src/component.c
    src/framebuffer.c
    src/gameobject.c
    src/listener.c
    src/main.c
    src/renderer.c
    src/scene.c
    src/shader.c
    src/shaderpool.c
    src/sprite.c
    src/spritepool.c
    src/spriterenderer.c
    src/texture.c
    src/texturepool.c
    src/title.c
    src/transform.c
    src/transformpool.c
    src/util.c
    src/window.c
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