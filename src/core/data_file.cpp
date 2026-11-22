#include "core/data_file.h"

#include <fstream>
#include <stdexcept>

#include "core/validation.h"

namespace ftc {

// Читає і перевіряє файл; результат записує у змінні лише в кінці
void loadFromFile(const std::string& fileName, double& xStart, double& xEnd, double& dx, int& n) {
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("the file was not found. Check the name and that the file is in the program folder");

    double start, end, step, nValue;
    if (!(file >> start >> end >> step >> nValue))
        throw std::invalid_argument("the file must contain 4 numbers: start, end, step and n");
    std::string extra;
    if (file >> extra)
        throw std::invalid_argument("the file contains more than 4 values");

    checkValidParams(start, end, step, nValue);  // кидає std::invalid_argument

    xStart = start;
    xEnd = end;
    dx = step;
    n = static_cast<int>(nValue);
}

}  // namespace ftc
