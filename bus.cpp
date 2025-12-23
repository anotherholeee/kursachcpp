#include "bus.h"

/**
 * @brief Конструктор автобуса
 * @param m Модель автобуса
 * @param lp Государственный номер
 * @param cap Вместимость пассажиров (по умолчанию 50)
 * @param fuel Тип топлива (по умолчанию "дизель")
 * 
 * Создает объект автобуса, который является топливным транспортом.
 */
Bus::Bus(const std::string& m, const std::string& lp, int cap, const std::string& fuel)
    : FuelTransport("Автобус", m, lp, cap, fuel) {}

