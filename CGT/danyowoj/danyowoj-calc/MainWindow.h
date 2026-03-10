#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "TSettings.h"
#include "THistory.h"
#include "UCtrl.h"

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
    void handleCommand(int cmd);
    void onActionSave();
    void onActionLoad();
    void onActionPreferences();
    void onActionHelp();
    void onActionAuthors();

private:
    void updateDisplay();
    void updateHistory();
    void updateMemoryState();
    void setupButtons();

    Ui::MainWindow *ui;
    TSettings *m_settings;
    THistory *m_history;
    TCtrl *m_ctrl;
    QStringListModel *m_historyModel;
    std::string m_clipboard;
};

#endif // MAINWINDOW_H
