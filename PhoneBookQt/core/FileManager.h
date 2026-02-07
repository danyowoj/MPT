#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "AbonentList.h"

class FileManager
{
public:
    static bool saveToFile(const AbonentList &list, const std::string &filename);
    static bool loadFromFile(AbonentList &list, const std::string &filename);
};

#endif
