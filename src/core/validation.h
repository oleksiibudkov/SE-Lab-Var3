// Перевірки вхідних даних, однакові для клавіатури і файлу.
// Кожна функція повертає "" якщо все правильно, інакше текст помилки.
#pragma once

#include <string>

#include "core/input_data.h"

namespace ftc {

// Кінцеве значення не менше за початкове
std::string checkXEnd(const InputData& d);

// Крок більший за 0 і дає не більше MAX_POINTS рядків
std::string checkDx(const InputData& d);

// n ціле, від 4 до MAX_N (n зчитується як double, щоб розпізнати дробове)
std::string checkN(double n);

}  // namespace ftc
