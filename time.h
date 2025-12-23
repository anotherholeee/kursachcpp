#ifndef TIME_H
#define TIME_H

#include <string>
#include <iostream>
#include "exceptions.h"

/**
 * @class Time
 * @brief Класс для работы со временем в формате 24-часового формата
 * 
 * Представляет время суток в формате часы:минуты (00:00 - 23:59).
 * Поддерживает арифметические операции, сравнение и сериализацию.
 */
class Time {
private:
    int hours;    // Часы (0-23)
    int minutes;  // Минуты (0-59)

    /**
     * @brief Нормализует время, приводя его к диапазону 00:00 - 23:59
     * @param totalMinutes Общее количество минут (может быть отрицательным или больше 1440)
     */
    void normalize(int totalMinutes);

public:
    /**
     * @brief Конструктор из часов и минут
     * @param h Часы (0-23)
     * @param m Минуты (0-59)
     */
    explicit Time(int h = 0, int m = 0);

    /**
     * @brief Конструктор из строки формата "HH:MM"
     * @param timeStr Строка с временем
     * @throws InputException если формат некорректен
     */
    explicit Time(const std::string& timeStr);

    /**
     * @brief Получить общее количество минут с начала суток (0-1439)
     */
    int getTotalMinutes() const;

    /**
     * @brief Получить часы (0-23)
     */
    int getHours() const;

    /**
     * @brief Получить минуты (0-59)
     */
    int getMinutes() const;

    // Операторы сравнения времени
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;

    // Арифметические операции с временем
    Time operator+(int minutesToAdd) const;        // Добавить минуты
    Time operator-(int minutesToSubtract) const;   // Вычесть минуты
    int operator-(const Time& other) const;       // Разница в минутах

    // Операторы ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Time& time);
    friend std::istream& operator>>(std::istream& is, Time& time);

    // Сериализация
    std::string serialize() const;                          // В строку "HH:MM"
    static Time deserialize(const std::string& data);       // Из строки "HH:MM"
};

#endif // TIME_H