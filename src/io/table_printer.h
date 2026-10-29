// Таблиця значень x та y
#pragma once

#include <ostream>

#include "core/input_data.h"

namespace ftc {

// Друкує таблицю на екран (std::cout) або у файл (std::ofstream).
// x = 0 позначається "not defined", переповнення — "too large".
void printTable(std::ostream& out, const InputData& d);

}  // namespace ftc
