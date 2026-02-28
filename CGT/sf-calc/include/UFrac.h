#ifndef UFRAC_H
#define UFRAC_H

#include <cstdint>
#include <string>
#include <stdexcept>

/**
 * @brief Класс "Простая дробь"
 *
 * Хранит числитель и знаменатель в сокращенном виде.
 * Знак всегда в числителе, знаменатель положительный.
 */
class TFrac
{
private:
    int64_t numerator;   // числитель
    int64_t denominator; // знаменатель

    /** @brief Приведение дроби к нормальному виду.
     *
     * Сокращение на НОД ->
     * Знак переносится в числитель ->
     * Если числитель = 0, знаменатель = 1.
     */
    void normalize();

public:
    // Конструкторы
    TFrac();
    TFrac(int64_t num, int64_t den);
    explicit TFrac(const std::string &str);
    TFrac(const TFrac &other) = default;

    // Арифметические операции
    TFrac add(const TFrac &other) const;
    TFrac subtract(const TFrac &other) const;
    TFrac multiply(const TFrac &other) const;
    TFrac divide(const TFrac &other) const;
    TFrac square() const;     // x^2
    TFrac reciprocal() const; // 1/x
    TFrac negate() const;     // унарный минус

    // Операции сравнения
    bool equals(const TFrac &other) const;
    bool greaterThan(const TFrac &other) const;

    // Геттеры
    int64_t getNumerator() const;
    int64_t getDenominator() const;
    std::string getNumeratorString() const;
    std::string getDenominatorString() const;
    std::string toString() const;

    // Необязательные методы
    double toDouble() const;           // перевод в десятичную дробь
    bool isTerminatingDecimal() const; // конечная десятичная дробь
};

// Глобальный оператор сложения для TFrac
TFrac operator+(const TFrac &lhs, const TFrac &rhs);

int64_t gcd(int64_t a, int64_t b);

#endif // UFRAC_H
