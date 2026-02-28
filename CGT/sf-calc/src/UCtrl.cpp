#include "../include/UCtrl.h"
#include <sstream>

TCtrl::TCtrl() : m_editor(new TEditor()), m_proc(new TProc()), m_memory(new TMemory<TFrac>()), m_state(TCtrlState::cStart), m_number(), m_lastResult("0/1") {}

TCtrl::~TCtrl()
{
    delete m_editor;
    delete m_proc;
    delete m_memory;
}

void TCtrl::updateResultString() { m_lastResult = m_number.toString(); }

void TCtrl::setState(TCtrlState newState) { m_state = newState; }

std::string TCtrl::executeCommand(int cmd, std::string &clipboard, std::string &memState)
{
    if (cmd >= CMD_0 && cmd <= CMD_9)
    {
        std::string editorResult = m_editor->edit(cmd);
        try
        {
            m_number = TFrac(editorResult);
            setState(TCtrlState::cEditing);
            m_proc->setOperand(m_number);
        }
        catch (...)
        {
        }
    }
    else if (cmd == CMD_TOGGLE_SIGN)
    {
        std::string editorResult = m_editor->edit(-1);
        try
        {
            m_number = TFrac(editorResult);
            setState(TCtrlState::cEditing);
            m_proc->setOperand(m_number);
        }
        catch (...)
        {
        }
    }
    else if (cmd == CMD_BACKSPACE)
    {
        std::string editorResult = m_editor->edit(-2);
        try
        {
            m_number = TFrac(editorResult);
            setState(TCtrlState::cEditing);
            m_proc->setOperand(m_number);
        }
        catch (...)
        {
        }
    }
    else if (cmd == CMD_CLEAR)
    {
        m_editor->clear();
        m_proc->clear();
        m_number = TFrac();
        m_proc->setOperand(m_number);
        setState(TCtrlState::cStart);
    }
    else if (cmd == CMD_ADD || cmd == CMD_SUBTRACT || cmd == CMD_MULTIPLY || cmd == CMD_DIVIDE)
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
            op = Operation::None;
        }
        if (op != Operation::None)
        {
            m_proc->setOperation(op);
            setState(TCtrlState::cOpChange);
        }
    }
    else if (cmd == CMD_SQR || cmd == CMD_RECIPROCAL || cmd == CMD_NEGATE)
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
            Function::Square;
        }
        m_proc->performFunction(func);
        m_number = m_proc->getCurrentValue();
        m_editor->setString(m_number.toString());
        setState(TCtrlState::cFuncDone);
    }
    else if (cmd == CMD_EQUALS)
    {
        m_proc->calculate();
        m_number = m_proc->getCurrentValue();
        m_editor->setString(m_number.toString());
        setState(TCtrlState::cExpDone);
    }
    else if (cmd == CMD_MEM_STORE)
    {
        m_memory->store(m_number);
    }
    else if (cmd == CMD_MEM_RECALL)
    {
        TFrac val = m_memory->recall();
        m_number = val;
        m_editor->setString(m_number.toString());
        m_proc->setOperand(m_number);
        setState(TCtrlState::cEditing);
    }
    else if (cmd == CMD_MEM_CLEAR)
    {
        m_memory->clear();
    }
    else if (cmd == CMD_MEM_ADD)
    {
        m_memory->add(m_number);
    }
    else if (cmd == CMD_COPY)
    {
        clipboard = m_number.toString();
    }
    else if (CMD_PASTE)
    {
        if (!clipboard.empty())
        {
            try
            {
                TFrac val(clipboard);
                m_number = val;
                m_editor->setString(m_number.toString());
                m_proc->setOperand(m_number);
                setState(TCtrlState::cEditing);
            }
            catch (...)
            {
            }
        }
    }

    memState = m_memory->stateString();

    updateResultString();
    return m_lastResult;
}

std::string TCtrl::executeEditorCommand(int cmd)
{
    std::string res = m_editor->edit(cmd);
    try
    {
        m_number = TFrac(res);
        m_proc->setOperand(m_number);
    }
    catch (...)
    {
    }
    updateResultString();
    return m_lastResult;
}

std::string TCtrl::executeOperation(int cmd) { return executeCommand(cmd, *(new std::string), *(new std::string)); }

std::string TCtrl::executeFunction(int cmd) { return executeCommand(cmd, *(new std::string), *(new std::string)); }

std::string TCtrl::calculateExpression() { return executeCommand(CMD_EQUALS, *(new std::string), *(new std::string)); }

std::string TCtrl::setInitialState() { return executeCommand(CMD_CLEAR, *(new std::string), *(new std::string)); }

std::string TCtrl::executeMemoryCommand(int cmd, std::string &memState) { return executeCommand(cmd, *(new std::string), memState); }

std::string TCtrl::executeClipboardCommand(int cmd, std::string &clipboard) { return executeCommand(cmd, clipboard, *(new std::string)); }
