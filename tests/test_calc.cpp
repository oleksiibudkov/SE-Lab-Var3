// Unit tests (GoogleTest) для функцій з папки src/core.
// Назви: Функція.аргументи_очікуванийРезультат, як у методичці (testedFunction_functionArguments_expectedResult).
// Біля кожного тесту вказано техніку: EP — Equivalence Partitioning, BVA — Boundary Value Analysis.
#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

#include "core/calculator.h"
#include "core/data_file.h"
#include "core/validation.h"

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

// ---------------------------------------------------------------- checkValidParams (xStart, xEnd, dx, n)

// EP: правильні дані
TEST(CheckValidParams, validData_noThrow) {
    EXPECT_NO_THROW(checkValidParams(-2, 4, 0.5, 10));
}

// EP, неправильний клас: кінець менший за початок
TEST(CheckValidParams, xEndLessThanXStart_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(4, -2, 1, 4), std::invalid_argument);
}

// BVA: кінець дорівнює початку — таблиця з одного рядка
TEST(CheckValidParams, xEndEqualsXStart_noThrow) {
    EXPECT_NO_THROW(checkValidParams(1, 1, 1, 4));
}

// BVA: крок 0 — межа між правильним і неправильним кроком
TEST(CheckValidParams, dx0_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, 4, 0, 4), std::invalid_argument);
}

// EP, неправильний клас: від'ємний крок
TEST(CheckValidParams, dxNegative_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, 4, -1, 4), std::invalid_argument);
}

// BVA: n = 3 — найбільше неправильне n
TEST(CheckValidParams, n3_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, 4, 1, 3), std::invalid_argument);
}

// BVA: n = 4 — найменше правильне n
TEST(CheckValidParams, n4_noThrow) {
    EXPECT_NO_THROW(checkValidParams(-2, 4, 1, 4));
}

// EP, неправильний клас: дробове n з файлу
TEST(CheckValidParams, n4_5_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, 4, 1, 4.5), std::invalid_argument);
}

// BVA: n = 1001 — перше n більше за MAX_N
TEST(CheckValidParams, n1001_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, 4, 1, 1001), std::invalid_argument);
}

// BVA: 1000 рядків — ще можна (дефект #92)
TEST(CheckValidParams, rows1000_noThrow) {
    EXPECT_NO_THROW(checkValidParams(0, 999, 1, 4));
}

// BVA: 1001 рядок — уже ні (дефект #92: раніше пропускалось)
TEST(CheckValidParams, rows1001_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(0, 1000, 1, 4), std::invalid_argument);
}

// EP, неправильний клас: нескінченне значення
TEST(CheckValidParams, xEndInfinite_throwsInvalidArgument) {
    EXPECT_THROW(checkValidParams(-2, std::numeric_limits<double>::infinity(), 1, 4), std::invalid_argument);
}

// ---------------------------------------------------------------- parseDouble

// EP: число з пробілами навколо
TEST(ParseDouble, text2_5_returns2_5) {
    EXPECT_DOUBLE_EQ(parseDouble(" 2.5 "), 2.5);
}

// EP, неправильний клас: літери
TEST(ParseDouble, textAbc_throwsInvalidArgument) {
    EXPECT_THROW(parseDouble("abc"), std::invalid_argument);
}

// EP, неправильний клас: порожній рядок
TEST(ParseDouble, emptyText_throwsInvalidArgument) {
    EXPECT_THROW(parseDouble(""), std::invalid_argument);
}

// EP, неправильний клас: число, після якого є зайві символи
TEST(ParseDouble, text2_5x_throwsInvalidArgument) {
    EXPECT_THROW(parseDouble("2.5x"), std::invalid_argument);
}

// EP, неправильний клас: нескінченність
TEST(ParseDouble, textInf_throwsInvalidArgument) {
    EXPECT_THROW(parseDouble("inf"), std::invalid_argument);
}

// ---------------------------------------------------------------- parseInt

// EP: ціле число
TEST(ParseInt, text4_returns4) {
    EXPECT_EQ(parseInt("4"), 4);
}

// EP, неправильний клас: дробове число
TEST(ParseInt, text4_5_throwsInvalidArgument) {
    EXPECT_THROW(parseInt("4.5"), std::invalid_argument);
}

// EP, неправильний клас: число з літерами
TEST(ParseInt, text5abc_throwsInvalidArgument) {
    EXPECT_THROW(parseInt("5abc"), std::invalid_argument);
}

// EP, неправильний клас: порожній рядок
TEST(ParseInt, emptyText_throwsInvalidArgument) {
    EXPECT_THROW(parseInt(""), std::invalid_argument);
}

// ---------------------------------------------------------------- loadFromFile

// Допоміжна функція: створює тимчасовий файл з текстом і повертає його ім'я
static std::string writeTempFile(const std::string& name, const std::string& text) {
    std::string path = ::testing::TempDir() + name;
    std::ofstream(path) << text;
    return path;
}

// EP: правильний файл — значення завантажено
TEST(LoadFromFile, validFile_loadsValues) {
    std::string path = writeTempFile("valid.txt", "-2 4 1 4\n");
    double xStart = 0, xEnd = 0, dx = 0;
    int n = 0;
    loadFromFile(path, xStart, xEnd, dx, n);
    EXPECT_DOUBLE_EQ(xStart, -2);
    EXPECT_DOUBLE_EQ(xEnd, 4);
    EXPECT_DOUBLE_EQ(dx, 1);
    EXPECT_EQ(n, 4);
    std::remove(path.c_str());
}

// EP, неправильний клас: файлу немає
TEST(LoadFromFile, missingFile_throwsRuntimeError) {
    double xStart = 0, xEnd = 0, dx = 0;
    int n = 0;
    EXPECT_THROW(loadFromFile("no_such_file.txt", xStart, xEnd, dx, n), std::runtime_error);
}

// EP, неправильний клас: у файлі лише 3 числа
TEST(LoadFromFile, threeValues_throwsInvalidArgument) {
    std::string path = writeTempFile("three.txt", "-2 4 1\n");
    double xStart = 0, xEnd = 0, dx = 0;
    int n = 0;
    EXPECT_THROW(loadFromFile(path, xStart, xEnd, dx, n), std::invalid_argument);
    std::remove(path.c_str());
}

// EP, неправильний клас: у файлі зайве п'яте значення
TEST(LoadFromFile, fiveValues_throwsInvalidArgument) {
    std::string path = writeTempFile("five.txt", "1 2 1 5 9\n");
    double xStart = 0, xEnd = 0, dx = 0;
    int n = 0;
    EXPECT_THROW(loadFromFile(path, xStart, xEnd, dx, n), std::invalid_argument);
    std::remove(path.c_str());
}

// EP: неправильний файл не змінює попередні дані
TEST(LoadFromFile, wrongN_keepsOldValues) {
    std::string path = writeTempFile("bad_n.txt", "0 1 1 4.5\n");
    double xStart = -2, xEnd = 4, dx = 1;
    int n = 5;
    EXPECT_THROW(loadFromFile(path, xStart, xEnd, dx, n), std::invalid_argument);
    EXPECT_DOUBLE_EQ(xStart, -2);
    EXPECT_DOUBLE_EQ(xEnd, 4);
    EXPECT_DOUBLE_EQ(dx, 1);
    EXPECT_EQ(n, 5);
    std::remove(path.c_str());
}
