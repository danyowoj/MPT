#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/Control.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_sliderP1_valueChanged(int value);
    void on_sliderP2_valueChanged(int value);

    void on_spinBoxP1_valueChanged(int arg1);
    void on_spinBoxP2_valueChanged(int arg1);

    void on_b0_clicked();
    void on_b1_clicked();
    void on_b2_clicked();
    void on_b3_clicked();
    void on_b4_clicked();
    void on_b5_clicked();
    void on_b6_clicked();
    void on_b7_clicked();
    void on_b8_clicked();
    void on_b9_clicked();

    void on_bA_clicked();
    void on_bB_clicked();
    void on_bC_clicked();
    void on_bD_clicked();
    void on_bE_clicked();
    void on_bF_clicked();

    void on_bDot_clicked();
    void on_bMinus_clicked();

    void on_bBS_clicked();   // backspace
    void on_bCE_clicked();   // clear
    void on_bExec_clicked(); // execute

    void onHistoryTriggered();
    void onAboutTriggered();

private:
    void showDigitError(int digit);

    void updateP1(int value);
    void updateP2(int value);

    void refreshInputDisplay();
    void refreshOutputDisplay();

    void executeDigit(int digit);

    Ui::MainWindow *ui;
    Control *m_control;
};
#endif // MAINWINDOW_H
