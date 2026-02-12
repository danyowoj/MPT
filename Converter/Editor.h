#pragma once

#include <string>

class Editor
{
public:
    Editor(int base = 10);

    void setBase(int p);

    void addDigit(char c);
    void addPoint();
    void toggleSign();
    void backspace();
    void clear();

    std::string getValue() const;

private:
    std::string value;
    int base;

    bool isValidDigit(char c) const;
};
