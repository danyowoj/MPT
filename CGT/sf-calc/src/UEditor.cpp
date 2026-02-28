#include "../include/UEditor.h"
#include "../include/UFrac.h" // для проверки корректности строки
#include <cctype>

static bool isValidFractionString(const std::string &s)
{
    if (s.empty())
        return false;

    try
    {
        TFrac tmp(s); // попытка создать дробь
        return true;
    }
    catch (...)
    {
        return false;
    }
}

#pragma region Основные методы редактирования

std::string TEditor::toggleSign()
{
    std::string candidate;
    if (!m_str.empty() && m_str[0] == '-')
    {
        candidate = m_str.substr(1);
        if (candidate.empty())
            candidate = "0/1";
    }
    else
    {
        candidate = "-" + m_str;
    }
    if (isValidFractionString(candidate))
        m_str = candidate;

    return m_str;
}

std::string TEditor::addDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return m_str;
    char ch = static_cast<char>('0' + digit);
    std::string candidate = m_str + ch;
    if (isValidFractionString(candidate))
    {
        m_str = candidate;
    }
    return m_str;
}

std::string TEditor::backspace()
{
    if (m_str.empty())
        return m_str;
    std::string candidate = m_str.substr(0, m_str.size() - 1);
    if (candidate.empty())
        candidate = "0/1";
    if (isValidFractionString(candidate))
        m_str = candidate;
    return m_str;
}

std::string TEditor::clear()
{
    m_str = "0/1";
    return m_str;
}

#pragma endregion

std::string TEditor::edit(int cmd)
{
    if (cmd >= 0 && cmd <= 9)
        return addDigit(cmd);

    switch (cmd)
    {
    case -1:
        return toggleSign();
    case -2:
        return backspace();
    case -3:
        return clear();
    default:
        return m_str;
    }
}
