#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "TProc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TCtrl;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ... остальные слоты ...
    void onDigitClicked();
    void onLetterClicked();
    void onOperationClicked();
    void onFunctionClicked();
    void onMemoryClicked();
    void onEditClicked();
    void onCalculateClicked();
    void onClearEntryClicked();
    void onClearClicked();
    void onBackspaceClicked();
    void onSignClicked();
    void onSeparatorClicked();
    void onModeChanged();
    void onBaseChanged();
    void onPreferences();
    void onHelp();
    void onAuthors();

private:
    Ui::MainWindow *ui;
    TCtrl *ctrl;
    int currentMode;
    int currentBase;
    TOprtn lastOperation;          // последняя операция для повторения
    QString lastRightOperand;      // последний правый операнд
    QString lastLeftOperand;      // последний левый операнд

    QStringList historyList;
    QStringListModel *historyModel;

    bool useMemoryMode;            // true - память, false - буфер обмена
    void updateMemoryClipboardVisibility();   // обновить видимость групп кнопок
    void setupMemoryClipboardActions();

    void setupConnections();
    void setupModeActions();
    void setupBaseActions();
    void updateUIMode();
    void updateBaseVisibility();
    void updateDigitButtonsState();   // новая функция
    void updateDisplay(const QString &text);
    void updateMemIndicator(const QString &state);
    void addHistoryEntry(const QString &entry);
};

#endif // MAINWINDOW_H
