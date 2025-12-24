#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <memory>
#include "exceptions.h"

// Базовый класс для транспортного средства (Уровень 1 иерархии)
// Представляет базовую информацию о транспортном средстве:
// тип, модель и государственный номер. Является базовым классом
// для всех видов транспорта (автобусы, трамваи, троллейбусы)
class Vehicle {
protected:
    std::string type;          // Тип транспортного средства (Автобус, Трамвай, Троллейбус)
    std::string model;         // Модель транспортного средства
    std::string licensePlate;  // Государственный номер

public:
    Vehicle(const std::string& t, const std::string& m, const std::string& lp);

    virtual ~Vehicle() = default;

    // Получить строку с информацией о транспортном средстве формата "Тип Модель Номер"
    std::string getInfo() const;

    // Геттеры
    std::string getType() const;
    std::string getModel() const;
    std::string getLicensePlate() const;

    // Сериализация транспортного средства в строку формата "type|model|licensePlate"
    virtual std::string serialize() const;

    // Десериализация транспортного средства (не используется напрямую)
    // Выбрасывает ContainerException всегда (используйте DataManager для создания)
    static std::shared_ptr<Vehicle> deserialize(const std::string& data);
};

#endif // VEHICLE_H

