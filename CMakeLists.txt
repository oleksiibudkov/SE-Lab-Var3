# Збирання через CMake: Visual Studio (File → Open → Folder), CLion або з терміналу:
#   cmake -S . -B build && cmake --build build
# Unit tests: ./run_tests.sh (потрібен GoogleTest: brew install googletest)
cmake_minimum_required(VERSION 3.15)
project(FunctionTableCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(WARNINGS /W4 /utf-8)
else()
    set(WARNINGS -Wall -Wextra -pedantic)
endif()

# Логіка без вводу/виводу на екран: її використовують і програма, і тести
add_library(core STATIC
    src/core/calculator.cpp
    src/core/data_file.cpp
    src/core/format.cpp
    src/core/validation.cpp
)
target_include_directories(core PUBLIC src)
target_compile_options(core PRIVATE ${WARNINGS})

# Програма
add_executable(calculator
    src/main.cpp
    src/io/console.cpp
    src/io/data_input.cpp
    src/io/table_printer.cpp
    src/ui/screens.cpp
)
target_link_libraries(calculator PRIVATE core)
target_compile_options(calculator PRIVATE ${WARNINGS})

# Приклад вхідних даних поруч із програмою, щоб пункт 2 меню працював одразу
configure_file(data/input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)

# Unit tests (GoogleTest). Якщо GoogleTest не встановлено, збирається лише програма
find_package(GTest)
if(GTest_FOUND)
    enable_testing()
    add_executable(unit_tests tests/test_calc.cpp)
    target_link_libraries(unit_tests PRIVATE core GTest::gtest_main)
    target_compile_options(unit_tests PRIVATE ${WARNINGS})
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
else()
    message(STATUS "GoogleTest not found: unit tests are not built")
endif()
