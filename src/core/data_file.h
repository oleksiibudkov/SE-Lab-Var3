// Читання вхідних даних з текстового файлу (без виводу на екран)
#pragma once

#include <string>

namespace ftc {

// Читає з файлу 4 числа: start end step n.
// Кидає std::runtime_error, якщо файл не вдалося відкрити,
// і std::invalid_argument, якщо вміст неправильний.
// Змінні змінюються, лише якщо весь файл правильний.
void loadFromFile(const std::string& fileName, double& xStart, double& xEnd, double& dx, int& n);

}  // namespace ftc
