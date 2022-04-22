@echo off
cmake . -G "MinGW Makefiles"
mingw32-make
del CMakeCache.txt /s /f /q > nul
del cmake_install.cmake /s /f /q > nul
del Makefile /s /f /q > nul
del cglm.pc /s /f /q > nul
del CPackConfig.cmake /s /f /q > nul
del CPackSourceConfig.cmake /s /f /q > nul
@RD /S /Q ".\CMakeFiles" > nul