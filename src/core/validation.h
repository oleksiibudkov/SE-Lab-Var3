// Перевірки і розбір вхідних даних, однакові для клавіатури і файлу.
// Кожна перевірка нічого не повертає, якщо все правильно,
// і кидає std::invalid_argument з текстом помилки, якщо ні.
#pragma once

#include <string>

namespace ftc {

// Кінцеве значення не менше за початкове
void checkRange(double xStart, double xEnd);

// Кількість рядків таблиці: x = xStart, xStart + dx, ... до xEnd.
// Викликати лише для даних, які пройшли checkStep
int countRows(double xStart, double xEnd, double dx);

// Крок більший за 0 і дає не більше MAX_POINTS рядків
void checkStep(double xStart, double xEnd, double dx);

// n ціле, від 4 до MAX_N (n передається як double, щоб розпізнати дробове)
void checkN(double n);

// Усі чотири значення разом: скінченні числа, а далі checkRange, checkStep, checkN
void checkValidParams(double xStart, double xEnd, double dx, double n);

// Текст -> дійсне число; пробіли навколо дозволені. Кидає для "", "abc", "2.5x", "inf"
double parseDouble(const std::string& text);

// Текст -> ціле число; пробіли навколо дозволені. Кидає для "", "4.5", "5abc"
int parseInt(const std::string& text);

}  // namespace ftc
