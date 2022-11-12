#!/usr/bin/env bash
if test -f os.txt; then
    os=`cat os.txt`
    if [ $os == "windows" ]; then
        rm CMakeCache.txt
    fi
elif test -f CMakeCache.txt; then
    rm CMakeCache.txt
fi

echo linux > os.txt
rm -f waffle 2> /dev/null
git submodule update --init --recursive
cmake .
make

if test -f waffle; then
    rm -rf build 2> /dev/null
    mkdir build
    cp waffle build/waffle
    cp -r assets build/assets
fi