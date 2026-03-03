#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "include/UCtrl.h"
#include "include/TSettings.h"
#include "include/THistory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDigitClicked();
    void onAddClicked();
    void onSubtractClicked();
    void onMultiplyClicked();
    void onDivideClicked();
    void onEqualsClicked();
    void onSquareClicked();
    void onReciprocalClicked();
    void onToggleSignClicked();
    void onBackspaceClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onWholeSeparatorClicked();   // для кнопки '|'
    void onMemoryStoreClicked();
    void onMemoryRecallClicked();
    void onMemoryAddClicked();
    void onMemoryClearClicked();
    void onCopyClicked();
    void onPasteClicked();
    void onActionSave();
    void onActionLoad();
    void onActionHelp();
    void onActionAuthors();
    void onSettingsClicked();          // новый слот для диалога настроек

    void updateDisplay();

private:
    Ui::MainWindow *ui;
    TSettings m_settings;
    THistory m_history;
    TCtrl m_ctrl;
    QStringListModel m_historyModel;

    void sendCommand(int cmd);
};

#endif
