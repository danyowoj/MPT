#ifndef TPROC_H
#define TPROC_H

#include "UFrac.h"
#include "CalcEnums.h"

class TProc
{
private:
    TFrac m_currentValue;         // текущее отображаемое значение
    TFrac m_pendingOperand;       // первый операнд для ожидающей операции
    Operation m_pendingOperation; // ожидающая операция (если не None)
    Operation m_lastOperation;    // последняя выполненная операция
    TFrac m_lastOperand;          // последний второй операнд
    bool m_operationPending;      // true, если есть ожидающая операция
    bool m_secondOperandReceived; // true, если после установки операции был введён второй операнд

public:
    TProc();

    void setOperand(const TFrac &value);
    void setOperation(Operation op);
    void performFunction(Function func);
    void calculate();
    void repeatLastOperation();
    void clear();

    TFrac getCurrentValue() const;
    Operation getPendingOperation() const { return m_pendingOperation; }
    bool isOperationPending() const { return m_operationPending; }
    bool isSecondOperandReceived() const { return m_secondOperandReceived; }
};

#endif