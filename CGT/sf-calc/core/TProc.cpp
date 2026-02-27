// TProc.cpp
#include "TProc.h"

TProc::TProc()
    : m_currentValue(), m_pendingOperand(), m_pendingOperation(Operation::None), m_lastOperation(Operation::None), m_lastOperand(), m_operationPending(false), m_secondOperandReceived(false)
{
}

void TProc::setOperand(const TFrac &value)
{
    m_currentValue = value;
    if (m_operationPending && !m_secondOperandReceived)
    {
        m_secondOperandReceived = true;
    }
}

void TProc::setOperation(Operation op)
{
    if (op == Operation::None)
        return;

    // Если есть незавершённая операция и второй операнд уже получен,
    // то сначала выполняем её (автоматически, как в обычных калькуляторах)
    if (m_operationPending && m_secondOperandReceived)
    {
        calculate(); // после calculate() m_operationPending = false
    }

    // Устанавливаем новую операцию
    m_pendingOperand = m_currentValue; // первый операнд
    m_pendingOperation = op;
    m_operationPending = true;
    m_secondOperandReceived = false; // второго операнда пока нет
}

void TProc::performFunction(Function func)
{
    TFrac result;
    switch (func)
    {
    case Function::Square:
        result = m_currentValue.square();
        break;
    case Function::Reciprocal:
        result = m_currentValue.reciprocal();
        break;
    case Function::Negate:
        result = m_currentValue.negate();
        break;
    default:
        return;
    }
    m_currentValue = result;

    if (m_operationPending && !m_secondOperandReceived)
    {
        m_pendingOperand = m_currentValue; // первый операнд стал новым значением
    }
    // Если второй операнд уже был, то меняется только он (m_currentValue), а первый остаётся.
}

void TProc::calculate()
{
    if (!m_operationPending)
        return;

    // Определяем второй операнд
    TFrac secondOperand;
    if (m_secondOperandReceived)
    {
        secondOperand = m_currentValue;
    }
    else
    {
        secondOperand = m_pendingOperand; // второй операнд не вводился – используем первый
    }

    TFrac result;
    switch (m_pendingOperation)
    {
    case Operation::Add:
        result = m_pendingOperand.add(secondOperand);
        break;
    case Operation::Subtract:
        result = m_pendingOperand.subtract(secondOperand);
        break;
    case Operation::Multiply:
        result = m_pendingOperand.multiply(secondOperand);
        break;
    case Operation::Divide:
        result = m_pendingOperand.divide(secondOperand);
        break;
    default:
        return;
    }

    // Запоминаем для повтора
    m_lastOperation = m_pendingOperation;
    m_lastOperand = secondOperand;

    m_currentValue = result;

    m_operationPending = false;
    m_secondOperandReceived = false;
    m_pendingOperation = Operation::None;
}

void TProc::repeatLastOperation()
{
    if (m_lastOperation == Operation::None)
        return;

    TFrac result;
    switch (m_lastOperation)
    {
    case Operation::Add:
        result = m_currentValue.add(m_lastOperand);
        break;
    case Operation::Subtract:
        result = m_currentValue.subtract(m_lastOperand);
        break;
    case Operation::Multiply:
        result = m_currentValue.multiply(m_lastOperand);
        break;
    case Operation::Divide:
        result = m_currentValue.divide(m_lastOperand);
        break;
    default:
        return;
    }
    m_currentValue = result;
    // m_lastOperation и m_lastOperand остаются
}

void TProc::clear()
{
    m_currentValue = TFrac();
    m_pendingOperand = TFrac();
    m_pendingOperation = Operation::None;
    m_lastOperation = Operation::None;
    m_lastOperand = TFrac();
    m_operationPending = false;
    m_secondOperandReceived = false;
}

TFrac TProc::getCurrentValue() const
{
    return m_currentValue;
}
