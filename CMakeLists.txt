cmake_minimum_required(VERSION 3.14)

set(project_name waffle)
project(${project_name})

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list include/*.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

add_subdirectory(dependencies/glfw/)
add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/cglm/)

set(cimgui dependencies/cimgui)
set(imgui_impl ${cimgui}/imgui/backends)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(dependencies)
include_directories(dependencies/glfw/include)
include_directories(dependencies/cglm/include)

include_directories(${cimgui})
include_directories(${cimgui}/imgui)
include_directories(${imgui_impl})

HEADER_DIRECTORIES(header_directories)
include_directories(${header_directories})

file(GLOB 
	cimgui_source
	"${cimgui}/*.cpp" 
	"${cimgui}/imgui/*.cpp"
	"${imgui_impl}/imgui_impl_glfw.cpp"
	"${imgui_impl}/imgui_impl_opengl3.cpp")
add_library(cimgui ${cimgui_source})
target_compile_definitions(cimgui PRIVATE 
	IMGUI_IMPL_API=extern\ \"C\"
	IMGUI_IMPL_OPENGL_LOADER_GLAD)

file(GLOB_RECURSE source_files src/*.c)

add_executable(${project_name} ${source_files})
#add_executable(${project_name} WIN32 ${source_files})

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

target_include_directories(${project_name} PUBLIC glfw)
target_include_directories(${project_name} PUBLIC glad)
target_include_directories(${project_name} PUBLIC cglm)

if (WIN32)
find_library(pthread NAME pthread)
target_link_libraries(${project_name} pthread)
target_link_libraries(${project_name} -static)
endif()

target_link_libraries(${project_name} glfw)
target_link_libraries(${project_name} glad)
target_link_libraries(${project_name} cglm)
target_link_libraries(${project_name} cimgui)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")