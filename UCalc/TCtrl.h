#ifndef TCTRL_H
#define TCTRL_H

#include <string>
#include "TAEditor.h"
#include "TProc.h"
#include "TMemory.h"
#include "TANumber.h"
#include "CalcCommands.h"

// Состояния калькулятора
enum TCtrlState
{
    cStart,    // начальное
    cEditing,  // ввод и редактирование
    cValDone,  // значение введено
    cOpDone,   // операция установлена
    cFunDone,  // функция выполнена
    cExpDone,  // выражение вычислено
    cOpChange, // операция изменена
    cError     // ошибка
};

class TCtrl
{
private:
    TAEditor *editor;
    TProc *proc;
    TMemory *mem;
    TCtrlState state;
    TANumber *zero; // образец нуля для сброса

    // Вспомогательные методы
    void resetToZero();
    std::string handleEditorCommand(unsigned cmd);
    std::string handleOperation(unsigned cmd);
    std::string handleFunction(unsigned cmd);
    std::string handleMemory(unsigned cmd, std::string &memState);
    std::string handleClipboard(unsigned cmd, std::string &clipboard);

public:
    // Конструктор: mode = 0 – p-ичное, 1 – дробь, 2 – комплексное
    // base – основание для p-ичного (если mode==0)
    TCtrl(int mode, int base = 10);
    ~TCtrl();

    /* Основной метод выполнения команды
    Возвращает строку для отображения, обновляет clipboard и memState */
    std::string executeCommand(unsigned cmd, std::string &clipboard, std::string &memState);

    // Сменить режим (пересоздаёт компоненты)
    void changeMode(int mode, int base = 10);

    std::string repeatOperation(const std::string& left, const std::string& right, TOprtn op);
};

#endif
