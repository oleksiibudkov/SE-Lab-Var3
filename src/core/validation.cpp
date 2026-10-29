#include "core/validation.h"

#include <cmath>

#include "core/format.h"

namespace ftc {

std::string checkXEnd(const InputData& d) {
    if (d.xEnd < d.xStart)
        return "the end value (" + num(d.xEnd) + ") cannot be smaller than the start value ("
               + num(d.xStart) + ")";
    return "";
}

std::string checkDx(const InputData& d) {
    if (d.dx <= 0)
        return "the step must be greater than 0, for example 0.5 or 1";
    if ((d.xEnd - d.xStart) / d.dx > MAX_POINTS)
        return "the step is too small: the table would have more than " + std::to_string(MAX_POINTS)
               + " rows, so choose a bigger step";
    return "";
}

std::string checkN(double n) {
    if (n != std::floor(n)) return "parameter n must be a whole number, for example 4, 5 or 10";
    if (n <= 3) return "parameter n must be at least 4";
    if (n > MAX_N) return "parameter n cannot be greater than " + std::to_string(MAX_N);
    return "";
}

}  // namespace ftc
