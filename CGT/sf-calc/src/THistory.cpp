#include "../include/THistory.h"

THistory::THistory(size_t maxSize) : m_maxSize(maxSize) {}

void THistory::addEntry(const std::string &entry)
{
    if (entry.empty())
        return;
    m_entries.push_back(entry);
    if (m_entries.size() > m_maxSize)
        m_entries.erase(m_entries.begin());
}

std::vector<std::string> THistory::getEntries() const { return m_entries; }

void THistory::clear() { m_entries.clear(); }

std::string THistory::getLastEntry() const
{
    if (m_entries.empty())
        return "";
    return m_entries.back();
}

size_t THistory::size() const { return m_entries.size(); }

size_t THistory::maxSize() const { return m_maxSize; }

void THistory::setMaxSize(size_t maxSize)
{
    m_maxSize = maxSize;
    while (m_entries.size() > m_maxSize)
        m_entries.erase(m_entries.begin());
}
