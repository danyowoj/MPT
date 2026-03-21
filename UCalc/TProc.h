#ifndef TPROC_H
#define TPROC_H

#include <string>
#include "TANumber.h"
// Операции (с двумя операндами)
enum TOprtn
{
    None,
    Add,
    Sub,
    Mul,
    Dvd
};
// Функции (с одним операдом)
enum TFunc
{
    Rev,
    Sqr
};

class TProc
{
private:
    TANumber *fLop_Res; // левый операнд и результат
    TANumber *fRop;     // правый операнд
    TANumber *fZero;    // образец нуля для сброса
    TOprtn fOperation;  // текущая операция
    std::string fError; // сообщение об ошибке

public:
    // Конструктор: создаёт копии L и R, запоминает тип для zero()
    TProc(TANumber *L, TANumber *R);
    ~TProc();

    // Сброс процессора: установить оба операнда в ноль, очистить ошибку, операцию = None
    void ReSet();

    // Сброс операции: установить fOperation = None
    void OprtnClear();

    // Выполнить текущую операцию (если установлена)
    void OprtnRun();

    // Выполнить функцию над правым операндом
    void FuncRun(TFunc func);

    // Установка левого операнда (копия)
    void SetLOpnd(TANumber *r);
    // Получение копии левого операнда
    TANumber *GetLOpnd_Res() const;

    // Установка правого операнда (копия)
    void SetROpnd(TANumber *r);
    // Получение копии правого операнда
    TANumber *GetRopd() const;

    // Установка/получение операции
    void SetOperation(TOprtn op);
    TOprtn GetOperation() const;

    // Ошибки
    std::string GetError() const;
    void ClearError();

    // Запрет копирования (во избежание двойного удаления)
    TProc(const TProc &) = delete;
    TProc &operator=(const TProc &) = delete;
};

#endif
