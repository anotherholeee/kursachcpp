#ifndef TROLLEYBUS_H
#define TROLLEYBUS_H

#include "electric_transport.h"

// Класс троллейбуса (Уровень 3 иерархии)
// Наследуется от ElectricTransport. Представляет троллейбус как электрический транспорт
class Trolleybus : public ElectricTransport {
public:
    Trolleybus(const std::string& m, const std::string& lp, int cap = 50, double volt = 600.0);
};

#endif // TROLLEYBUS_H

