#ifndef UEDITOR_H
#define UEDITOR_H

#include <string>

class TEditor
{
private:
    std::string m_str;

public:
    TEditor() : m_str("0/1") {}

    bool isZero() const { return m_str == "0/1"; }
    std::string getString() const { return m_str; }
    void setString(const std::string &s) { m_str = s; }

    std::string toggleSign();
    std::string addDigit(int digit);
    std::string addZero() { return addDigit(0); }
    std::string addWholeSeparator();
    std::string addSlash();          // новый метод для добавления '/'
    std::string backspace();
    std::string clear();

    std::string edit(int cmd);
};

#endif
