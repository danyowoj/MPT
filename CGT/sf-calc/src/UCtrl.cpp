#include "../include/UCtrl.h"
#include <stdexcept>

TCtrl::TCtrl()
    : m_editor(new TEditor()), m_proc(new TProc()), m_memory(new TMemory<TFrac>()), m_state(TCtrlState::cStart), m_number()
{
    updateEditor();
}

TCtrl::~TCtrl()
{
    delete m_editor;
    delete m_proc;
    delete m_memory;
}

void TCtrl::updateFromEditor()
{
    try
    {
        m_number = TFrac(m_editor->getString());
    }
    catch (...)
    {
    }
}

void TCtrl::updateEditor()
{
    m_editor->setString(m_number.toString());
}

void TCtrl::setState(TCtrlState newState)
{
    m_state = newState;
}

void TCtrl::handleError()
{
    setState(TCtrlState::cError);
    m_proc->clear(); // сброс процессора при ошибке
    m_editor->setString("Error");
}

std::string TCtrl::getDisplay() const
{
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeEditorCommand(int cmd)
{
    std::string oldStr = m_editor->getString();
    std::string newStr = m_editor->edit(cmd);
    if (newStr != oldStr)
    {
        try
        {
            TFrac val(newStr);
            m_number = val;
            m_proc->setOperand(m_number);
            setState(TCtrlState::cEditing);
        }
        catch (...)
        {
            m_editor->setString(oldStr);
        }
    }
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeOperation(int cmd)
{
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
        return m_editor->getString();
    }
    m_proc->setOperation(op);
    setState(TCtrlState::cOpChange);
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeFunction(int cmd)
{
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
        return m_editor->getString();
    }

    try
    {
        m_proc->performFunction(func);
        m_number = m_proc->getCurrentValue();
        updateEditor();
        setState(TCtrlState::cFunDone);
    }
    catch (...)
    {
        handleError();
    }
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::calculateExpression()
{
    try
    {
        m_proc->calculate();
        m_number = m_proc->getCurrentValue();
        updateEditor();
        setState(TCtrlState::cExpDone);
    }
    catch (...)
    {
        handleError();
    }
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::setInitialState()
{
    m_editor->clear();
    m_proc->clear();
    m_number = TFrac();
    m_proc->setOperand(m_number);
    setState(TCtrlState::cStart);
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeMemoryCommand(int cmd, std::string &memState)
{
    switch (cmd)
    {
    case CMD_MEM_STORE:
        m_memory->store(m_number);
        break;
    case CMD_MEM_RECALL:
    {
        TFrac val = m_memory->recall();
        m_number = val;
        updateEditor();
        m_proc->setOperand(m_number);
        setState(TCtrlState::cEditing);
        break;
    }
    case CMD_MEM_CLEAR:
        m_memory->clear();
        break;
    case CMD_MEM_ADD:
        m_memory->add(m_number);
        break;
    }
    memState = m_memory->stateString();
    return m_editor->getString();
}

// ----------------------------------------------------------------------
std::string TCtrl::executeClipboardCommand(int cmd, std::string &clipboard)
{
    if (cmd == CMD_COPY)
    {
        clipboard = m_number.toString();
    }
    else if (cmd == CMD_PASTE && !clipboard.empty())
    {
        try
        {
            TFrac val(clipboard);
            m_number = val;
            updateEditor();
            m_proc->setOperand(m_number);
            setState(TCtrlState::cEditing);
        }
        catch (...)
        {
            // Не удалось вставить – игнорируем
        }
    }
    return m_editor->getString();
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
        return executeEditorCommand(-1);
    case CMD_BACKSPACE:
        return executeEditorCommand(-2);
    case CMD_CLEAR:
        return setInitialState();
    case CMD_ADD:
    case CMD_SUBTRACT:
    case CMD_MULTIPLY:
    case CMD_DIVIDE:
        return executeOperation(cmd);
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
        return m_editor->getString();
    }
}
