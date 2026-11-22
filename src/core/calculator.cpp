#include "core/calculator.h"

#include <cmath>
#include <stdexcept>

namespace ftc {

// Обчислює y за однією з двох формул; для неможливих даних кидає виняток
double calcY(double x, int n) {
    if (n <= 3) throw std::invalid_argument("parameter n must be at least 4");
    if (std::fabs(x) < 1e-9) throw std::invalid_argument("y is not defined for x = 0");

    // x < 3: 1/(2x) плюс добуток (i + x) для i = 1..n-2
    if (x < 3) {
        double product = 1;
        for (int i = 1; i <= n - 2; i++)
            product *= (i + x);
        return 1 / (2 * x) + product;
    }
    // x >= 3: добуток по j від суми по i виразу (i + 1/(4j^2))
    double mlt = 1;
    for (int j = 1; j <= n; j++) {
        double sum = 0;
        for (int i = 1; i <= n + 3; i++)
            sum += i + 1.0 / (4.0 * j * j);
        mlt *= sum;
    }
    return mlt;
}

}  // namespace ftc
