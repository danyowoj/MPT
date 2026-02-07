#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Телефонная книга");
    interface.load("phonebook.txt");
    updateList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateList()
{
    ui->listWidget->clear();

    auto abonents = interface.getAll();

    for (const auto &pair : abonents)
    {
        QString line = QString::fromStdString(pair.first) + " : " + QString::fromStdString(pair.second);
        ui->listWidget->addItem(line);
    }

    ui->nameEdit->clear();
    ui->phoneEdit->clear();
}

void MainWindow::on_addButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();

    if(!interface.addAbonent(name.toStdString(), phone.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные или абонент уже существует");
        return;
    }

    updateList();
}


void MainWindow::on_removeButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();

    if(!interface.removeAbonent(name.toStdString(), phone.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Абонент не найден");
        return;
    }

    updateList();
}


void MainWindow::on_findButton_clicked()
{
    QString name = ui->nameEdit->text().trimmed();
    QString phone = ui->phoneEdit->text().trimmed();

    if (name.isEmpty() && phone.isEmpty())
    {
        QMessageBox::warning(this,
                             "Поиск",
                             "Введите имя и/или номер телефона для поиска");
        return;
    }

    auto all = interface.getAll();

    QStringList results;

    for (const auto &p : all)
    {
        QString entryName = QString::fromStdString(p.first);
        QString entryPhone = QString::fromStdString(p.second);

        bool nameMatch  = name.isEmpty()  || entryName.contains(name, Qt::CaseInsensitive);
        bool phoneMatch = phone.isEmpty() || entryPhone.contains(phone);

        if (nameMatch && phoneMatch)
        {
            results << (entryName + " : " + entryPhone);
        }
    }

    if (results.isEmpty())
    {
        QMessageBox::information(this,
                                 "Результат поиска",
                                 "Совпадений не найдено");
        return;
    }

    QString resultText = results.join("\n");

    QMessageBox::information(this, "Результат поиска", resultText);
}


void MainWindow::on_editButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();

    if(!interface.addAbonent(name.toStdString(), phone.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Абонент не найден");
        return;
    }

    updateList();
}


void MainWindow::on_clearButton_clicked()
{
    interface.clear();
    updateList();
}

void MainWindow::on_saveAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить телефонную книгу", "", "Text files (*.txt)");

    if(fileName.isEmpty())
        return;

    if(!interface.save(fileName.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QMessageBox::information(this, "Сохранение", "Данные успешно сохранены");
}


void MainWindow::on_loadAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Загрузить телефонную книгу", "", "Text files (*.txt)");

    if(fileName.isEmpty())
        return;

    if(!interface.load("phonebook.txt"))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить файл");
        return;
    }

    updateList();

    QMessageBox::information(this, "Загрузка", "Данные успешно загружены");
}


void MainWindow::on_mainHelpAction_triggered()
{
    QString text = QString::fromStdString(help.getMainInfo());

    QMessageBox::information(this, "Справка", text);
}


void MainWindow::on_loadHelpAction_triggered()
{
    QString text = QString::fromStdString(help.getLoadInfo());

    QMessageBox::information(this, "Справка", text);
}


void MainWindow::on_authorsHelpAction_triggered()
{
    QString text = QString::fromStdString(help.getAuthorsInfo());

    QMessageBox::information(this, "Справка", text);
}
