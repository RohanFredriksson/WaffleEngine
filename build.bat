@echo off
git submodule update --init --recursive
cmake . -G "MinGW Makefiles"
mingw32-make