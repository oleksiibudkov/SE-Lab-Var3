#include "io/table_printer.h"

#include <cmath>
#include <iomanip>

#include "core/calculator.h"

namespace ftc {

void printTable(std::ostream& out, const InputData& d) {
    out << "            x |                y\n"
        << " -------------+-----------------\n";
    for (int k = 0; d.xStart + k * d.dx <= d.xEnd + 1e-9; k++) {
        double x = d.xStart + k * d.dx;
        if (std::fabs(x) < 1e-9) x = 0;
        out << " " << std::setw(12) << x << " | ";

        if (x == 0) {
            out << std::setw(16) << "not defined" << "\n";
            continue;
        }
        double y = calcY(x, d.n);
        if (std::isfinite(y)) out << std::setw(16) << y << "\n";
        else out << std::setw(16) << "too large" << "\n";
    }
}

}  // namespace ftc
