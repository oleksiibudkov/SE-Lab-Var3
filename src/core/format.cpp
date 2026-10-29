#include "core/format.h"

#include <sstream>

namespace ftc {

std::string num(double value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

std::string dataText(const InputData& d) {
    return "x from " + num(d.xStart) + " to " + num(d.xEnd) + ", step " + num(d.dx)
           + ", n = " + std::to_string(d.n);
}

}  // namespace ftc
