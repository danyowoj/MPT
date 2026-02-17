#include "Editor.h"
#include <algorithm>

const std::string Editor::ZERO = "0";

Editor::Editor() : number(ZERO) {}

std::string Editor::getNumber() const { return number.empty() ? ZERO : number; }

void Editor::addDigit(int digit)
{
    if (digit < 0 || digit > 15)
        return;

    char ch = digitToChar(digit);

    // Если текущее число "0" и добавляется не ноль – заменить "0" на цифру
    if (number == ZERO && digit != 0)
        number = std::string(1, ch);
    else
        number.push_back(ch);
}

void Editor::addZero() { addDigit(0); }

void Editor::addDelim()
{
    if (hasDelim())
        return;

    // Если число пусто или "0", добавляем "0."
    if (number.empty() || number == ZERO)
        number = "0" + std::string(1, DELIM);
    else
        number.push_back(DELIM);
}

void Editor::backspace()
{
    if (number.empty())
        return;

    number.pop_back();

    // После удаления пустая строка заменяется на "0"
    if (number.empty())
        number = ZERO;
}

void Editor::clear() { number = ZERO; }

int Editor::accuracy() const
{
    size_t dotPos = number.find(DELIM);
    if (dotPos == std::string::npos)
        return 0;
    return static_cast<int>(number.length() - dotPos - 1);
}

std::string Editor::doCommand(int cmd)
{
    if (cmd >= 0 && cmd <= 15)
        addDigit(cmd);
    else if (cmd == 16)
        addDelim();
    else if (cmd == 17)
        backspace();
    else if (cmd == 18)
        clear();

    return getNumber();
}

bool Editor::hasDelim() const { return number.find(DELIM) != std::string::npos; }

char Editor::digitToChar(int digit) const
{
    if (digit >= 0 && digit <= 9)
        return static_cast<char>('0' + digit);
    if (digit >= 10 && digit <= 15)
        return static_cast<char>('A' + (digit - 10));
    return '0';
}

void Editor::toggleSign()
{
    if (number.empty() || number == "0")
        return;

    if (number[0] == '-') {
        number.erase(0, 1); // убираем минус
    } else {
        number.insert(0, 1, '-'); // добавляем минус в начало
    }
}
