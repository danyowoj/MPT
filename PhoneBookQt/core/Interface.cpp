#include "Interface.h"
#include "FileManager.h"
#include "Help.h"

//* Сценарий "Добавить"
bool Interface::addAbonent(const std::string &name, const std::string &phone)
{
    if (name.empty() || phone.empty())
    {
        return false;
    }

    list.add(name, phone);
    return true;
}

//* Сценарий "Удалить"
bool Interface::removeAbonent(const std::string &name, const std::string &phone)
{
    if (name.empty() || phone.empty())
    {
        return false;
    }

    return list.remove(name, phone);
}

//* Сценарий "Изменить"
bool Interface::editAbonent(const std::string &name, const std::string &newPhone)
{
    if (name.empty() || newPhone.empty())
    {
        return false;
    }

    return list.edit(name, newPhone);
}

//* Сценарий "Найти"
std::vector<std::string> Interface::findAbonent(const std::string &name) const
{
    if (name.empty())
    {
        return {};
    }

    return list.find(name);
}

//* Сценарий "Очистить"
void Interface::clear()
{
    list.clear();
}

//* Получение всех записей
const std::multimap<std::string, std::string> &Interface::getAll() const
{
    return list.getAll();
}

//* Сохранение
bool Interface::save(const std::string &filename)
{
    if (filename.empty())
    {
        return false;
    }

    return FileManager::saveToFile(list, filename);
}

//* Загрузка
bool Interface::load(const std::string &filename)
{
    if (filename.empty())
    {
        return false;
    }

    return FileManager::loadFromFile(list, filename);
}

//* Информация

std::string Interface::mainInfo() const
{
    return Help::getMainInfo();
}

std::string Interface::loadInfo() const
{
    return Help::getLoadInfo();
}

std::string Interface::authorsInfo() const
{
    return Help::getAuthorsInfo();
}
