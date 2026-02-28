#ifndef UFRAC_H
#define UFRAC_H

#include <cstdint>
#include <string>
#include <stdexcept>

enum class DisplayFormat
{
    Fraction, // всегда в виде дробт
    Decimal   // десятичное, если возможно, иначе — дробь
};

/**
 * @brief Класс "Простая дробь"
 *
 * Хранит числитель и знаменатель в сокращенном виде.
 * Знак всегда в числителе, знаменатель положительный.
 */
class TFrac
{
private:
    int64_t m_num; // числитель
    int64_t m_den; // знаменатель

    /** @brief Приведение дроби к нормальному виду.
     *
     * Сокращение на НОД ->
     * Знак переносится в числитель ->
     * Если числитель = 0, знаменатель = 1.
     */
    void normalize();

public:
    // Конструкторы
    TFrac() : m_num(0), m_den(1) {}
    TFrac(int64_t num, int64_t den);
    explicit TFrac(const std::string &str);
    TFrac(const TFrac &) = default;

    // Арифметические операции
    TFrac add(const TFrac &other) const;
    TFrac subtract(const TFrac &other) const;
    TFrac multiply(const TFrac &other) const;
    TFrac divide(const TFrac &other) const; // бросает исключение при делении на 0
    TFrac square() const;                   // x^2
    TFrac reciprocal() const;               // 1/x, бросает исключение при делении на 0
    TFrac negate() const;                   // унарный минус

    // Операции сравнения
    bool equals(const TFrac &other) const;
    bool greaterThan(const TFrac &other) const;

    // Геттеры
    int64_t numerator() const { return m_num; }
    int64_t denominator() const { return m_den; }
    bool isZero() const { return m_num == 0; }

    std::string toString(DisplayFormat fmt = DisplayFormat::Fraction) const;

    // Десятичное представление
    double toDouble() const;           // перевод в десятичную дробь
    bool isTerminatingDecimal() const; // конечная десятичная дробь

    // Операторы
    friend TFrac operator+(const TFrac &lhs, const TFrac &rhs) { return lhs.add(rhs); }
    friend TFrac operator-(const TFrac &lhs, const TFrac &rhs) { return lhs.subtract(rhs); }
    friend TFrac operator*(const TFrac &lhs, const TFrac &rhs) { return lhs.multiply(rhs); }
    friend TFrac operator/(const TFrac &lhs, const TFrac &rhs) { return lhs.divide(rhs); }
    friend bool operator==(const TFrac &lhs, const TFrac &rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const TFrac &lhs, const TFrac &rhs) { return !lhs.equals(rhs); }
    friend bool operator<(const TFrac &lhs, const TFrac &rhs) { return !lhs.greaterThan(rhs) && !lhs.equals(rhs); }
    friend bool operator>(const TFrac &lhs, const TFrac &rhs) { return lhs.greaterThan(rhs); }
    friend bool operator<=(const TFrac &lhs, const TFrac &rhs) { return !lhs.greaterThan(rhs); }
    friend bool operator>=(const TFrac &lhs, const TFrac &rhs) { return lhs.greaterThan(rhs) || lhs.equals(rhs); }
};

static int64_t gcd(int64_t a, int64_t b);

#endif // UFRAC_H
