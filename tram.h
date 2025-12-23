#ifndef TRAM_H
#define TRAM_H

#include "electric_transport.h"

/**
 * @class Tram
 * @brief Класс трамвая (Уровень 3 иерархии)
 * 
 * Наследуется от ElectricTransport. Представляет трамвай как электрический транспорт.
 */
class Tram : public ElectricTransport {
public:
    /**
     * @brief Конструктор трамвая
     * @param m Модель трамвая
     * @param lp Государственный номер
     * @param cap Вместимость пассажиров (по умолчанию 100)
     * @param volt Напряжение питания в вольтах (по умолчанию 600.0)
     */
    Tram(const std::string& m, const std::string& lp, int cap = 100, double volt = 600.0);
};

#endif // TRAM_H

