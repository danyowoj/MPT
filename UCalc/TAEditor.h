#ifndef TAEDITOR_H
#define TAEDITOR_H

#include <string>
#include "TANumber.h"
#include "TPNumber.h"
#include "TFrac.h"
#include "TComplex.h"

// Константы команд редактора
const unsigned cZero = 0;
const unsigned cOne = 1;
const unsigned cTwo = 2;
const unsigned cThree = 3;
const unsigned cFour = 4;
const unsigned cFive = 5;
const unsigned cSix = 6;
const unsigned cSeven = 7;
const unsigned cEight = 8;
const unsigned cNine = 9;
const unsigned cSign = 10;      // изменить знак
const unsigned cSeparator = 11; // добавить разделитель
const unsigned cBackspace = 12; // удалить последний символ
const unsigned cClear = 13;     // очистить (установить "0")

class TAEditor
{
protected:
    std::string fNumber; // строковое представление редактируемого числа
public:
    TAEditor() : fNumber("0") {}
    virtual ~TAEditor() {}

    // Основные операции редактирования
    virtual std::string addDigit(int digit) = 0; // добавить цифру 0..9 или A-F
    virtual std::string addSign() = 0;           // добавить/убрать знак
    virtual std::string addSeparator() = 0;      // добавить разделитель
    virtual std::string backspace() = 0;         // удалить последний символ
    virtual std::string clear() = 0;             // установить "0"

    // Проверка
    virtual bool isZero() const = 0; // равно ли число нулю?

    // Доступ к строке
    virtual std::string getString() const = 0;
    virtual void setString(const std::string &str) = 0;

    // Универсальный метод выполнения команды
    std::string executeCommand(unsigned cmd);

    // Создать число из текущей строки
    virtual TANumber *createNumber() const = 0;
};

#endif
