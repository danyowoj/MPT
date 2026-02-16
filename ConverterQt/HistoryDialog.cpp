#include "HistoryDialog.h"
#include <QVBoxLayout>

HistoryDialog::HistoryDialog(History *history, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("История преобразований");
    resize(400, 300);

    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_textEdit);

    if (history->count() == 0)
        m_textEdit->setPlainText("История пуста");
    else
    {
        for (int i = 0; i < history->count(); ++i)
        {
            History::Record rec = history->getRecord(i);
            m_textEdit->append(QString::fromStdString(rec.toString()));
        }
    }
}
