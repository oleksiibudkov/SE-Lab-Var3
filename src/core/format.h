// Перетворення чисел і вхідних даних на текст для повідомлень
#pragma once

#include <string>

#include "core/input_data.h"

namespace ftc {

// Число як короткий текст: -2, 0.5, 4
std::string num(double value);

// Опис даних одним рядком: "x from -2 to 4, step 1, n = 5"
std::string dataText(const InputData& d);

}  // namespace ftc
