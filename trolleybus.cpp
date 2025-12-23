#include "trolleybus.h"

/**
 * @brief Конструктор троллейбуса
 * @param m Модель троллейбуса
 * @param lp Государственный номер
 * @param cap Вместимость пассажиров (по умолчанию 50)
 * @param volt Напряжение питания в вольтах (по умолчанию 600.0)
 * 
 * Создает объект троллейбуса, который является электрическим транспортом.
 */
Trolleybus::Trolleybus(const std::string& m, const std::string& lp, int cap, double volt)
    : ElectricTransport("Троллейбус", m, lp, cap, volt) {}

