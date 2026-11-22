#!/usr/bin/env bash
# Збирає програму і unit tests через CMake у папку build/ і запускає тести GoogleTest.
# Використання: ./run_tests.sh   (додаткові параметри передаються тестам, напр. --gtest_filter=CalcY.*)
set -e
cd "$(dirname "$0")"
cmake -S . -B build
cmake --build build
./build/unit_tests --gtest_color=yes "$@"
