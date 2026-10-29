#include "ui/screens.h"

#include <fstream>
#include <iostream>

#include "core/format.h"
#include "io/console.h"
#include "io/table_printer.h"

namespace ftc {

void showMenu(const InputData& d, bool hasData) {
    printTitle("FUNCTION TABLE CALCULATOR (Variant 3) - MAIN MENU");
    std::cout << " This program calculates the function y(x) for every x\n"
              << " from a start value to an end value with a chosen step and shows a table:\n"
              << "   y = 1/(2x) + product(i + x), i = 1..n-2                 if x < 3\n"
              << "   y = product(j = 1..n) of sum(i = 1..n+3) (i + 1/(4j^2))   if x >= 3\n"
              << "--------------------------------------------------\n";
    if (hasData) std::cout << " Current data: " << dataText(d) << "\n";
    else std::cout << " Current data: not entered yet (choose 1 or 2)\n";
    std::cout << "--------------------------------------------------\n"
              << " 1. Enter data from keyboard  - type the start, end, step and n\n"
              << " 2. Load data from file       - read the same 4 values from a file\n"
              << " 3. Calculate and show table  - build the table of y(x), save it if needed\n"
              << " 4. Help                      - meaning of every value, rules, examples\n"
              << " 0. Exit                      - close the program\n";
}

void showResultTable(const InputData& d, bool hasData) {
    printTitle("RESULT TABLE");
    if (!hasData) {
        std::cout << " There is no data yet. Please enter it first: option 1 (keyboard) or option 2 (file).\n";
        pause();
        return;
    }
    std::cout << " Data: " << dataText(d) << "\n\n";
    printTable(std::cout, d);

    if (askYesNo("\n Do you want to save this table to the file results.txt? (y/n): ")) {
        std::ofstream file("results.txt");
        if (file) {
            file << "Data: " << dataText(d) << "\n\n";
            printTable(file, d);
            std::cout << " Table saved to results.txt\n";
        } else {
            std::cout << " Sorry, the file results.txt could not be created.\n";
        }
    }
    pause();
}

void showHelp() {
    printTitle("HELP");
    std::cout << " WHAT THE PROGRAM DOES\n"
              << "   Builds a table of y(x) for x = start, start + step, start + 2*step, ... up to the end.\n\n"
              << " INPUT DATA\n"
              << "   Start value   where the table begins        any number, e.g. -2\n"
              << "   End value     where the table ends          not smaller than the start, e.g. 4\n"
              << "   Step          how much x grows in each row  greater than 0, at most "
              << MAX_POINTS << " rows\n"
              << "   Parameter n   the last number the formula counts up to (1, 2, ... n),\n"
              << "                 whole number from 4 to " << MAX_N << ". The bigger n, the bigger the results.\n"
              << "   Use a dot for decimals: 0.5, not 0,5\n\n"
              << " HOW TO USE\n"
              << "   1. Enter data from the keyboard (option 1) or load it from a file (option 2).\n"
              << "   2. Choose option 3 to see the table. You can save it to results.txt.\n"
              << "   If a value is wrong, the program explains why and asks again.\n\n"
              << " FILE FORMAT\n"
              << "   4 numbers separated by spaces, in this order: start end step n\n"
              << "   Example: -2 4 1 5        (default file name: input.txt)\n\n"
              << " SPECIAL RESULTS IN THE TABLE\n"
              << "   not defined   x = 0: the term 1/(2x) would divide by zero\n"
              << "   too large     the value is bigger than the computer can store\n"
              << "   6.54826e+07   a big number: 6.54826 * 10^7\n";
    pause();
}

}  // namespace ftc
