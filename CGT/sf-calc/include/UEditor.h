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
    std::string m_str;

public:
    TEditor() : m_str("0/1") {}

    bool isZero() const { return m_str == "0/1"; }

    std::string getString() const { return m_str; }
    void setString(const std::string &s) { m_str = s.empty() ? "0/1" : s; }

    // Основные операции редактирования: возвращают новую строку, если она корректна,
    // иначе возвращают старую строку.

    std::string toggleSign();
    std::string addDigit(int digit);
    std::string addZero() { return addDigit(0); }
    std::string backspace();
    std::string clear();

    // Универсальный метод редактирования
    std::string edit(int cmd);
};

#endif // UEDITOR_H
