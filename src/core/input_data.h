// Вхідні дані програми та їхні обмеження
#pragma once

namespace ftc {

// Найбільша кількість рядків у таблиці і найбільше n
constexpr int MAX_POINTS = 1000;
constexpr int MAX_N = 1000;

// Початок, кінець, крок і параметр n
struct InputData {
    double xStart = 0;
    double xEnd = 0;
    double dx = 0;
    int n = 0;   // верхня межа лічильників i та j у формулі (скільки разів повторюються цикли)
};

}  // namespace ftc
