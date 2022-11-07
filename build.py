import os
import platform
import subprocess
import shutil

operating_system = platform.system().lower()

def run(command: str):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    while True:
        line = process.stdout.readline()
        if not line: 
            break
        print(line.decode('utf-8').replace('\n', ''))

def write_file(filename: str, contents: str):
    f = open(filename, 'w')
    f.write(contents)
    f.close()

def read_file(filename: str) -> str:
    f = open(filename, 'r')
    contents = f.read()
    f.close()
    return contents

def build():

    # If the operating system changes, remove the cache
    if os.path.exists("os.txt"):
        tag = read_file("os.txt")
        if tag != operating_system and os.path.exists("CMakeCache.txt"):
            os.remove("CMakeCache.txt")

    # If there is no os tag and the cache exists, 
    # we cannot guarantee the os, remove the old cache.
    elif os.path.exists("CMakeCache.txt"):
        os.remove("CMakeCache.txt")

    # Update the os cache
    write_file("os.txt", operating_system)

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
    
if __name__ == '__main__':
    build()