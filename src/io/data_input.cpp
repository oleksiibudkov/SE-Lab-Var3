#include "io/data_input.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "core/data_file.h"
#include "core/format.h"
#include "core/validation.h"
#include "io/console.h"

namespace ftc {

// Друкує помилку введеного значення і просить ввести його ще раз
static void printWrongValue(const std::invalid_argument& e) {
    std::cout << "  Wrong value: " << e.what() << ". Please try again.\n";
}

// Запитує start, end, step і n; кожне значення перевіряється одразу після введення
InputData enterFromKeyboard() {
    printTitle("ENTER DATA FROM KEYBOARD");
    std::cout << " The table shows y for every x from the start value to the end value,\n"
              << " moving by the step. Example: start -2, end 4, step 1 gives\n"
              << " x = -2, -1, 0, 1, 2, 3, 4.\n\n";
    InputData d;

    d.xStart = readDouble(" Start value of x (where the table begins, e.g. -2): ");

    while (true) {
        d.xEnd = readDouble(" End value of x (where the table ends, not smaller than the start, e.g. 4): ");
        try {
            checkRange(d.xStart, d.xEnd);
            break;
        } catch (const std::invalid_argument& e) {
            printWrongValue(e);
        }
    }

    while (true) {
        d.dx = readDouble(" Step (how much x grows in each row, greater than 0, e.g. 1): ");
        try {
            checkStep(d.xStart, d.xEnd, d.dx);
            break;
        } catch (const std::invalid_argument& e) {
            printWrongValue(e);
        }
    }

    double n;
    while (true) {
        n = readDouble(" Parameter n (the last number the formula counts up to: 1, 2, ... n; whole number from 4 to "
                       + std::to_string(MAX_N) + ", e.g. 5): ");
        try {
            checkN(n);
            break;
        } catch (const std::invalid_argument& e) {
            printWrongValue(e);
        }
    }
    d.n = static_cast<int>(n);

    std::cout << "\n Data accepted: " << dataText(d) << "\n";
    pause();
    return d;
}

// Екран завантаження з файлу: питає ім'я, ловить винятки loadFromFile
bool askAndLoadFile(InputData& data) {
    printTitle("LOAD DATA FROM FILE");
    std::cout << " The file must contain 4 numbers separated by spaces, in this order:\n"
              << "   start  end  step  n      (example: -2 4 1 5)\n\n";
    std::string fileName = readLine(" File name (just press Enter to use input.txt): ");
    if (fileName.empty()) fileName = "input.txt";

    bool loaded = false;
    try {
        loadFromFile(fileName, data.xStart, data.xEnd, data.dx, data.n);
        std::cout << "\n Data loaded: " << dataText(data) << "\n";
        loaded = true;
    } catch (const std::exception& e) {  // std::runtime_error (немає файлу) і std::invalid_argument (вміст)
        std::cout << "\n Could not load \"" << fileName << "\": " << e.what() << ".\n"
                  << " Your previous data was not changed.\n";
    }
    pause();
    return loaded;
}

}  // namespace ftc
