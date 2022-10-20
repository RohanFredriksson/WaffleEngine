import os
import platform
import subprocess
import shutil

head = """cmake_minimum_required(VERSION 3.14)

set(project_name waffle)
project(${project_name})

add_subdirectory(dependencies/glfw/)
add_subdirectory(dependencies/glad/)
add_subdirectory(dependencies/cglm)

set(cimgui dependencies/cimgui)
set(imgui_impl ${cimgui}/imgui/backends)
set(gl3w dependencies/gl3w)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(dependencies)
include_directories(dependencies/glfw/include)
include_directories(dependencies/cglm/include)

include_directories(${cimgui})
include_directories(${cimgui}/imgui)
include_directories(${imgui_impl})
include_directories(${gl3w})

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

"""

tail = """
add_executable(${project_name} ${source_files})
#add_executable(${project_name} WIN32 ${source_files})

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#set_target_properties(${project_name} PROPERTIES LINK_SEARCH_START_STATIC 1)
#set_target_properties(${project_name} PROPERTIES LINK_SEARCH_END_STATIC 1)
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

target_include_directories(${project_name} PUBLIC glfw)
target_include_directories(${project_name} PUBLIC glad)
target_include_directories(${project_name} PUBLIC cglm)

target_link_libraries(${project_name} glfw)
target_link_libraries(${project_name} glad)
target_link_libraries(${project_name} cglm)
target_link_libraries(${project_name} cimgui)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")"""

operating_system = platform.system().lower()

def run(command: str):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    while True:
        line = process.stdout.readline()
        if not line: 
            break
        print(line.decode('utf-8').replace('\n', ''))

def create_include():

    include = ""
    for (dirpath, dirnames, filenames) in os.walk("include"):
        include += "include_directories(" + dirpath.replace('\\','/') + ")\n"
    include += "\n"

    return include

def create_source():

    source = "set(source_files\n"
    for (dirpath, dirnames, filenames) in os.walk("src"):
        for filename in filenames:
            if filename.endswith(".c"):
                source += "    " + dirpath.replace('\\','/') + "/" + filename + "\n"
    
    source += ")\n"
    return source

def write_file(filename: str, contents: str):

    f = open(filename, 'w')
    f.write(contents)
    f.close()

def update_cmakelists():

    # Generate the latest CMakeLists file.
    tag = "#" + operating_system + "\n"
    include = create_include()
    source = create_source()

    # Combine all components to build the latest CMakeLists file.
    new_script = tag + head + include + source + tail

    # Get the contents of the previous CMakeLists file.
    old_script = ""
    if os.path.exists("CMakeLists.txt"):
        with open("CMakeLists.txt", 'r') as f:
            old_script = f.read()
            f.close()
    
    # If the file has changed, overwrite the old one with the latest.
    if old_script != new_script:

        # If the operating system has changed, remove cache.
        old_tag = old_script.split('\n')[0] + "\n"
        if old_tag != tag:
            if os.path.exists("CMakeCache.txt"):
                os.remove("CMakeCache.txt")
        
        # Overwrite the old file with the new one.
        write_file("CMakeLists.txt", new_script)

def build():

    # Build the program depending on operating system
    if operating_system == "windows":
        run("git submodule update --init --recursive")
        run("cmake . -G \"MinGW Makefiles\"")
        run("mingw32-make")
    else:
        run("git submodule update --init --recursive\n")
        run("cmake .\n")
        run("make\n")

    # If the build failed stop
    if operating_system == "windows":
        if not os.path.exists("waffle.exe"):
            return
    else:
        if not os.path.exists("waffle"):
            return

    # Create the build directory if it doesn't exist
    if not os.path.isdir("build"):
        os.mkdir("build")

    # Update the assets in the build directory
    if os.path.isdir("build/assets"):
        shutil.rmtree("build/assets")
    shutil.copytree("assets", "build/assets")
    
    # Copy the application into the build file
    if operating_system == "windows":
        shutil.copy("waffle.exe", "build")
    else:
        shutil.copy("waffle", "build")

def main():

    update_cmakelists()
    build()

    
if __name__ == '__main__':
    main()