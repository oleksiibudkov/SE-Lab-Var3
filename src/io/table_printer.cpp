#include "io/table_printer.h"

#include <cmath>
#include <iomanip>
#include <stdexcept>

#include "core/calculator.h"
#include "core/validation.h"

namespace ftc {

// Друкує рядок для кожного x; рядок x = 0 отримує виняток від calcY
void printTable(std::ostream& out, const InputData& d) {
    out << "            x |                y\n"
        << " -------------+-----------------\n";
    int rows = countRows(d.xStart, d.xEnd, d.dx);
    for (int k = 0; k < rows; k++) {
        double x = d.xStart + k * d.dx;
        if (std::fabs(x) < 1e-9) x = 0;
        out << " " << std::setw(12) << x << " | ";

        try {
            double y = calcY(x, d.n);
            if (std::isfinite(y)) out << std::setw(16) << y << "\n";
            else out << std::setw(16) << "too large" << "\n";
        } catch (const std::invalid_argument&) {  // x = 0: 1/(2x) не визначено
            out << std::setw(16) << "not defined" << "\n";
        }
    }
}

}  // namespace ftc
