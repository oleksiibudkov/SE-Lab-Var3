# Збирання з терміналу (g++ або clang++):
#   make         — зібрати build/calculator
#   make run     — зібрати і запустити в папці data/ (там лежить input.txt)
#   make test    — регресійний тест: вивід такий самий, як у початкової програми
#   make clean   — видалити build/
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2

SRC := $(wildcard src/*.cpp src/*/*.cpp)
OBJ := $(SRC:src/%.cpp=build/%.o)
BIN := build/calculator

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Isrc -MMD -MP -c $< -o $@

run: $(BIN)
	cd data && ../$(BIN)

test: $(BIN)
	bash tests/regression/run.sh $(BIN)

clean:
	rm -rf build

.PHONY: all run test clean

-include $(OBJ:.o=.d)
