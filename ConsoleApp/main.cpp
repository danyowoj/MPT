#include <iostream>
#include "AbonentList.h"
#include "Interface.h"

void testAbonentList()
{
    std::cout << "=== Тестирование класса AbonentList ===\n";
    AbonentList list;

    //* Добавление записей
    list.add("Ivan", "123456");
    list.add("Ivan", "654321");
    list.add("Anna", "999999");
    list.add("Petr", "111111");
    list.add("Jose", "258852");

    std::cout << "=== Все записи ===\n";
    for (const auto &[name, phone] : list.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    //* Поиск
    std::cout << "\n=== Поиск Ivan ===\n";
    auto phones = list.find("Ivan");
    for (const auto &phone : phones)
    {
        std::cout << phone << '\n';
    }

    //* Редактирование
    list.edit("Anna", "000000");

    //* Удаление
    list.remove("Ivan");

    std::cout << "\n=== После удаления Ivan и редактирования Anna ===\n";
    for (const auto &[name, phone] : list.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    //* Очистка
    list.clear();

    std::cout << "\n=== После очистки ===\n";
    std::cout << "Количество записей: " << list.getAll().size() << '\n';
}

void testInterface()
{
    std::cout << "=== Тестирование класса Interface ===\n";
    Interface ui;

    // Попытка добавления записи с пустыми полями
    if (!ui.addAbonent("", "123"))
    {
        std::cout << "Ошибка: пустое имя\n";
    }

    // Основной сценарий
    ui.addAbonent("Ivan", "111");
    ui.addAbonent("Ivan", "222");
    ui.addAbonent("Anna", "999");

    std::cout << "\n=== Все записи ===\n";
    for (const auto &[name, phone] : ui.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    // Поиск
    std::cout << "\n=== Поиск Ivan ===\n";
    for (const auto &phone : ui.findAbonent("Ivan"))
    {
        std::cout << phone << '\n';
    }

    // Редактирование
    ui.editAbonent("Anna", "000");

    // Удаление
    ui.removeAbonent("Ivan");

    std::cout << "\n=== После изменения Anna и удаления Ivan ===\n";
    for (const auto &[name, phone] : ui.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    // Очистка
    ui.clear();
    std::cout << "\nКоличество записей после очистки: "
              << ui.getAll().size() << '\n';
}

void testFileManager()
{
    Interface ui;

    ui.addAbonent("Ivan", "111");
    ui.addAbonent("Anna", "999");

    std::cout << "\n=== Все записи ===\n";
    for (const auto &[name, phone] : ui.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    ui.save("book.txt");

    ui.addAbonent("Jose", "228");

    std::cout << "\n=== Все записи после редактирования ===\n";
    for (const auto &[name, phone] : ui.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }

    ui.load("book.txt");

    std::cout << "\n=== Все записи после загрузки ===\n";
    for (const auto &[name, phone] : ui.getAll())
    {
        std::cout << name << " : " << phone << '\n';
    }
}

void testHelp()
{
    Interface ui;

    std::cout << ui.info();
}

int main()
{
    // testAbonentList();
    // testInterface();
    // testFileManager();
    testHelp();

    return 0;
}
