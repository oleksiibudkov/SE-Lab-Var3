// Unit tests (GoogleTest) для функцій з папки src/core.
// Назви: Функція.аргументи_очікуванийРезультат, як у методичці (testedFunction_functionArguments_expectedResult).
// Біля кожного тесту вказано техніку: EP — Equivalence Partitioning, BVA — Boundary Value Analysis.
#include <gtest/gtest.h>

#include "core/calculator.h"
#include "core/data_file.h"
#include "core/validation.h"

#include <stdexcept>

using namespace ftc;

// ---------------------------------------------------------------- calcY
// Еталони пораховано вручну. Гілка 1 (x < 3): y = 1/(2x) + (1 + x)(2 + x) для n = 4.

// EP: звичайна точка першої гілки, 1/2 + 2*3 = 6.5
TEST(CalcY, x1n4_returns6_5) {
    EXPECT_NEAR(calcY(1, 4), 6.5, 1e-9);
}

// EP: від'ємний x, добуток (-1)*0 = 0, тому y = 1/(-4) = -0.25
TEST(CalcY, xMinus2n4_returnsMinus0_25) {
    EXPECT_NEAR(calcY(-2, 4), -0.25, 1e-9);
}

// EP: добуток дорівнює 0 (множник 1 + x = 0), y = 1/(-2) = -0.5
TEST(CalcY, xMinus1n4_returnsMinus0_5) {
    EXPECT_NEAR(calcY(-1, 4), -0.5, 1e-9);
}

// EP: дробовий x, 1 + 1.5*2.5 = 4.75
TEST(CalcY, x0_5n4_returns4_75) {
    EXPECT_NEAR(calcY(0.5, 4), 4.75, 1e-9);
}

// BVA: остання точка першої гілки перед межею x = 3
TEST(CalcY, x2_999n4_returns20_1577) {
    EXPECT_NEAR(calcY(2.999, 4), 20.1577232407, 1e-6);
}

// BVA: перша точка другої гілки, 29.75 * 28.4375 * 28.19444 * 28.109375
TEST(CalcY, x3n4_returns670491_25) {
    EXPECT_NEAR(calcY(3, 4), 670491.2504408, 1e-4);
}

// EP: друга гілка не залежить від x
TEST(CalcY, x4n4_equalsX3n4) {
    EXPECT_NEAR(calcY(4, 4), calcY(3, 4), 1e-9);
}

// EP: інше n, 0.5 + 2*3*4 = 24.5
TEST(CalcY, x1n5_returns24_5) {
    EXPECT_NEAR(calcY(1, 5), 24.5, 1e-9);
}

// EP, неправильний клас: x = 0, ділення на нуль
TEST(CalcY, x0n4_throwsInvalidArgument) {
    EXPECT_THROW(calcY(0, 4), std::invalid_argument);
}

// BVA: n = 3 — найбільше неправильне n
TEST(CalcY, x1n3_throwsInvalidArgument) {
    EXPECT_THROW(calcY(1, 3), std::invalid_argument);
}

// ---------------------------------------------------------------- countRows (дефект #92)

// EP: -2, -1, ..., 4 — сім рядків
TEST(CountRows, m2to4step1_returns7) {
    EXPECT_EQ(countRows(-2, 4, 1), 7);
}

// BVA: найбільша дозволена таблиця — 1000 рядків
TEST(CountRows, range999step1_returns1000) {
    EXPECT_EQ(countRows(0, 999, 1), 1000);
}

// EP: похибка ділення 0.3 / 0.1 = 2.9999999999999996 не губить останній рядок
TEST(CountRows, range0_3step0_1_returns4) {
    EXPECT_EQ(countRows(0, 0.3, 0.1), 4);
}

// BVA: start = end і дуже малий крок — один рядок, а не нескінченна таблиця
TEST(CountRows, sameStartEndTinyStep_returns1) {
    EXPECT_EQ(countRows(1, 1, 1e-300), 1);
}
