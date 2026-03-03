#ifndef UMEMORY_H
#define UMEMORY_H

#include <string>

enum class MemoryState
{
    Off,
    On
};

/**
 * Шаблонынй класс "Память" для хранения одного числа типа T.
 *
 * Память хранит число и состояние. Операции: Store, Recall, Add, Clear, чтение состояния и числа.
 */
template <class T>
class TMemory
{
private:
    T m_number;          // хранимое число
    MemoryState m_state; // сосятоние памяти

public:
    TMemory();

    void store(const T &value);
    T recall() const;
    void add(const T &value);
    void clear();

    T getNumber() const;
    MemoryState getState() const;

    std::string stateString() const;
};

template <class T>
TMemory<T>::TMemory() : m_number(T()), m_state(MemoryState::Off) {}

template <class T>
void TMemory<T>::store(const T &value)
{
    m_number = value;
    m_state = MemoryState::On;
}

template <class T>
T TMemory<T>::recall() const { return m_number; }

template <class T>
void TMemory<T>::add(const T &value)
{
    m_number = m_number + value;
    m_state = MemoryState::On;
}

template <class T>
void TMemory<T>::clear()
{
    m_number = T();
    m_state = MemoryState::Off;
}

template <class T>
T TMemory<T>::getNumber() const { return m_number; }

template <class T>
MemoryState TMemory<T>::getState() const { return m_state; }

template <class T>
std::string TMemory<T>::stateString() const { return (m_state == MemoryState::On) ? "_On" : "_Off"; }

#endif // UMEMORY_H
