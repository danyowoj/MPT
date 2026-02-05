#include "AbonentList.h"

//* Добавление абонента
void AbonentList::add(const std::string &name, const std::string &phone)
{
    data.insert({name, phone});
}

//* Поиск всех абонентов с данным именем
std::vector<std::string> AbonentList::find(const std::string &name) const
{
    std::vector<std::string> result;

    auto range = data.equal_range(name);
    for (auto it = range.first; it != range.second; ++it)
    {
        result.push_back(it->second);
    }

    return result;
}

//* Удаление всех записей с данным именем
bool AbonentList::remove(const std::string &name)
{
    auto range = data.equal_range(name);

    if (range.first == range.second)
    {
        return false; // ничего не найдено
    }

    data.erase(range.first, range.second);
    return true;
}

//* Редактирование первой найденой записи
bool AbonentList::edit(const std::string &name, const std::string &newPhone)
{
    auto range = data.equal_range(name);

    if (range.first == range.second)
    {
        return false; // ничего не найдено
    }

    range.first->second = newPhone;
    return true;
}

//* Очистка книги
void AbonentList::clear()
{
    data.clear();
}

//* Получение всех записей
const std::multimap<std::string, std::string> &AbonentList::getAll() const
{
    return data;
}
