#!/usr/bin/env bash
rm -rf build/
cmake -S . -B build
cmake --build build

