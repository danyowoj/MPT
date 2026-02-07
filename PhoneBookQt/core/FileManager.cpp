#include "FileManager.h"
#include <fstream>
#include <sstream>

// Сохранения данных в файл
bool FileManager::saveToFile(const AbonentList &list, const std::string &filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        return false;
    }

    for (const auto &[name, phone] : list.getAll())
    {
        out << name << ";" << phone << '\n';
    }

    return true;
}

// Загрузка данных из файла
bool FileManager::loadFromFile(AbonentList &list, const std::string &filname)
{
    std::ifstream in(filname);
    if (!in.is_open())
    {
        return false;
    }

    list.clear();

    std::string line;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string name, phone;

        if (std::getline(ss, name, ';') && std::getline(ss, phone))
        {
            list.add(name, phone);
        }
    }

    return true;
}
