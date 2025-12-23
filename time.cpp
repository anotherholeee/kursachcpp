#include "time.h"
#include <sstream>

/**
 * @brief Нормализует время, приводя его к диапазону 00:00 - 23:59
 * @param totalMinutes Общее количество минут (может быть отрицательным или больше 1440)
 * 
 * Приводит время к 24-часовому формату, обрабатывая переполнения и отрицательные значения.
 * 1440 минут = 24 часа (один полный день)
 */
void Time::normalize(int totalMinutes) {
    // Приводим к диапазону [0, 1439] с учетом отрицательных значений
    totalMinutes = ((totalMinutes % 1440) + 1440) % 1440;
    hours = totalMinutes / 60;      // Вычисляем часы
    minutes = totalMinutes % 60;    // Вычисляем минуты
}

// Конструктор из часов и минут
Time::Time(int h, int m) {
    normalize(h * 60 + m);
}

// Конструктор из строки формата "HH:MM"
Time::Time(const std::string& timeStr) {
    std::stringstream ss(timeStr);
    char colon;
    int h, m;
    // Парсим строку: ожидаем формат "HH:MM"
    if (!(ss >> h >> colon >> m) || colon != ':' || ss.fail() || !ss.eof()) {
        throw InputException("Неверный формат времени: " + timeStr);
    }
    // Проверяем корректность диапазонов
    if (h < 0 || h > 23 || m < 0 || m > 59) {
        throw InputException("Некорректное время: " + timeStr);
    }
    hours = h;
    minutes = m;
}

// Возвращает общее количество минут с начала суток (0-1439)
int Time::getTotalMinutes() const {
    return hours * 60 + minutes;
}

// Возвращает часы (0-23)
int Time::getHours() const {
    return hours;
}

// Возвращает минуты (0-59)
int Time::getMinutes() const {
    return minutes;
}

// Операторы сравнения: сравнивают время по общему количеству минут
bool Time::operator<(const Time& other) const {
    return getTotalMinutes() < other.getTotalMinutes();
}

bool Time::operator<=(const Time& other) const {
    return getTotalMinutes() <= other.getTotalMinutes();
}

bool Time::operator>(const Time& other) const {
    return getTotalMinutes() > other.getTotalMinutes();
}

bool Time::operator>=(const Time& other) const {
    return getTotalMinutes() >= other.getTotalMinutes();
}

bool Time::operator==(const Time& other) const {
    return getTotalMinutes() == other.getTotalMinutes();
}

bool Time::operator!=(const Time& other) const {
    return getTotalMinutes() != other.getTotalMinutes();
}

// Добавляет минуты к времени (с нормализацией через 24 часа)
Time Time::operator+(int minutesToAdd) const {
    return Time(0, getTotalMinutes() + minutesToAdd);
}

// Вычитает минуты из времени (с нормализацией через 24 часа)
Time Time::operator-(int minutesToSubtract) const {
    return Time(0, getTotalMinutes() - minutesToSubtract);
}

// Вычитает одно время из другого, возвращая разницу в минутах
int Time::operator-(const Time& other) const {
    return getTotalMinutes() - other.getTotalMinutes();
}

// Оператор вывода времени в формате "HH:MM" с ведущими нулями
std::ostream& operator<<(std::ostream& os, const Time& time) {
    os << (time.hours < 10 ? "0" : "") << time.hours << ":"
       << (time.minutes < 10 ? "0" : "") << time.minutes;
    return os;
}

// Оператор ввода времени из потока (формат "HH:MM")
std::istream& operator>>(std::istream& is, Time& time) {
    std::string timeStr;
    is >> timeStr;
    time = Time(timeStr);  // Используем конструктор из строки
    return is;
}

// Сериализация времени в строку формата "HH:MM"
std::string Time::serialize() const {
    return (hours < 10 ? "0" : "") + std::to_string(hours) + ":" +
           (minutes < 10 ? "0" : "") + std::to_string(minutes);
}

// Десериализация времени из строки формата "HH:MM"
Time Time::deserialize(const std::string& data) {
    return Time(data);
}