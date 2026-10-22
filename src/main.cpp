// Лабораторна робота 2 "Кодування предметної області", варіант 3
// Таблиця значень функції y(x) для x від початкового до кінцевого значення з кроком dx:
//   y = 1/(2x) + P(i + x), i = 1..n-2,                    якщо x < 3
//   y = P(j = 1..n) S(i = 1..n+3) (i + 1/(4j^2)),         якщо x >= 3
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Обмеження: найбільша кількість рядків у таблиці і найбільше n
const int MAX_POINTS = 1000;
const int MAX_N = 1000;

// Вхідні дані: початок, кінець, крок і параметр n
struct Data {
    double xStart = 0;
    double xEnd = 0;
    double dx = 0;
    int n = 0;   // верхня межа лічильників i та j у формулі (скільки разів повторюються цикли)
};

// Заголовок розділу в рамці
void printTitle(string title) {
    cout << "\n==================================================\n"
         << "  " << title << "\n"
         << "==================================================\n";
}

// Число як короткий текст: -2, 0.5, 4
string num(double value) {
    ostringstream out;
    out << value;
    return out.str();
}

// Зчитує рядок; якщо ввід закінчився (Ctrl+D), завершує програму
string readLine(string text) {
    cout << text;
    string line;
    if (!getline(cin, line)) {
        cout << "\n Input ended. Goodbye!\n";
        exit(0);
    }
    return line;
}

void pause() {
    readLine("\n Press Enter to continue...");
}

// Зчитує число; повторює запит, доки ввід не правильний
double readDouble(string text) {
    while (true) {
        string line = readLine(text);
        istringstream in(line);
        double value;
        if (in >> value && (in >> ws).eof() && isfinite(value)) return value;
        if (line.empty()) cout << "  Nothing was entered. ";
        else cout << "  \"" << line << "\" is not a number. ";
        cout << "Please type a number, for example -2 or 0.5 (use a dot, not a comma).\n";
    }
}

// Зчитує номер пункту меню
int readInt(string text) {
    while (true) {
        string line = readLine(text);
        istringstream in(line);
        int value;
        if (in >> value && (in >> ws).eof()) return value;
        if (line.empty()) cout << "  Nothing was entered. ";
        else cout << "  \"" << line << "\" is not a menu option. ";
        cout << "Please type a number from 0 to 4.\n";
    }
}

bool askYesNo(string text) {
    while (true) {
        string answer = readLine(text);
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        cout << "  Please type y for yes or n for no.\n";
    }
}

// Перевірки даних (однакові для клавіатури і файлу):
// повертають "" якщо все правильно, інакше текст помилки
string checkXEnd(const Data& d) {
    if (d.xEnd < d.xStart)
        return "the end value (" + num(d.xEnd) + ") cannot be smaller than the start value ("
               + num(d.xStart) + ")";
    return "";
}

string checkDx(const Data& d) {
    if (d.dx <= 0)
        return "the step must be greater than 0, for example 0.5 or 1";
    if ((d.xEnd - d.xStart) / d.dx > MAX_POINTS)
        return "the step is too small: the table would have more than " + to_string(MAX_POINTS)
               + " rows, so choose a bigger step";
    return "";
}

string checkN(double n) {
    if (n != floor(n)) return "parameter n must be a whole number, for example 4, 5 or 10";
    if (n <= 3) return "parameter n must be at least 4";
    if (n > MAX_N) return "parameter n cannot be greater than " + to_string(MAX_N);
    return "";
}

string dataText(const Data& d) {
    return "x from " + num(d.xStart) + " to " + num(d.xEnd) + ", step " + num(d.dx)
           + ", n = " + to_string(d.n);
}

// Введення з клавіатури: кожне значення запитується, доки воно не правильне
Data enterFromKeyboard() {
    printTitle("ENTER DATA FROM KEYBOARD");
    cout << " The table shows y for every x from the start value to the end value,\n"
         << " moving by the step. Example: start -2, end 4, step 1 gives\n"
         << " x = -2, -1, 0, 1, 2, 3, 4.\n\n";
    Data d;
    string error;

    d.xStart = readDouble(" Start value of x (where the table begins, e.g. -2): ");

    do {
        d.xEnd = readDouble(" End value of x (where the table ends, not smaller than the start, e.g. 4): ");
        error = checkXEnd(d);
        if (!error.empty()) cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());

    do {
        d.dx = readDouble(" Step (how much x grows in each row, greater than 0, e.g. 1): ");
        error = checkDx(d);
        if (!error.empty()) cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());

    double n;
    do {
        n = readDouble(" Parameter n (the last number the formula counts up to: 1, 2, ... n; whole number from 4 to " + to_string(MAX_N) + ", e.g. 5): ");
        error = checkN(n);
        if (!error.empty()) cout << "  Wrong value: " << error << ". Please try again.\n";
    } while (!error.empty());
    d.n = (int)n;

    cout << "\n Data accepted: " << dataText(d) << "\n";
    pause();
    return d;
}

// Завантаження з файлу: старі дані замінюються, лише якщо весь файл правильний
bool loadFromFile(Data& data) {
    printTitle("LOAD DATA FROM FILE");
    cout << " The file must contain 4 numbers separated by spaces, in this order:\n"
         << "   start  end  step  n      (example: -2 4 1 5)\n\n";
    string fileName = readLine(" File name (just press Enter to use input.txt): ");
    if (fileName.empty()) fileName = "input.txt";

    ifstream file(fileName);
    Data d;
    double n = 0;
    string extra, error;
    if (!file)
        error = "the file was not found. Check the name and that the file is in the program folder";
    else if (!(file >> d.xStart >> d.xEnd >> d.dx >> n))
        error = "the file must contain 4 numbers: start, end, step and n";
    else if (file >> extra)
        error = "the file contains more than 4 values";
    if (error.empty()) error = checkXEnd(d);
    if (error.empty()) error = checkDx(d);
    if (error.empty()) error = checkN(n);

    if (error.empty()) {
        d.n = (int)n;
        data = d;
        cout << "\n Data loaded: " << dataText(data) << "\n";
    } else {
        cout << "\n Could not load \"" << fileName << "\": " << error << ".\n"
             << " Your previous data was not changed.\n";
    }
    pause();
    return error.empty();
}

// Обчислює y для одного значення x
double calcY(double x, int n) {
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

// Друкує таблицю на екран (cout) або у файл (ofstream)
void printTable(ostream& out, const Data& d) {
    out << "            x |                y\n"
        << " -------------+-----------------\n";
    for (int k = 0; d.xStart + k * d.dx <= d.xEnd + 1e-9; k++) {
        double x = d.xStart + k * d.dx;
        if (fabs(x) < 1e-9) x = 0;
        out << " " << setw(12) << x << " | ";

        if (x == 0) {
            out << setw(16) << "not defined" << "\n";
            continue;
        }
        double y = calcY(x, d.n);
        if (isfinite(y)) out << setw(16) << y << "\n";
        else out << setw(16) << "too large" << "\n";
    }
}

// Показує таблицю і за бажанням зберігає її у results.txt
void calculate(const Data& d, bool hasData) {
    printTitle("RESULT TABLE");
    if (!hasData) {
        cout << " There is no data yet. Please enter it first: option 1 (keyboard) or option 2 (file).\n";
        pause();
        return;
    }
    cout << " Data: " << dataText(d) << "\n\n";
    printTable(cout, d);

    if (askYesNo("\n Do you want to save this table to the file results.txt? (y/n): ")) {
        ofstream file("results.txt");
        if (file) {
            file << "Data: " << dataText(d) << "\n\n";
            printTable(file, d);
            cout << " Table saved to results.txt\n";
        } else {
            cout << " Sorry, the file results.txt could not be created.\n";
        }
    }
    pause();
}

// Розділ Help
void showHelp() {
    printTitle("HELP");
    cout << " WHAT THE PROGRAM DOES\n"
         << "   Builds a table of y(x) for x = start, start + step, start + 2*step, ... up to the end.\n\n"
         << " INPUT DATA\n"
         << "   Start value   where the table begins        any number, e.g. -2\n"
         << "   End value     where the table ends          not smaller than the start, e.g. 4\n"
         << "   Step          how much x grows in each row  greater than 0, at most "
         << MAX_POINTS << " rows\n"
         << "   Parameter n   the last number the formula counts up to (1, 2, ... n),\n"
         << "                 whole number from 4 to " << MAX_N << ". The bigger n, the bigger the results.\n"
         << "   Use a dot for decimals: 0.5, not 0,5\n\n"
         << " HOW TO USE\n"
         << "   1. Enter data from the keyboard (option 1) or load it from a file (option 2).\n"
         << "   2. Choose option 3 to see the table. You can save it to results.txt.\n"
         << "   If a value is wrong, the program explains why and asks again.\n\n"
         << " FILE FORMAT\n"
         << "   4 numbers separated by spaces, in this order: start end step n\n"
         << "   Example: -2 4 1 5        (default file name: input.txt)\n\n"
         << " SPECIAL RESULTS IN THE TABLE\n"
         << "   not defined   x = 0: the term 1/(2x) would divide by zero\n"
         << "   too large     the value is bigger than the computer can store\n"
         << "   6.54826e+07   a big number: 6.54826 * 10^7\n";
    pause();
}

// Головне меню: опис програми, поточні дані і пункти
void showMenu(const Data& d, bool hasData) {
    printTitle("FUNCTION TABLE CALCULATOR (Variant 3) - MAIN MENU");
    cout << " This program calculates the function y(x) for every x\n"
         << " from a start value to an end value with a chosen step and shows a table:\n"
         << "   y = 1/(2x) + product(i + x), i = 1..n-2                 if x < 3\n"
         << "   y = product(j = 1..n) of sum(i = 1..n+3) (i + 1/(4j^2))   if x >= 3\n"
         << "--------------------------------------------------\n";
    if (hasData) cout << " Current data: " << dataText(d) << "\n";
    else cout << " Current data: not entered yet (choose 1 or 2)\n";
    cout << "--------------------------------------------------\n"
         << " 1. Enter data from keyboard  - type the start, end, step and n\n"
         << " 2. Load data from file       - read the same 4 values from a file\n"
         << " 3. Calculate and show table  - build the table of y(x), save it if needed\n"
         << " 4. Help                      - meaning of every value, rules, examples\n"
         << " 0. Exit                      - close the program\n";
}

int main() {
    Data data;
    bool hasData = false;

    // Головне меню: повторюється, доки користувач не вибере 0
    while (true) {
        showMenu(data, hasData);
        int choice = readInt(" Choose an option (0-4): ");

        if (choice == 1) {
            data = enterFromKeyboard();
            hasData = true;
        }
        else if (choice == 2) {
            if (loadFromFile(data)) hasData = true;
        }
        else if (choice == 3) {
            calculate(data, hasData);
        }
        else if (choice == 4) {
            showHelp();
        }
        else if (choice == 0) {
            cout << " Goodbye!\n";
            return 0;
        }
        else {
            cout << "  There is no option " << choice << ". Please choose a number from 0 to 4.\n";
        }
    }
}
