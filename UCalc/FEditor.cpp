#include "FEditor.h"
#include <cctype>
#include <algorithm>

FEditor::FEditor() : TAEditor(), hasSeparator(false), isNegative(false)
{
    fNumber = "0";
}

FEditor::FEditor(const FEditor &other) : TAEditor(other), hasSeparator(other.hasSeparator), isNegative(other.isNegative) {}

void FEditor::updateFlags()
{
    hasSeparator = (fNumber.find('/') != std::string::npos);
    isNegative = (!fNumber.empty() && fNumber[0] == '-');
}

std::string FEditor::addDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return fNumber;
    char d = static_cast<char>('0' + digit);
    // Если число "0" и нет разделителя, заменяем на цифру
    if (fNumber == "0" && !hasSeparator)
    {
        fNumber = std::string(1, d);
        isNegative = false;
        return fNumber;
    }
    // Если уже есть разделитель, добавляем в знаменатель
    fNumber += d;
    return fNumber;
}

std::string FEditor::addSign()
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
    return fNumber;
}

std::string FEditor::addSeparator()
{
    if (hasSeparator)
        return fNumber; // уже есть разделитель
    if (fNumber.empty())
        fNumber = "0";
    if (fNumber[0] == '-')
    {
        fNumber.insert(1, "/");
    }
    else
    {
        fNumber += "/";
    }
    hasSeparator = true;
    return fNumber;
}

std::string FEditor::backspace()
{
    if (fNumber == "0")
        return fNumber;
    if (fNumber == "-0")
    {
        fNumber = "0";
        isNegative = false;
        hasSeparator = false;
        return fNumber;
    }
    char last = fNumber.back();
    fNumber.pop_back();
    if (last == '/')
        hasSeparator = false;
    if (fNumber.empty() || fNumber == "-")
    {
        fNumber = "0";
        isNegative = false;
        hasSeparator = false;
    }
    else
    {
        updateFlags();
    }
    return fNumber;
}

std::string FEditor::clear()
{
    fNumber = "0";
    hasSeparator = false;
    isNegative = false;
    return fNumber;
}

bool FEditor::isZero() const
{
    // Простая дробь равна нулю, если числитель нуль
    if (fNumber.empty())
        return true;
    std::string s = fNumber;
    if (s[0] == '-')
        s = s.substr(1);
    size_t slash = s.find('/');
    if (slash == std::string::npos)
    {
        // без разделителя – целое число
        for (char c : s)
        {
            if (c != '0')
                return false;
        }
        return true;
    }
    // проверим числитель
    std::string numPart = s.substr(0, slash);
    for (char c : numPart)
    {
        if (c != '0')
            return false;
    }
    return true;
}

std::string FEditor::getString() const
{
    return fNumber;
}

void FEditor::setString(const std::string &str)
{
    fNumber = str;
    updateFlags();
}

TANumber *FEditor::createNumber() const
{
    return new TFrac(fNumber);
}
