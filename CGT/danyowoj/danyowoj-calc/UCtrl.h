#ifndef UCTRL_H
#define UCTRL_H

#include "TProc.h"
#include "UEditor.h"
#include "UMemory.h"
#include "TSettings.h"
#include "THistory.h"
#include <string>

enum class TCtrlState
{
    cStart,
    cEditing,
    cFunDone,
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
    CMD_PASTE,
    CMD_CLEAR_ENTRY = 27,
    CMD_WHOLE_SEP = 29
};

class TCtrl
{
private:
    TEditor *m_editor;
    TProc *m_proc;
    TMemory<TFrac> *m_memory;
    TSettings *m_settings;
    THistory *m_history;
    TCtrlState m_state;
    TFrac m_lastValidNumber;  // последнее корректное число (для форматирования)

    void setState(TCtrlState newState);
    void handleError();
    void addHistoryEntry(const std::string &entry);
    bool parseCurrentEditor(TFrac &out) const;
    void updateLastValidFromEditor();  // обновить m_lastValidNumber из редактора, если строка валидна

public:
    TCtrl(TSettings *settings = nullptr, THistory *history = nullptr);
    ~TCtrl();

    std::string executeCommand(int cmd, std::string &clipboard, std::string &memState);
    std::string executeEditorCommand(int cmd);
    std::string executeOperation(int cmd);
    std::string executeFunction(int cmd);
    std::string calculateExpression();
    std::string setInitialState();
    std::string executeMemoryCommand(int cmd, std::string &memState);
    std::string executeClipboardCommand(int cmd, std::string &clipboard);

    TCtrlState getState() const { return m_state; }
    std::string getDisplay() const;
    std::string getMemoryState() const { return m_memory->stateString(); }

    void setSettings(TSettings *settings) { m_settings = settings; }
    void setHistory(THistory *history) { m_history = history; }

    // Применить текущие настройки (формат отображения) к отображаемому числу
    void applySettings();
};

#endif
