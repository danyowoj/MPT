#include "TProc.h"

TProc::TProc(TANumber *L, TANumber *R)
    : fOperation(None)
{
    // Сохраняем образец нуля для сброса
    fZero = L->zero();
    // Копируем операнды
    fLop_Res = L->copy();
    fRop = R->copy();
    fError = "";
}

TProc::~TProc()
{
    delete fLop_Res;
    delete fRop;
    delete fZero;
}

void TProc::ReSet()
{
    delete fLop_Res;
    delete fRop;
    fLop_Res = fZero->copy();
    fRop = fZero->copy();
    fOperation = None;
    fError = "";
}

void TProc::OprtnClear()
{
    fOperation = None;
}

void TProc::OprtnRun()
{
    if (fOperation == None)
        return;

    TANumber *res = nullptr;
    switch (fOperation)
    {
    case Add:
        res = fLop_Res->add(*fRop);
        break;
    case Sub:
        res = fLop_Res->sub(*fRop);
        break;
    case Mul:
        res = fLop_Res->mul(*fRop);
        break;
    case Dvd:
        if (fRop->isZero())
        {
            fError = "Division by zero";
            return;
        }
        res = fLop_Res->div(*fRop);
        break;
    default:
        return;
    }
    delete fLop_Res;
    fLop_Res = res;
    // Очищаем ошибку, если операция выполнена
    fError = "";
}

void TProc::FuncRun(TFunc func)
{
    TANumber *res = nullptr;
    switch (func)
    {
    case Rev:
        if (fRop->isZero())
        {
            fError = "Division by zero (reciprocal)";
            return;
        }
        res = fRop->rev();
        break;
    case Sqr:
        res = fRop->sqr();
        break;
    default:
        return;
    }
    delete fRop;
    fRop = res;
    fError = "";
}

void TProc::SetLOpnd(TANumber *r)
{
    delete fLop_Res;
    fLop_Res = r->copy();
}

TANumber *TProc::GetLOpnd_Res() const
{
    return fLop_Res->copy();
}

void TProc::SetROpnd(TANumber *r)
{
    delete fRop;
    fRop = r->copy();
}

TANumber *TProc::GetRopd() const
{
    return fRop->copy();
}

void TProc::SetOperation(TOprtn op)
{
    fOperation = op;
}

TOprtn TProc::GetOperation() const
{
    return fOperation;
}

std::string TProc::GetError() const
{
    return fError;
}

void TProc::ClearError()
{
    fError = "";
}
