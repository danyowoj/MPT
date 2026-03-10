#include "../include/UCtrl.h"
#include "../include/UFrac.h"

TCtrl::TCtrl(TSettings *settings, THistory *history)
    : m_editor(new TEditor())
    , m_proc(new TProc())
    , m_memory(new TMemory<TFrac>())
    , m_settings(settings)
    , m_history(history)
    , m_state(TCtrlState::cStart)
    , m_lastValidNumber("0/1")
{
    TFrac initial("0/1");
    m_proc->setOperand(initial);
}

TCtrl::~TCtrl()
{
    delete m_editor;
    delete m_proc;
    delete m_memory;
}

void TCtrl::setState(TCtrlState newState)
{
    m_state = newState;
}

void TCtrl::handleError()
{
    setState(TCtrlState::cError);
    m_proc->clear();
    m_editor->setString("Error");
    // lastValidNumber остаётся прежним (можно оставить)
}

void TCtrl::addHistoryEntry(const std::string &entry)
{
    if (m_history)
        m_history->addEntry(entry);
}

bool TCtrl::parseCurrentEditor(TFrac &out) const
{
    std::string s = m_editor->getString();
    if (s.empty())
        return false;
    try
    {
        out = TFrac(s);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void TCtrl::updateLastValidFromEditor()
{
    TFrac val;
    if (parseCurrentEditor(val))
        m_lastValidNumber = val;
}

std::string TCtrl::getDisplay() const
{
    if (m_state == TCtrlState::cError)
        return m_editor->getString(); // "Error"
    return m_editor->getString();
}

void TCtrl::applySettings()
{
    // Переформатировать последнее корректное число в соответствии с новыми настройками
    DisplayFormat fmt = m_settings ? m_settings->displayFormat() : DisplayFormat::Fraction;
    m_editor->setString(m_lastValidNumber.toString(fmt));
    // Если состояние ошибки, оставляем "Error"
    if (m_state == TCtrlState::cError)
        m_editor->setString("Error");
    // Процессор не трогаем, так как он оперирует числами независимо
}

// ----------------------------------------------------------------------
std::string TCtrl::executeEditorCommand(int cmd)
{
    std::string oldStr = m_editor->getString();
    bool startNewInput = false;

    // Определяем, нужно ли начать новый ввод перед обработкой команды
    if (cmd >= CMD_0 && cmd <= CMD_9)
    {
        if (m_state == TCtrlState::cOpChange ||
            m_state == TCtrlState::cExpDone ||
            m_state == TCtrlState::cFunDone ||
            oldStr == "0/1")
        {
            startNewInput = true;
        }
    }
    else if (cmd == CMD_WHOLE_SEP)
    {
        if (m_state == TCtrlState::cOpChange ||
            m_state == TCtrlState::cExpDone ||
            m_state == TCtrlState::cFunDone ||
            oldStr == "0/1")
        {
            startNewInput = true;
        }
    }

    if (startNewInput)
    {
        m_editor->setString("");
    }

    // Выполняем команду редактирования
    switch (cmd)
    {
    case CMD_0: case CMD_1: case CMD_2: case CMD_3: case CMD_4:
    case CMD_5: case CMD_6: case CMD_7: case CMD_8: case CMD_9:
        m_editor->addDigit(cmd);
        break;
    case CMD_TOGGLE_SIGN:
        m_editor->toggleSign();
        break;
    case CMD_BACKSPACE:
        m_editor->backspace();
        break;
    case CMD_CLEAR_ENTRY:
        m_editor->clear(); // становится "0/1"
        // После очистки lastValidNumber должно стать "0/1"
        m_lastValidNumber = TFrac("0/1");
        setState(TCtrlState::cStart); // или cEditing? оставим cStart
        return getDisplay(); // сразу возвращаем, чтобы не обновлять lastValid повторно
    case CMD_WHOLE_SEP:
        m_editor->addWholeSeparator();
        break;
    default:
        return getDisplay();
    }

    // После изменения пытаемся обновить lastValidNumber, если строка стала валидной
    updateLastValidFromEditor();
    setState(TCtrlState::cEditing);
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeOperation(int cmd)
{
    TFrac current;
    if (!parseCurrentEditor(current))
        return getDisplay(); // невалидное число – игнорируем

    m_lastValidNumber = current; // запоминаем последнее валидное
    m_proc->setOperand(current);

    Operation op;
    switch (cmd)
    {
    case CMD_ADD:
        op = Operation::Add;
        break;
    case CMD_SUBTRACT:
        op = Operation::Subtract;
        break;
    case CMD_MULTIPLY:
        op = Operation::Multiply;
        break;
    case CMD_DIVIDE:
        op = Operation::Divide;
        break;
    default:
        return getDisplay();
    }

    m_proc->setOperation(op);
    setState(TCtrlState::cOpChange);
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeFunction(int cmd)
{
    TFrac current;
    if (!parseCurrentEditor(current))
        return getDisplay();

    m_lastValidNumber = current;

    Function func;
    switch (cmd)
    {
    case CMD_SQR:
        func = Function::Square;
        break;
    case CMD_RECIPROCAL:
        func = Function::Reciprocal;
        break;
    case CMD_NEGATE:
        func = Function::Negate;
        break;
    default:
        return getDisplay();
    }

    try
    {
        TFrac before = current;
        m_proc->setOperand(current);
        m_proc->performFunction(func);
        TFrac result = m_proc->getCurrentValue();

        m_lastValidNumber = result;
        DisplayFormat fmt = m_settings ? m_settings->displayFormat() : DisplayFormat::Fraction;
        m_editor->setString(result.toString(fmt));
        setState(TCtrlState::cFunDone);

        std::string entry = before.toString() + " " +
                            (func == Function::Square ? "sqr" :
                                 (func == Function::Reciprocal ? "1/x" : "±")) +
                            " = " + result.toString();
        addHistoryEntry(entry);
    }
    catch (...)
    {
        handleError();
    }
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::calculateExpression()
{
    TFrac current;
    if (!parseCurrentEditor(current))
        return getDisplay();

    m_lastValidNumber = current;
    m_proc->setOperand(current);

    if (m_proc->getPendingOperation() == Operation::None)
    {
        // Нет операции — ничего не делаем (калькуляторы обычно ничего не делают)
        return getDisplay();
    }

    TFrac first = m_proc->getPendingOperand();
    Operation op = m_proc->getPendingOperation();

    try
    {
        m_proc->calculate();
        TFrac result = m_proc->getCurrentValue();
        TFrac second = m_proc->getLastOperand();

        m_lastValidNumber = result;
        DisplayFormat fmt = m_settings ? m_settings->displayFormat() : DisplayFormat::Fraction;
        m_editor->setString(result.toString(fmt));
        setState(TCtrlState::cExpDone);

        std::string opStr;
        switch (op)
        {
        case Operation::Add: opStr = "+"; break;
        case Operation::Subtract: opStr = "-"; break;
        case Operation::Multiply: opStr = "*"; break;
        case Operation::Divide: opStr = "/"; break;
        default: opStr = ""; break;
        }
        std::string entry = first.toString() + " " + opStr + " " +
                            second.toString() + " = " + result.toString();
        addHistoryEntry(entry);
    }
    catch (...)
    {
        handleError();
    }
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::setInitialState()
{
    m_editor->clear(); // устанавливает "0/1"
    m_lastValidNumber = TFrac("0/1");
    m_proc->clear();
    m_proc->setOperand(m_lastValidNumber);
    setState(TCtrlState::cStart);
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeMemoryCommand(int cmd, std::string &memState)
{
    TFrac current;
    bool parsed = parseCurrentEditor(current);

    switch (cmd)
    {
    case CMD_MEM_STORE:
        if (parsed)
        {
            m_lastValidNumber = current;
            m_memory->store(current);
        }
        break;
    case CMD_MEM_RECALL:
    {
        TFrac val = m_memory->recall();
        m_lastValidNumber = val;
        DisplayFormat fmt = m_settings ? m_settings->displayFormat() : DisplayFormat::Fraction;
        m_editor->setString(val.toString(fmt));
        m_proc->setOperand(val);
        setState(TCtrlState::cEditing);
        break;
    }
    case CMD_MEM_CLEAR:
        m_memory->clear();
        break;
    case CMD_MEM_ADD:
        if (parsed)
        {
            m_lastValidNumber = current;
            m_memory->add(current);
        }
        break;
    }
    memState = m_memory->stateString();
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeClipboardCommand(int cmd, std::string &clipboard)
{
    if (cmd == CMD_COPY)
    {
        clipboard = m_editor->getString(); // копируем текущее содержимое
    }
    else if (cmd == CMD_PASTE && !clipboard.empty())
    {
        try
        {
            TFrac val(clipboard);
            m_lastValidNumber = val;
            DisplayFormat fmt = m_settings ? m_settings->displayFormat() : DisplayFormat::Fraction;
            m_editor->setString(val.toString(fmt));
            m_proc->setOperand(val);
            setState(TCtrlState::cEditing);
        }
        catch (...)
        {
            // игнорируем невалидные данные
        }
    }
    return getDisplay();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeCommand(int cmd, std::string &clipboard, std::string &memState)
{
    if (cmd >= CMD_0 && cmd <= CMD_9)
    {
        return executeEditorCommand(cmd);
    }
    switch (cmd)
    {
    case CMD_TOGGLE_SIGN:
    case CMD_BACKSPACE:
    case CMD_CLEAR_ENTRY:
    case CMD_WHOLE_SEP:
        return executeEditorCommand(cmd);
    case CMD_CLEAR:
        return setInitialState();
    case CMD_ADD:
    case CMD_SUBTRACT:
    case CMD_MULTIPLY:
        return executeOperation(cmd);
    case CMD_DIVIDE:
    {
        TFrac dummy;
        if (parseCurrentEditor(dummy))
            return executeOperation(cmd);   // валидное число → операция
        else
        {
            m_editor->addSlash();            // невалидное → добавляем символ '/'
            updateLastValidFromEditor();
            setState(TCtrlState::cEditing);
            return getDisplay();
        }
    }
    case CMD_SQR:
    case CMD_RECIPROCAL:
    case CMD_NEGATE:
        return executeFunction(cmd);
    case CMD_EQUALS:
        return calculateExpression();
    case CMD_MEM_STORE:
    case CMD_MEM_RECALL:
    case CMD_MEM_CLEAR:
    case CMD_MEM_ADD:
        return executeMemoryCommand(cmd, memState);
    case CMD_COPY:
    case CMD_PASTE:
        return executeClipboardCommand(cmd, clipboard);
    default:
        return getDisplay();
    }
}
