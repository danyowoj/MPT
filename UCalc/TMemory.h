#ifndef TMEMORY_H
#define TMEMORY_H

#include <string>
#include "TANumber.h"

class TMemory
{
private:
    TANumber *fMem; // указатель на хранящееся число (0, если память выключена)
    bool fIsOn;     // состояние памяти: true - включена, false - выключена

public:
    // Конструктор: инициализирует память копией числа n, состояние выключено
    TMemory(const TANumber &n);
    // Деструктор: освобождает память, занятую fMem
    ~TMemory();

    // Операция "Записать" (Store): сохраняет копию n, память включается
    void store(const TANumber &n);
    // Операция "Добавить" (Add): прибавляет n к текущему значению памяти, если память включена
    void add(const TANumber &n);
    // Операция "Очистить" (Clear): удаляет хранимое число, память выключается
    void clear();
    // Операция "Восстановить" (Restore): возвращает копию хранящегося числа, если память включена,
    // иначе возвращает 0 (нужно проверять указатель)
    TANumber *restore() const;

    // Свойство: включена ли память?
    bool isOn() const { return fIsOn; }
    // Получить строковое представление состояния памяти
    std::string getStateString() const;

    // Запрещаем копирование во избежание двойного удаления
    TMemory(const TMemory &) = delete;
    TMemory &operator=(const TMemory &) = delete;
};

#endif
