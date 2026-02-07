#ifndef INTERFACE_H
#define INTERFACE_H

#include "AbonentList.h"
#include <string>
#include <vector>

class Interface
{
public:
    bool addAbonent(const std::string &name, const std::string &phone);
    bool removeAbonent(const std::string &name, const std::string &phone);
    bool editAbonent(const std::string &name, const std::string &newPhone);
    std::vector<std::string> findAbonent(const std::string &name) const;
    void clear();
    const std::multimap<std::string, std::string> &getAll() const;
    bool save(const std::string &filename);
    bool load(const std::string &filename);

    std::string mainInfo() const;
    std::string loadInfo() const;
    std::string authorsInfo() const;

private:
    AbonentList list;
};

#endif
