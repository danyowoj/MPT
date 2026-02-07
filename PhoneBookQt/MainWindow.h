#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "core/Interface.h"
#include "core/Help.h"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_findButton_clicked();
    void on_editButton_clicked();
    void on_clearButton_clicked();

    void on_saveAction_triggered();
    void on_loadAction_triggered();


    void on_mainHelpAction_triggered();

    void on_loadHelpAction_triggered();

    void on_authorsHelpAction_triggered();

private:
    Ui::MainWindow *ui;

    Interface interface; // контроллер
    Help help;           // справка

    void updateList();
};

#endif // MAINWINDOW_H
