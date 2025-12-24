#include "fuel_transport.h"

// Конструктор топливного транспорта
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

