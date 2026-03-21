#include "CEditor.h"
#include <cctype>
#include <algorithm>

CEditor::CEditor() : TAEditor(), hasSeparator(false), isNegative(false), imNegative(false)
{
    fNumber = "0";
}

CEditor::CEditor(const CEditor &other) : TAEditor(other), hasSeparator(other.hasSeparator), isNegative(other.isNegative), imNegative(other.imNegative) {}

void CEditor::updateFlags()
{
    hasSeparator = (fNumber.find("i*") != std::string::npos);
    isNegative = (!fNumber.empty() && fNumber[0] == '-');
    // признак отрицательности мнимой части можно определить по наличию знака после "i*"
    imNegative = false;
    size_t pos = fNumber.find("i*");
    if (pos != std::string::npos && pos + 2 < fNumber.length())
    {
        char sign = fNumber[pos + 2];
        if (sign == '-')
            imNegative = true;
    }
}

std::string CEditor::addDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return fNumber;
    char d = static_cast<char>('0' + digit);
    if (!hasSeparator)
    {
        // Редактируем действительную часть
        if (fNumber == "0")
        {
            fNumber = std::string(1, d);
            isNegative = false;
        }
        else
        {
            fNumber += d;
        }
    }
    else
    {
        // Редактируем мнимую часть
        // Ищем позицию после "i*"
        size_t pos = fNumber.find("i*");
        if (pos != std::string::npos)
        {
            std::string prefix = fNumber.substr(0, pos + 2);
            std::string suffix = fNumber.substr(pos + 2);
            // Если мнимая часть "0" и это первая цифра, заменяем
            if (suffix == "0")
                suffix = "";
            suffix += d;
            fNumber = prefix + suffix;
        }
    }
    return fNumber;
}

std::string CEditor::addSign()
{
    if (!hasSeparator)
    {
        // Изменяем знак действительной части
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
    }
    else
    {
        // Изменяем знак мнимой части
        size_t pos = fNumber.find("i*");
        if (pos != std::string::npos && pos + 2 < fNumber.length())
        {
            std::string prefix = fNumber.substr(0, pos + 2);
            std::string suffix = fNumber.substr(pos + 2);
            if (suffix.empty())
                suffix = "0";
            if (suffix[0] == '-')
            {
                suffix = suffix.substr(1);
                imNegative = false;
            }
            else
            {
                suffix = "-" + suffix;
                imNegative = true;
            }
            fNumber = prefix + suffix;
        }
    }
    return fNumber;
}

std::string CEditor::addSeparator()
{
    if (hasSeparator)
        return fNumber;
    if (fNumber.empty())
        fNumber = "0";
    if (fNumber[0] == '-')
    {
        fNumber.insert(1, " + i*");
    }
    else
    {
        fNumber += " + i*";
    }
    hasSeparator = true;
    // Добавим 0 после разделителя
    fNumber += "0";
    imNegative = false;
    return fNumber;
}

std::string CEditor::backspace()
{
    if (fNumber == "0")
        return fNumber;
    if (fNumber == "-0")
    {
        fNumber = "0";
        isNegative = false;
        hasSeparator = false;
        imNegative = false;
        return fNumber;
    }
    char last = fNumber.back();
    fNumber.pop_back();
    // Если удалили последний символ и остался только разделитель или часть, то сбрасываем
    if (hasSeparator)
    {
        size_t pos = fNumber.find("i*");
        if (pos != std::string::npos && pos + 2 == fNumber.length())
        {
            // удалили всю мнимую часть, убираем разделитель
            fNumber = fNumber.substr(0, pos);
            hasSeparator = false;
            imNegative = false;
        }
    }
    if (fNumber.empty() || fNumber == "-")
    {
        fNumber = "0";
        isNegative = false;
        hasSeparator = false;
        imNegative = false;
    }
    else
    {
        updateFlags();
    }
    return fNumber;
}

std::string CEditor::clear()
{
    fNumber = "0";
    hasSeparator = false;
    isNegative = false;
    imNegative = false;
    return fNumber;
}

bool CEditor::isZero() const
{
    // Комплексное число равно нулю, если обе части нулевые
    std::string s = fNumber;
    if (s.empty())
        return true;
    size_t pos = s.find("i*");
    if (pos == std::string::npos)
    {
        // только действительная часть
        s = (s[0] == '-') ? s.substr(1) : s;
        for (char c : s)
            if (c != '0')
                return false;
        return true;
    }
    std::string rePart = s.substr(0, pos);
    if (rePart[0] == '-')
        rePart = rePart.substr(1);
    for (char c : rePart)
        if (c != '0')
            return false;
    std::string imPart = s.substr(pos + 2);
    if (imPart[0] == '-')
        imPart = imPart.substr(1);
    for (char c : imPart)
        if (c != '0')
            return false;
    return true;
}

std::string CEditor::getString() const
{
    return fNumber;
}

void CEditor::setString(const std::string &str)
{
    fNumber = str;
    updateFlags();
}

TANumber *CEditor::createNumber() const
{
    return new TComplex(fNumber);
}
