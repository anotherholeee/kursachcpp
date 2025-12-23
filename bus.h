#ifndef BUS_H
#define BUS_H

#include "fuel_transport.h"

/**
 * @class Bus
 * @brief Класс автобуса (Уровень 3 иерархии)
 * 
 * Наследуется от FuelTransport. Представляет автобус как топливный транспорт.
 */
class Bus : public FuelTransport {
public:
    /**
     * @brief Конструктор автобуса
     * @param m Модель автобуса
     * @param lp Государственный номер
     * @param cap Вместимость пассажиров (по умолчанию 50)
     * @param fuel Тип топлива (по умолчанию "дизель")
     */
    Bus(const std::string& m, const std::string& lp, int cap = 50, const std::string& fuel = "дизель");
};

#endif // BUS_H

