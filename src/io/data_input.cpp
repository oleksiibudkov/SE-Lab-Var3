#include "io/data_input.h"

#include <fstream>
#include <iostream>
#include <string>

#include "core/format.h"
#include "core/validation.h"
#include "io/console.h"

namespace ftc {

InputData enterFromKeyboard() {
    printTitle("ENTER DATA FROM KEYBOARD");
    std::cout << " The table shows y for every x from the start value to the end value,\n"
              << " moving by the step. Example: start -2, end 4, step 1 gives\n"
              << " x = -2, -1, 0, 1, 2, 3, 4.\n\n";
    InputData d;
    std::string error;

    d.xStart = readDouble(" Start value of x (where the table begins, e.g. -2): ");

    do {
        d.xEnd = readDouble(" End value of x (where the table ends, not smaller than the start, e.g. 4): ");
        error = checkXEnd(d);
        if (!error.empty()) std::cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());

    do {
        d.dx = readDouble(" Step (how much x grows in each row, greater than 0, e.g. 1): ");
        error = checkDx(d);
        if (!error.empty()) std::cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());

    double n;
    do {
        n = readDouble(" Parameter n (the last number the formula counts up to: 1, 2, ... n; whole number from 4 to "
                       + std::to_string(MAX_N) + ", e.g. 5): ");
        error = checkN(n);
        if (!error.empty()) std::cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());
    d.n = static_cast<int>(n);

    std::cout << "\n Data accepted: " << dataText(d) << "\n";
    pause();
    return d;
}

bool loadFromFile(InputData& data) {
    printTitle("LOAD DATA FROM FILE");
    std::cout << " The file must contain 4 numbers separated by spaces, in this order:\n"
              << "   start  end  step  n      (example: -2 4 1 5)\n\n";
    std::string fileName = readLine(" File name (just press Enter to use input.txt): ");
    if (fileName.empty()) fileName = "input.txt";

    std::ifstream file(fileName);
    InputData d;
    double n = 0;
    std::string extra, error;
    if (!file)
        error = "the file was not found. Check the name and that the file is in the program folder";
    else if (!(file >> d.xStart >> d.xEnd >> d.dx >> n))
        error = "the file must contain 4 numbers: start, end, step and n";
    else if (file >> extra)
        error = "the file contains more than 4 values";
    if (error.empty()) error = checkXEnd(d);
    if (error.empty()) error = checkDx(d);
    if (error.empty()) error = checkN(n);

    if (error.empty()) {
        d.n = static_cast<int>(n);
        data = d;
        std::cout << "\n Data loaded: " << dataText(data) << "\n";
    } else {
        std::cout << "\n Could not load \"" << fileName << "\": " << error << ".\n"
                  << " Your previous data was not changed.\n";
    }
    pause();
    return error.empty();
}

}  // namespace ftc
