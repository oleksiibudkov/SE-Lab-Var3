#include "io/console.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace ftc {

void printTitle(const std::string& title) {
    std::cout << "\n==================================================\n"
              << "  " << title << "\n"
              << "==================================================\n";
}

std::string readLine(const std::string& text) {
    std::cout << text;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\n Input ended. Goodbye!\n";
        std::exit(0);
    }
    return line;
}

void pause() {
    readLine("\n Press Enter to continue...");
}

double readDouble(const std::string& text) {
    while (true) {
        std::string line = readLine(text);
        std::istringstream in(line);
        double value;
        if (in >> value && (in >> std::ws).eof() && std::isfinite(value)) return value;
        if (line.empty()) std::cout << "  Nothing was entered. ";
        else std::cout << "  \"" << line << "\" is not a number. ";
        std::cout << "Please type a number, for example -2 or 0.5 (use a dot, not a comma).\n";
    }
}

int readInt(const std::string& text) {
    while (true) {
        std::string line = readLine(text);
        std::istringstream in(line);
        int value;
        if (in >> value && (in >> std::ws).eof()) return value;
        if (line.empty()) std::cout << "  Nothing was entered. ";
        else std::cout << "  \"" << line << "\" is not a menu option. ";
        std::cout << "Please type a number from 0 to 4.\n";
    }
}

bool askYesNo(const std::string& text) {
    while (true) {
        std::string answer = readLine(text);
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "  Please type y for yes or n for no.\n";
    }
}

}  // namespace ftc
