#include "../include/TSettings.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>

static std::string trim(const std::string &s)
{
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

TSettings::TSettings()
    : m_displayFormat(DisplayFormat::Fraction), m_operandSource(OperandSource::Memory)
{
}

DisplayFormat TSettings::displayFormat() const
{
    return m_displayFormat;
}

OperandSource TSettings::operandSource() const
{
    return m_operandSource;
}

void TSettings::setDisplayFormat(DisplayFormat fmt)
{
    m_displayFormat = fmt;
}

void TSettings::setOperandSource(OperandSource src)
{
    m_operandSource = src;
}

void TSettings::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    file << "displayFormat=";
    switch (m_displayFormat)
    {
    case DisplayFormat::Fraction:
        file << "Fraction";
        break;
    case DisplayFormat::Decimal:
        file << "Decimal";
        break;
    }
    file << "\n";

    file << "operandSource=";
    switch (m_operandSource)
    {
    case OperandSource::Memory:
        file << "Memory";
        break;
    case OperandSource::Clipboard:
        file << "Clipboard";
        break;
    }
    file << "\n";
}

void TSettings::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return; // если файла нет, оставляем текущие настройки

    std::string line;
    while (std::getline(file, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue; // игнорируем комментарии и пустые строки

        auto eqPos = line.find('=');
        if (eqPos == std::string::npos)
            continue;

        std::string key = trim(line.substr(0, eqPos));
        std::string value = trim(line.substr(eqPos + 1));

        if (key == "displayFormat")
        {
            if (value == "Fraction")
                m_displayFormat = DisplayFormat::Fraction;
            else if (value == "Decimal")
                m_displayFormat = DisplayFormat::Decimal;
        }
        else if (key == "operandSource")
        {
            if (value == "Memory")
                m_operandSource = OperandSource::Memory;
            else if (value == "Clipboard")
                m_operandSource = OperandSource::Clipboard;
        }
    }
}
