#pragma once

#include <string>

class Editor
{
public:
    Editor();

    // Текущее редактируемое число
    std::string getNumber() const;

    // Добавить цифру (0-15)
    void addDigit(int digit);

    // Добавить ноль
    void addZero();

    // Добавить разделитель целой и дробной части (точку)
    void addDelim();

    // Удалить последний символ
    void backspace();

    // Сбросить число в "0"
    void clear();

    // Количество разрядов дробной части
    int accuracy() const;

    /**
     * Выполнить команду по номеру.
     * 0-15: addDigit
     * 16: addDelim
     * 17: backspace
     * 18: clear
     */
    std::string doCommand(int cmd);

private:
    std::string number;                // хранимое значение
    static const char DELIM = '.';     // разделитель
    static const std::string ZERO;     // строковый ноль
    bool hasDelim() const;             // проверка наличия раздеителя
    char digitToChar(int digit) const; // преобразование цифры в символ
};
