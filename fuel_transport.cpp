#include "fuel_transport.h"

/**
 * @brief Конструктор топливного транспорта
 * @param t Тип транспорта
 * @param m Модель
 * @param lp Государственный номер
 * @param cap Вместимость пассажиров
 * @param fuel Тип топлива (бензин, дизель, газ)
 * 
 * Создает объект топливного транспорта с заданными параметрами.
 */
FuelTransport::FuelTransport(const std::string& t, const std::string& m, const std::string& lp, 
                  int cap, const std::string& fuel)
    : Vehicle(t, m, lp), capacity(cap), fuelType(fuel) {}

// Возвращает вместимость пассажиров
int FuelTransport::getCapacity() const {
    return capacity;
}

// Устанавливает вместимость пассажиров
void FuelTransport::setCapacity(int cap) {
    capacity = cap;
}

// Возвращает тип топлива
std::string FuelTransport::getFuelType() const {
    return fuelType;
}

// Устанавливает тип топлива
void FuelTransport::setFuelType(const std::string& fuel) {
    fuelType = fuel;
}

// Сериализация в строку: "type|model|licensePlate|capacity|fuelType"
std::string FuelTransport::serialize() const {
    return Vehicle::serialize() + "|" + std::to_string(capacity) + "|" + fuelType;
}

