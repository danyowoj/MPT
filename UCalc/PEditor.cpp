#include "PEditor.h"
#include <cctype>
#include <algorithm>

static const std::string digits = "0123456789ABCDEF";

PEditor::PEditor(int b) : TAEditor(), base(b), separatorUsed(false), isNegative(false)
{
    if (base < 2)
        base = 2;
    if (base > 16)
        base = 16;
    fNumber = "0";
}

PEditor::PEditor(const PEditor &other) : TAEditor(other), base(other.base), separatorUsed(other.separatorUsed), isNegative(other.isNegative) {}

bool PEditor::isValidDigit(char c) const
{
    c = std::toupper(c);
    size_t pos = digits.find(c);
    if (pos == std::string::npos)
        return false;
    return pos < (size_t)base;
}

std::string PEditor::normalize() const
{
    // Убираем ведущие нули, но оставляем хотя бы один ноль
    std::string s = fNumber;
    if (s.empty())
        return "0";
    bool neg = (s[0] == '-');
    if (neg)
        s = s.substr(1);
    size_t dot = s.find('.');
    std::string intPart = (dot == std::string::npos) ? s : s.substr(0, dot);
    std::string fracPart = (dot == std::string::npos) ? "" : s.substr(dot + 1);
    // Удаляем ведущие нули целой части
    size_t i = 0;
    while (i + 1 < intPart.length() && intPart[i] == '0')
        ++i;
    intPart = intPart.substr(i);
    if (intPart.empty())
        intPart = "0";
    // Удаляем конечные нули дробной части
    while (!fracPart.empty() && fracPart.back() == '0')
        fracPart.pop_back();
    s = intPart;
    if (!fracPart.empty())
        s += "." + fracPart;
    if (neg)
        s = "-" + s;
    return s;
}

std::string PEditor::addDigit(int digit)
{
    if (digit < 0 || digit >= base)
        return fNumber;
    char d = digits[digit];
    // Если число "0" и это не часть дробной части, заменяем на цифру
    if (fNumber == "0" && !separatorUsed)
    {
        fNumber = std::string(1, d);
        isNegative = false;
        return fNumber;
    }
    // Если нет разделителя, добавляем цифру в конец
    if (!separatorUsed)
    {
        fNumber += d;
    }
    else
    {
        // Если разделитель уже есть, добавляем в дробную часть
        fNumber += d;
    }
    return normalize();
}

std::string PEditor::addSign()
{
    if (fNumber == "0")
        return fNumber;
    if (fNumber[0] == '-')
    {
        fNumber = fNumber.substr(1);
        isNegative = false;
    }
    else
    {
        fNumber = "-" + fNumber;
        isNegative = true;
    }
    return normalize();
}

std::string PEditor::addSeparator()
{
    if (separatorUsed)
        return fNumber; // уже есть разделитель
    if (fNumber.empty())
        fNumber = "0";
    // Если строка начинается с минуса, добавляем точку после него
    if (fNumber[0] == '-')
    {
        fNumber.insert(1, ".");
    }
    else
    {
        fNumber += ".";
    }
    separatorUsed = true;
    return normalize();
}

std::string PEditor::backspace()
{
    if (fNumber == "0")
        return fNumber;
    if (fNumber == "-0")
    {
        fNumber = "0";
        isNegative = false;
        separatorUsed = false;
        return fNumber;
    }
    // Удаляем последний символ
    char last = fNumber.back();
    fNumber.pop_back();
    if (last == '.')
        separatorUsed = false;
    // Если после удаления строка пустая или только минус -> устанавливаем "0"
    if (fNumber.empty() || fNumber == "-")
    {
        fNumber = "0";
        isNegative = false;
        separatorUsed = false;
    }
    else
    {
        // Проверим, есть ли точка после удаления
        separatorUsed = (fNumber.find('.') != std::string::npos);
        isNegative = (fNumber[0] == '-');
    }
    return normalize();
}

std::string PEditor::clear()
{
    fNumber = "0";
    separatorUsed = false;
    isNegative = false;
    return fNumber;
}

bool PEditor::isZero() const
{
    std::string s = fNumber;
    if (s.empty())
        return true;
    if (s[0] == '-')
        s = s.substr(1);
    size_t dot = s.find('.');
    std::string intPart = (dot == std::string::npos) ? s : s.substr(0, dot);
    for (char c : intPart)
    {
        if (c != '0')
            return false;
    }
    return true;
}

std::string PEditor::getString() const
{
    return normalize();
}

void PEditor::setString(const std::string &str)
{
    fNumber = str;
    // Обновляем флаги
    separatorUsed = (fNumber.find('.') != std::string::npos);
    isNegative = (!fNumber.empty() && fNumber[0] == '-');
    // Приведение к нормальному виду
    fNumber = normalize();
}

void PEditor::setBase(int b)
{
    if (b >= 2 && b <= 16)
    {
        base = b;
        clear(); // сбрасываем ввод
    }
}
