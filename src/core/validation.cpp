#include "core/validation.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "core/format.h"
#include "core/input_data.h"

namespace ftc {

// Перевіряє, що кінець діапазону не менший за початок
void checkRange(double xStart, double xEnd) {
    if (xEnd < xStart)
        throw std::invalid_argument("the end value (" + num(xEnd) + ") cannot be smaller than the start value ("
                                    + num(xStart) + ")");
}

// Кількість кроків між xStart і xEnd; 1e-9 прощає похибку ділення, напр. 0.3 / 0.1 = 2.9999999999999996
static double stepsInRange(double xStart, double xEnd, double dx) {
    return (xEnd - xStart) / dx + 1e-9;
}

// Рахує рядки таблиці; цей самий лічильник використовує і друк таблиці
int countRows(double xStart, double xEnd, double dx) {
    return static_cast<int>(std::floor(stepsInRange(xStart, xEnd, dx))) + 1;
}

// Перевіряє, що крок додатний і таблиця не буде занадто довгою
void checkStep(double xStart, double xEnd, double dx) {
    if (dx <= 0)
        throw std::invalid_argument("the step must be greater than 0, for example 0.5 or 1");
    if (stepsInRange(xStart, xEnd, dx) >= MAX_POINTS)  // >= 1000 кроків означає > 1000 рядків
        throw std::invalid_argument("the step is too small: the table would have more than "
                                    + std::to_string(MAX_POINTS) + " rows, so choose a bigger step");
}

// Перевіряє, що n ціле і лежить у межах 4..MAX_N
void checkN(double n) {
    if (n != std::floor(n)) throw std::invalid_argument("parameter n must be a whole number, for example 4, 5 or 10");
    if (n <= 3) throw std::invalid_argument("parameter n must be at least 4");
    if (n > MAX_N) throw std::invalid_argument("parameter n cannot be greater than " + std::to_string(MAX_N));
}

// Перевіряє всі дані разом, у тому ж порядку, що й при введенні з клавіатури
void checkValidParams(double xStart, double xEnd, double dx, double n) {
    if (!std::isfinite(xStart) || !std::isfinite(xEnd) || !std::isfinite(dx) || !std::isfinite(n))
        throw std::invalid_argument("all values must be finite numbers");
    checkRange(xStart, xEnd);
    checkStep(xStart, xEnd, dx);
    checkN(n);
}

// Перетворює текст на double або кидає виняток з поясненням
double parseDouble(const std::string& text) {
    if (text.empty()) throw std::invalid_argument("Nothing was entered.");
    std::istringstream in(text);
    double value;
    if (in >> value && (in >> std::ws).eof() && std::isfinite(value)) return value;
    throw std::invalid_argument("\"" + text + "\" is not a number.");
}

// Перетворює текст на int або кидає виняток з поясненням
int parseInt(const std::string& text) {
    if (text.empty()) throw std::invalid_argument("Nothing was entered.");
    std::istringstream in(text);
    int value;
    if (in >> value && (in >> std::ws).eof()) return value;
    throw std::invalid_argument("\"" + text + "\" is not a whole number.");
}

}  // namespace ftc
