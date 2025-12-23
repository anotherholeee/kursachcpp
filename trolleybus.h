#ifndef TROLLEYBUS_H
#define TROLLEYBUS_H

#include "electric_transport.h"

/**
 * @class Trolleybus
 * @brief Класс троллейбуса (Уровень 3 иерархии)
 * 
 * Наследуется от ElectricTransport. Представляет троллейбус как электрический транспорт.
 */
class Trolleybus : public ElectricTransport {
public:
    /**
     * @brief Конструктор троллейбуса
     * @param m Модель троллейбуса
     * @param lp Государственный номер
     * @param cap Вместимость пассажиров (по умолчанию 50)
     * @param volt Напряжение питания в вольтах (по умолчанию 600.0)
     */
    Trolleybus(const std::string& m, const std::string& lp, int cap = 50, double volt = 600.0);
};

#endif // TROLLEYBUS_H

