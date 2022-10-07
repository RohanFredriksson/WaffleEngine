@echo off
git submodule update --init --recursive
cmake . -G "MinGW Makefiles"
mingw32-make
if not exist build (mkdir build > NUL)
xcopy /s /F /Q /y assets\ build\assets\ > NUL
xcopy /F /Q /y waffle.exe build\waffle.exe > NUL