#ifndef CALCCOMMANDS_H
#define CALCCOMMANDS_H

// Команды редактора (0..13) определены в UAEditor.h
// Здесь определяем дополнительные команды калькулятора

// Операции
const unsigned cAdd = 100;
const unsigned cSub = 101;
const unsigned cMul = 102;
const unsigned cDiv = 103;
const unsigned cEqual = 104;

// Функции
const unsigned cRev = 105;
const unsigned cSqr = 106;

// Память
const unsigned cMemStore = 110;   // MS
const unsigned cMemRestore = 111; // MR
const unsigned cMemAdd = 112;     // M+
const unsigned cMemClear = 113;   // MC

// Буфер обмена
const unsigned cCopy = 120;
const unsigned cPaste = 121;

// Режимы (для переключения)
const unsigned cModePNumber = 130;
const unsigned cModeFrac = 131;
const unsigned cModeComplex = 132;

#endif
