#include "UEditor.h"
#include <cctype>

std::string TEditor::toggleSign()
{
    if (m_str.empty())
        return m_str;
    if (m_str[0] == '-')
        m_str = m_str.substr(1);
    else
        m_str = "-" + m_str;
    return m_str;
}

std::string TEditor::addDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return m_str;
    m_str += static_cast<char>('0' + digit);
    return m_str;
}

std::string TEditor::addWholeSeparator()
{
    m_str += '|';
    return m_str;
}

std::string TEditor::addSlash()
{
    m_str += '/';
    return m_str;
}

std::string TEditor::backspace()
{
    if (!m_str.empty())
        m_str.pop_back();
    return m_str;
}

std::string TEditor::clear()
{
    m_str = "0/1";
    return m_str;
}
