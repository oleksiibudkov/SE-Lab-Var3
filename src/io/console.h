// Робота з консоллю: заголовки розділів і зчитування значень з перевіркою формату
#pragma once

#include <string>

namespace ftc {

// Заголовок розділу в рамці
void printTitle(const std::string& title);

// Зчитує рядок; якщо ввід закінчився (Ctrl+D / Ctrl+Z), завершує програму
std::string readLine(const std::string& text);

// Чекає, поки користувач натисне Enter
void pause();

// Зчитує число; повторює запит, доки ввід не правильний
double readDouble(const std::string& text);

// Зчитує номер пункту меню
int readInt(const std::string& text);

// Питання з відповіддю y або n
bool askYesNo(const std::string& text);

}  // namespace ftc
