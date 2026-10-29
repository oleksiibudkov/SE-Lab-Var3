// Екрани програми: головне меню, таблиця результатів, довідка
#pragma once

#include "core/input_data.h"

namespace ftc {

// Головне меню: опис програми, поточні дані і пункти
void showMenu(const InputData& d, bool hasData);

// Показує таблицю і за бажанням зберігає її у results.txt
void showResultTable(const InputData& d, bool hasData);

// Розділ Help
void showHelp();

}  // namespace ftc
