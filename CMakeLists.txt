cmake_minimum_required(VERSION 3.14)

project(waffle)

add_subdirectory(dependencies/glfw/)
add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/cglm/)

include_directories(include)
include_directories(dependencies)

set(SOURCE_DIR src)
set(SOURCE
    ${SOURCE}
    src/main.c
    src/window.c
    src/listener.c
    src/texture.c
    src/shader.c
    src/transform.c
    src/sprite.c
    src/spriterenderer.c
    src/component.c
    src/gameobject.c
    src/camera.c
    src/scene.c
    src/renderer.c
    src/assetpool.c
    src/texturepool.c
    src/shaderpool.c
    src/spritepool.c
    src/transformpool.c
    src/framebuffer.c
    src/title.c
)

#add_executable(${PROJECT_NAME} ${SOURCE})
add_executable(${PROJECT_NAME} WIN32 ${SOURCE})

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