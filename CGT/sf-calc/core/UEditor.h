#ifndef UEDITOR_H
#define UEDITOR_H

#include <string>

/**
 * @brief Класс "Редактор простых дробей"
 *
 * Отвечает за посимвольный ввод, хрнанеие и редактирование строкового
 * представления простой дроби. Начальное значение - "0/1".
 */
class TEditor
{
private:
    std::string m_string; // текущая редактируемая строка

public:
    // Константы
    static const std::string DEFAULT_ZERO;
    static const char SEPARATOR;
    static const char MIXED_SEPARATOR;

    // Конструктор
    TEditor();

    // Основные методы редактирования
    bool isZero() const;

    std::string toggleSign();
    std::string addDigit(int digit);
    std::string addZero();
    std::string backspace();
    std::string clear();

    // Чтение / запись строки
    std::string getString() const;
    void setString(const std::string &s);

    // Универсальный метод редактирования
    std::string edit(int cmd);
};

#endif // UEDITOR_H
