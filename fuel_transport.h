#ifndef FUEL_TRANSPORT_H
#define FUEL_TRANSPORT_H

#include "vehicle.h"

// Промежуточный класс для топливного транспорта (Уровень 2 иерархии)
// Наследуется от Vehicle и добавляет характеристики топливного транспорта:
// вместимость пассажиров и тип топлива. Базовый класс для автобусов
class FuelTransport : public Vehicle {
protected:
    int capacity;              // Вместимость пассажиров
    std::string fuelType;      // Тип топлива (бензин, дизель, газ)

public:
    FuelTransport(const std::string& t, const std::string& m, const std::string& lp, 
                  int cap = 50, const std::string& fuel = "дизель");

    // Геттеры и сеттеры
    int getCapacity() const;
    void setCapacity(int cap);
    std::string getFuelType() const;
    void setFuelType(const std::string& fuel);

    // Сериализация в строку формата "type|model|licensePlate|capacity|fuelType"
    std::string serialize() const override;
};

#endif // FUEL_TRANSPORT_H

