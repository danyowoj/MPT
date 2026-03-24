#include "TCtrl.h"
#include "PEditor.h"
#include "FEditor.h"
#include "CEditor.h"
#include "TPNumber.h"
#include "TFrac.h"
#include "TComplex.h"

// Конструктор
TCtrl::TCtrl(int mode, int base)
{
    TANumber *leftZero = nullptr;
    TANumber *rightZero = nullptr;

    if (mode == 0)
    { // p-ичное
        editor = new PEditor(base);
        zero = new TPNumber(0.0, base);
        leftZero = new TPNumber(0.0, base);
        rightZero = new TPNumber(0.0, base);
    }
    else if (mode == 1)
    { // дробь
        editor = new FEditor();
        zero = new TFrac(0, 1);
        leftZero = new TFrac(0, 1);
        rightZero = new TFrac(0, 1);
    }
    else
    { // комплексное
        editor = new CEditor();
        zero = new TComplex(0.0, 0.0);
        leftZero = new TComplex(0.0, 0.0);
        rightZero = new TComplex(0.0, 0.0);
    }

    proc = new TProc(leftZero, rightZero);
    mem = new TMemory(*zero);

    delete leftZero;
    delete rightZero;

    state = cStart;
}

// Деструктор
TCtrl::~TCtrl()
{
    delete editor;
    delete proc;
    delete mem;
    delete zero;
}

// Сброс калькулятора в начальное состояние
void TCtrl::resetToZero()
{
    editor->clear();
    proc->ReSet();
    proc->ClearError();
    state = cStart;
}

// Обработка команд редактора
std::string TCtrl::handleEditorCommand(unsigned cmd)
{
    std::string result = editor->executeCommand(cmd);
    if (!editor->isZero())
        state = cValDone;
    else
        state = cStart;
    return result;
}

// Обработка операций (+, -, *, /)
std::string TCtrl::handleOperation(unsigned cmd)
{
    // Получаем текущее число из редактора
    TANumber *current = editor->createNumber();
    // Устанавливаем его как левый операнд процессора
    proc->SetLOpnd(current);
    delete current;

    // Сбрасываем редактор к нулю
    editor->clear();

    // Устанавливаем операцию
    TOprtn op;
    switch (cmd)
    {
    case cAdd:
        op = Add;
        break;
    case cSub:
        op = Sub;
        break;
    case cMul:
        op = Mul;
        break;
    case cDiv:
        op = Dvd;
        break;
    default:
        return editor->getString();
    }
    proc->SetOperation(op);
    state = cOpDone;
    return editor->getString(); // отображаем 0
}

// Обработка функций (rev, sqr)
std::string TCtrl::handleFunction(unsigned cmd)
{
    // Функция выполняется над текущим числом редактора
    TANumber *current = editor->createNumber();
    proc->SetROpnd(current);
    delete current;

    // Выполняем функцию
    TFunc func = (cmd == cRev) ? Rev : Sqr;
    proc->FuncRun(func);

    // Проверяем ошибку
    if (!proc->GetError().empty())
    {
        state = cError;
        return proc->GetError();
    }

    // Результат в правом операнде; переносим в редактор
    TANumber *res = proc->GetRopd();
    editor->setString(res->toString());
    delete res;

    state = cFunDone;
    return editor->getString();
}

// Обработка команд памяти
std::string TCtrl::handleMemory(unsigned cmd, std::string &memState)
{
    if (cmd == cMemStore)
    {
        TANumber *current = editor->createNumber();
        mem->store(*current);
        delete current;
    }
    else if (cmd == cMemRestore)
    {
        TANumber *restored = mem->restore();
        if (restored)
        {
            editor->setString(restored->toString());
            delete restored;
        }
    }
    else if (cmd == cMemAdd)
    {
        TANumber *current = editor->createNumber();
        mem->add(*current);
        delete current;
    }
    else if (cmd == cMemClear)
    {
        mem->clear();
    }

    memState = mem->getStateString();
    return editor->getString();
}

// Обработка команд буфера обмена
std::string TCtrl::handleClipboard(unsigned cmd, std::string &clipboard)
{
    if (cmd == cCopy)
    {
        clipboard = editor->getString();
    }
    else if (cmd == cPaste)
    {
        editor->setString(clipboard);
    }
    return editor->getString();
}

// Основной метод выполнения команды
std::string TCtrl::executeCommand(unsigned cmd, std::string &clipboard, std::string &memState)
{
    // Если в состоянии ошибки, большинство команд сбрасывают ошибку
    if (state == cError && cmd != cClear)
    {
        resetToZero();
    }

    std::string result;

    // Команды редактора (0..13)
    if (cmd <= cClear)
    {
        result = handleEditorCommand(cmd);
    }
    // Операции
    else if (cmd == cAdd || cmd == cSub || cmd == cMul || cmd == cDiv)
    {
        result = handleOperation(cmd);
    }
    // Функции
    else if (cmd == cRev || cmd == cSqr)
    {
        result = handleFunction(cmd);
    }
    // Равенство
    else if (cmd == cEqual)
    {
        if (proc->GetOperation() != None)
        {
            // Сохраняем текущее число как правый операнд
            TANumber *current = editor->createNumber();
            proc->SetROpnd(current);
            delete current;

            proc->OprtnRun();
            if (!proc->GetError().empty())
            {
                state = cError;
                result = proc->GetError();
            }
            else
            {
                // Результат в левом операнде
                TANumber *res = proc->GetLOpnd_Res();
                editor->setString(res->toString());
                delete res;
                proc->SetOperation(None);
                state = cExpDone;
                result = editor->getString();
            }
        }
        else
        {
            result = editor->getString();
        }
    }
    // Команды памяти
    else if (cmd >= cMemStore && cmd <= cMemClear)
    {
        result = handleMemory(cmd, memState);
    }
    // Команды буфера обмена
    else if (cmd == cCopy || cmd == cPaste)
    {
        result = handleClipboard(cmd, clipboard);
    }
    // Сброс (CE)
    else if (cmd == cClear)
    {
        resetToZero();
        result = editor->getString();
    }
    // Неизвестная команда
    else
    {
        result = editor->getString();
    }

    return result;
}

// Смена режима (типа чисел)
void TCtrl::changeMode(int mode, int base)
{
    delete editor;
    delete proc;
    delete mem;
    delete zero;

    TANumber *leftZero = nullptr;
    TANumber *rightZero = nullptr;

    if (mode == 0)
    {
        editor = new PEditor(base);
        zero = new TPNumber(0.0, base);
        leftZero = new TPNumber(0.0, base);
        rightZero = new TPNumber(0.0, base);
    }
    else if (mode == 1)
    {
        editor = new FEditor();
        zero = new TFrac(0, 1);
        leftZero = new TFrac(0, 1);
        rightZero = new TFrac(0, 1);
    }
    else
    {
        editor = new CEditor();
        zero = new TComplex(0.0, 0.0);
        leftZero = new TComplex(0.0, 0.0);
        rightZero = new TComplex(0.0, 0.0);
    }

    proc = new TProc(leftZero, rightZero);
    mem = new TMemory(*zero);

    delete leftZero;
    delete rightZero;

    state = cStart;
}

std::string TCtrl::repeatOperation(const std::string& left, const std::string& right, TOprtn op)
{
    editor->setString(left);
    TANumber* leftNum = editor->createNumber();
    editor->setString(right);
    TANumber* rightNum = editor->createNumber();

    proc->SetLOpnd(leftNum);
    proc->SetROpnd(rightNum);
    proc->SetOperation(op);
    proc->OprtnRun();

    std::string result;
    if (proc->GetError().empty()) {
        TANumber* res = proc->GetLOpnd_Res();
        result = res->toString();
        editor->setString(result);
        delete res;
    } else {
        result = proc->GetError();
        editor->setString(result);
    }

    delete leftNum;
    delete rightNum;
    proc->ClearError();
    return result;
}
