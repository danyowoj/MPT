#ifndef THISTORY_H
#define THISTORY_H

#include <string>
#include <vector>

/**
 * @brief Класс для хранения истории вычислений.
 */
class THistory
{
private:
    std::vector<std::string> m_entries;
    size_t m_maxSize;

public:
    explicit THistory(size_t maxSize = 100);

    void addEntry(const std::string &entry);
    std::vector<std::string> getEntries() const;
    void clear();
    std::string getLastEntry() const;
    size_t size() const;
    size_t maxSize() const;
    void setMaxSize(size_t maxSize);
};

#endif // THISTORY_H
