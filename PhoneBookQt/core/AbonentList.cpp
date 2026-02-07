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

//* Удаление абонента по имени и номеру
bool AbonentList::remove(const std::string &name, const std::string &phone)
{
    auto range = data.equal_range(name);

    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second == phone)  // проверяем номер телефона
        {
            data.erase(it);       // удаляем конкретную пару
            return true;
        }
    }

    return false; // не найден абонент с таким именем И номером
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
