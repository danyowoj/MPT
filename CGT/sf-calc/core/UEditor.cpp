#include "UEditor.h"
#include <cctype>

const std::string TEditor::DEFAULT_ZERO = "0/1";
const char TEditor::SEPARATOR = '/';
const char TEditor::MIXED_SEPARATOR = '|';

TEditor::TEditor() : m_string(DEFAULT_ZERO) {}

#pragma region Основные методы редактирования

bool TEditor::isZero() const { return m_string == DEFAULT_ZERO; }

std::string TEditor::toggleSign()
{
    if (!m_string.empty() && m_string[0] == '-')
    {
        m_string.erase(0, 1);
        if (m_string.empty())
            m_string = DEFAULT_ZERO;
    }
    else
    {
        m_string = "-" + m_string;
    }
    return m_string;
}

std::string TEditor::addDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return m_string;
    char ch = static_cast<char>('0' + digit);
    m_string.push_back(ch);
    return m_string;
}

std::string TEditor::addZero() { return addDigit(0); }

std::string TEditor::backspace()
{
    if (!m_string.empty())
    {
        m_string.pop_back();
        if (m_string.empty())
            m_string = DEFAULT_ZERO;
    }
    return m_string;
}

std::string TEditor::clear()
{
    m_string = DEFAULT_ZERO;
    return m_string;
}

#pragma endregion

#pragma region Чтение / запись строки

std::string TEditor::getString() const { return m_string; }
void TEditor::setString(const std::string &s) { m_string = s.empty() ? DEFAULT_ZERO : s; }

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
        return m_string;
    }
}
