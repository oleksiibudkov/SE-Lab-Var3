// Отримання вхідних даних: з клавіатури або з файлу
#pragma once

#include "core/input_data.h"

namespace ftc {

// Запитує кожне значення, доки воно не правильне, і повертає прийняті дані
InputData enterFromKeyboard();

// Читає 4 числа з файлу. Дані замінюються, лише якщо весь файл правильний;
// повертає true, якщо дані завантажено
bool loadFromFile(InputData& data);

}  // namespace ftc
