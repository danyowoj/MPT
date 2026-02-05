#include <iostream>
#include "AbonentList.h"

int main()
{
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

    return 0;
}
