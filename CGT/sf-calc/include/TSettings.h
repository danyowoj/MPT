#ifndef TSETTINGS_H
#define TSETTINGS_H

#include <string>

enum class DisplayFormat
{
    Fraction, // дробь
    Decimal   // десятичный, если возможно
};

enum class OperandSource
{
    Memory,
    Clipboard
};

/**
 * @brief Класс настроек пользователя.
 *
 * Отвечает за хранение, сохранение и загрузку настроек.
 */
class TSettings
{
private:
    DisplayFormat m_displayFormat;
    OperandSource m_operandSource;

public:
    TSettings();

    DisplayFormat displayFormat() const;
    OperandSource operandSource() const;

    void setDisplayFormat(DisplayFormat fmt);
    void setOperandSource(OperandSource src);

    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};

#endif // TSETTINGS_H
