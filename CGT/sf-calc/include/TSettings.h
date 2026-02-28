#ifndef TSETTINGS_H
#define TSETTINGS_H

#include <string>
#include "UFrac.h" // для DisplayFormat

// Источник операндов
enum class OperandSource
{
    Memory,
    Clipboard
};

/**
 * @brief Класс для хранения и загрузки/сохранения настроек пользователя
 */
class TSettings
{
private:
    DisplayFormat m_displayFormat;
    OperandSource m_operandSource;

public:
    TSettings();

    // Геттеры
    DisplayFormat displayFormat() const;
    OperandSource operandSource() const;

    // Сеттеры
    void setDisplayFormat(DisplayFormat fmt);
    void setOperandSource(OperandSource src);

    // Сохранение и загрузка
    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);
};

#endif
