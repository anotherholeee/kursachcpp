#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <memory>
#include "exceptions.h"

/**
 * @class Vehicle
 * @brief Базовый класс для транспортного средства (Уровень 1 иерархии)
 * 
 * Представляет базовую информацию о транспортном средстве:
 * тип, модель и государственный номер. Является базовым классом
 * для всех видов транспорта (автобусы, трамваи, троллейбусы).
 */
class Vehicle {
protected:
    std::string type;          // Тип транспортного средства (Автобус, Трамвай, Троллейбус)
    std::string model;         // Модель транспортного средства
    std::string licensePlate;  // Государственный номер

public:
    /**
     * @brief Конструктор транспортного средства
     * @param t Тип транспорта
     * @param m Модель
     * @param lp Государственный номер
     */
    Vehicle(const std::string& t, const std::string& m, const std::string& lp);

    virtual ~Vehicle() = default;

    /**
     * @brief Получить строку с информацией о транспортном средстве
     * @return Строка формата "Тип Модель Номер"
     */
    std::string getInfo() const;

    // Геттеры
    std::string getType() const;
    std::string getModel() const;
    std::string getLicensePlate() const;

    /**
     * @brief Сериализация транспортного средства в строку
     * @return Строка формата "type|model|licensePlate"
     */
    virtual std::string serialize() const;

    /**
     * @brief Десериализация транспортного средства (не используется напрямую)
     * @param data Строка с данными транспортного средства
     * @return Указатель на транспортное средство
     * @throws ContainerException всегда (используйте DataManager для создания)
     */
    static std::shared_ptr<Vehicle> deserialize(const std::string& data);
};

#endif // VEHICLE_H

