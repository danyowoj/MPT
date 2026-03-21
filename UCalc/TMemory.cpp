#include "TMemory.h"
#include <cassert>

TMemory::TMemory(const TANumber &n) : fMem(n.copy()), fIsOn(false) {}

TMemory::~TMemory()
{
    delete fMem;
    fMem = nullptr;
}

void TMemory::store(const TANumber &n)
{
    delete fMem;
    fMem = n.copy();
    fIsOn = true;
}

void TMemory::add(const TANumber &n)
{
    if (!fIsOn)
        return;
    if (!fMem)
        return;
    TANumber *temp = fMem->add(n);
    delete fMem;
    fMem = temp;
}

void TMemory::clear()
{
    delete fMem;
    fMem = nullptr;
    fIsOn = false;
}

TANumber *TMemory::restore() const
{
    if (!fIsOn || !fMem)
        return nullptr;
    return fMem->copy();
}

std::string TMemory::getStateString() const
{
    return fIsOn ? "M" : "";
}
