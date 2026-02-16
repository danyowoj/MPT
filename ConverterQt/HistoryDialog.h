#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include "core/History.h"

class HistoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HistoryDialog(History *history, QWidget *parent = nullptr);
private:
    QTextEdit *m_textEdit;
};

#endif // HISTORYDIALOG_H
