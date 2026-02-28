#ifndef UCTRL_H
#define UCTRL_H

#include "../include/TProc.h"
#include "../include/UEditor.h"
#include "../include/UMemory.h"
#include <string>
#include <memory>

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
    // Цифры 0-9
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
    // Операции
    CMD_ADD = 10,
    CMD_SUBTRACT,
    CMD_MULTIPLY,
    CMD_DIVIDE,
    // Функции
    CMD_SQR = 14,
    CMD_RECIPROCAL,
    CMD_NEGATE,
    // Управление
    CMD_EQUALS = 17,
    CMD_CLEAR,
    CMD_BACKSPACE,
    CMD_TOGGLE_SIGN,
    // Память
    CMD_MEM_STORE = 21,
    CMD_MEM_RECALL,
    CMD_MEM_CLEAR,
    CMD_MEM_ADD,
    // Буфер обмена
    CMD_COPY = 25,
    CMD_PASTE
};

/**
 * @brief Класс управления калькулятором (TCtrl)
 *
 * Отвечает за координацию редактора, процессора, памяти и буфера обмена.
 * Реализует конечный автомат состояний.
 */
class TCtrl
{
private:
    TEditor *m_editor;        // редактор
    TProc *m_proc;            // процессор
    TMemory<TFrac> *m_memory; // память
    TCtrlState m_state;       // текущее состояние
    TFrac m_number;           // последний результат (дублирует текущее значение)

    void updateFromEditor();            // парсит строку редактора в m_number
    void updateEditor();                // устанавливает строку редактора из m_number
    void setState(TCtrlState newState); // устанавливает состояние
    void handleError();                 // переход в состояние ошибки

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

    // Доступ к состоянию
    TCtrlState getState() const { return m_state; }
    std::string getDisplay() const; // текущая строка для отображения
};

#endif // UCTRL_H
