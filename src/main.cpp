// Function Table Calculator, варіант 3 — точка входу.
// Таблиця значень функції y(x) для x від початкового до кінцевого значення з кроком dx:
//   y = 1/(2x) + P(i + x), i = 1..n-2,                    якщо x < 3
//   y = P(j = 1..n) S(i = 1..n+3) (i + 1/(4j^2)),         якщо x >= 3
//
// Модулі:
//   core/  — дані, обчислення y(x), перевірки, форматування (без вводу/виводу)
//   io/    — консоль, введення даних (клавіатура, файл), друк таблиці
//   ui/    — екрани: меню, таблиця результатів, довідка
#include <iostream>

#include "core/input_data.h"
#include "io/console.h"
#include "io/data_input.h"
#include "ui/screens.h"

int main() {
    ftc::InputData data;
    bool hasData = false;

    // Головне меню: повторюється, доки користувач не вибере 0
    while (true) {
        ftc::showMenu(data, hasData);
        int choice = ftc::readInt(" Choose an option (0-4): ");

        if (choice == 1) {
            data = ftc::enterFromKeyboard();
            hasData = true;
        }
        else if (choice == 2) {
            if (ftc::loadFromFile(data)) hasData = true;
        }
        else if (choice == 3) {
            ftc::showResultTable(data, hasData);
        }
        else if (choice == 4) {
            ftc::showHelp();
        }
        else if (choice == 0) {
            std::cout << " Goodbye!\n";
            return 0;
        }
        else {
            std::cout << "  There is no option " << choice << ". Please choose a number from 0 to 4.\n";
        }
    }
}
