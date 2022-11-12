cmake_minimum_required(VERSION 3.14)

set(project_name waffle)
set(project_library waffle_core)
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
add_subdirectory(dependencies/cdsl/)

set(cimgui dependencies/cimgui)
set(imgui_impl ${cimgui}/imgui/backends)

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    set(CMAKE_VERBOSE_MAKEFILE ON)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -fsanitize=address")
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

include_directories(dependencies)
include_directories(dependencies/glfw/include)
include_directories(dependencies/cglm/include)
include_directories(dependencies/cdsl/include)

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
list(REMOVE_ITEM source_files "src/main.c" "src/test.c")

add_library(${project_library} ${source_files})
target_include_directories(${project_library} PUBLIC glfw)
target_include_directories(${project_library} PUBLIC glad)
target_include_directories(${project_library} PUBLIC cglm)
target_include_directories(${project_library} PUBLIC cdsl)

if (WIN32)
find_library(pthread NAME pthread)
target_link_libraries(${project_library} pthread)
target_link_libraries(${project_library} -static)
endif()

target_link_libraries(${project_library} glfw)
target_link_libraries(${project_library} glad)
target_link_libraries(${project_library} cglm)
target_link_libraries(${project_library} cdsl)
target_link_libraries(${project_library} cimgui)

add_executable(${project_name} src/main.c)
#add_executable(${project_name} WIN32 src/main.c)

target_link_libraries(${project_name} ${project_library})

add_executable(waffle_test src/test.c)
target_link_libraries(waffle_test ${project_library})
enable_testing()

add_test(
    NAME waffle_test
    COMMAND $<TARGET_FILE:waffle_test>
)