#ifndef UCTRL_H
#define UCTRL_H

#include "../include/TProc.h"
#include "../include/UEditor.h"
#include "../include/UMemory.h"
#include <string>

enum class TCtrlState
{
    cStart,
    cEditing,
    cFuncDone,
    cValDone,
    cExpDone,
    cOpChange,
    cError
};

enum CommandCode
{
    CMD_0 = 0,
    CMD_1,
    CMD_2,
    CMD_3,
    CMD_4,
    CMD_5,
    CMD_6,
    CMD_7,
    CMD_8,
    CMD_9,

    CMD_ADD = 10,
    CMD_SUBTRACT,
    CMD_MULTIPLY,
    CMD_DIVIDE,

    CMD_SQR = 14,
    CMD_RECIPROCAL,
    CMD_NEGATE,

    CMD_EQUALS = 17,
    CMD_CLEAR,
    CMD_BACKSPACE,
    CMD_TOGGLE_SIGN,

    CMD_MEM_STORE = 21,
    CMD_MEM_RECALL,
    CMD_MEM_CLEAR,
    CMD_MEM_ADD,

    CMD_COPY = 25,
    CMD_PASTE
};

/**
 * @brief Класс управления калькулятором (TCtrl)
 *
 * Координирует работу редактора, процессора, памяти и буфера обмена.
 * Поддерживает конечный автомат состояний.
 */
class TCtrl
{
private:
    TEditor *m_editor;
    TProc *m_proc;
    TMemory<TFrac> *m_memory;
    TCtrlState m_state;
    TFrac m_number;           // последний результат
    std::string m_lastResult; // втрока для отображния

    void updateResultString();
    void setState(TCtrlState newState);

public:
    TCtrl();
    ~TCtrl();

    /**
     * @brief Основной метод обработки команд пользователя
     * @param cmd код команды
     * @param clipboard строка буфера обмена (вход/выход)
     * @param memState строка состояния памяти (вход/выход)
     * @return строка для отображения на дисплее
     */
    std::string executeCommand(int cmd, std::string &clipboard, std::string &memState);

    std::string executeEditorCommand(int cmd);
    std::string executeOperation(int cmd);
    std::string executeFunction(int cmd);
    std::string calculateExpression();
    std::string setInitialState();
    std::string executeMemoryCommand(int cmd, std::string &memState);
    std::string executeClipboardCommand(int cmd, std::string &clipboard);

    TCtrlState getState() const { return m_state; }
    std::string getDisplay() const { return m_lastResult; }
};

#endif // UCTRL_H
