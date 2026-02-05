#ifndef ABONENTLIST_H
#define ABONENTLIST_H

#include <map>
#include <string>
#include <vector>

class AbonentList
{
public:
    void add(const std::string &name, const std::string &phone);
    std::vector<std::string> find(const std::string &name) const;
    bool remove(const std::string &name);
    bool edit(const std::string &name, const std::string &newPhone);
    void clear();
    const std::multimap<std::string, std::string> &getAll() const;

private:
    std::multimap<std::string, std::string> data;
};

#endif
