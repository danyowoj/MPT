#include "Editor.h"

Editor::Editor(int base) : base(base), value("0") {}

void Editor::setBase(int p)
{
    if (p >= 2 && p <= 16)
        base = p;
}

bool Editor::isValidDigit(char c) const
{
    if (c >= '0' && c <= '9')
        return (c - '0') < base;

    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10) < base;

    return false;
}

void Editor::addDigit(char c)
{
    if (!isValidDigit(c))
        return;

    if (value == "0")
        value = "";

    value += c;
}

void Editor::addPoint()
{
    if (value.find('.') == std::string::npos)
        value += '.';
}

void Editor::toggleSign()
{
    if (value[0] == '-')
        value.erase(0, 1);
    else
        value = "-" + value;
}

void Editor::backspace()
{
    if (value.length() <= 1)
    {
        value = "0";
        return;
    }

    value.pop_back();

    if (value == "-" || value.empty())
        value = "0";
}

void Editor::clear() { value = "0"; }

std::string Editor::getValue() const { return value; }
