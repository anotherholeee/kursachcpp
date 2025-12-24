#ifndef ELECTRIC_TRANSPORT_H
#define ELECTRIC_TRANSPORT_H

#include "vehicle.h"

// Промежуточный класс для электрического транспорта (Уровень 2 иерархии)
// Наследуется от Vehicle и добавляет характеристики электрического транспорта:
// вместимость пассажиров и напряжение питания. Базовый класс для трамваев и троллейбусов
class ElectricTransport : public Vehicle {
protected:
    int capacity;      // Вместимость пассажиров
    double voltage;    // Напряжение питания в вольтах

public:
    ElectricTransport(const std::string& t, const std::string& m, const std::string& lp, 
                      int cap = 50, double volt = 600.0);

    // Геттеры и сеттеры
    int getCapacity() const;
    void setCapacity(int cap);
    double getVoltage() const;
    void setVoltage(double volt);

    // Сериализация в строку формата "type|model|licensePlate|capacity|voltage"
    std::string serialize() const override;
};

#endif // ELECTRIC_TRANSPORT_H

