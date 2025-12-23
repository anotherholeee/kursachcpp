#include "tram.h"

/**
 * @brief Конструктор трамвая
 * @param m Модель трамвая
 * @param lp Государственный номер
 * @param cap Вместимость пассажиров (по умолчанию 100)
 * @param volt Напряжение питания в вольтах (по умолчанию 600.0)
 * 
 * Создает объект трамвая, который является электрическим транспортом.
 */
Tram::Tram(const std::string& m, const std::string& lp, int cap, double volt)
    : ElectricTransport("Трамвай", m, lp, cap, volt) {}

