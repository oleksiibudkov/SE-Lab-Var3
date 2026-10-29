# Збирання через CMake: Visual Studio (File → Open → Folder), CLion або з терміналу:
#   cmake -S . -B build && cmake --build build
cmake_minimum_required(VERSION 3.15)
project(FunctionTableCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calculator
    src/main.cpp
    src/core/calculator.cpp
    src/core/format.cpp
    src/core/validation.cpp
    src/io/console.cpp
    src/io/data_input.cpp
    src/io/table_printer.cpp
    src/ui/screens.cpp
)
target_include_directories(calculator PRIVATE src)

if(MSVC)
    target_compile_options(calculator PRIVATE /W4 /utf-8)
else()
    target_compile_options(calculator PRIVATE -Wall -Wextra -pedantic)
endif()

# Приклад вхідних даних поруч із програмою, щоб пункт 2 меню працював одразу
configure_file(data/input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
