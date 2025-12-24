#include "electric_transport.h"

// Конструктор электрического транспорта
ElectricTransport::ElectricTransport(const std::string& t, const std::string& m, const std::string& lp, 
                      int cap, double volt)
    : Vehicle(t, m, lp), capacity(cap), voltage(volt) {}

// Возвращает вместимость пассажиров
int ElectricTransport::getCapacity() const {
    return capacity;
}

// Устанавливает вместимость пассажиров
void ElectricTransport::setCapacity(int cap) {
    capacity = cap;
}

// Возвращает напряжение питания в вольтах
double ElectricTransport::getVoltage() const {
    return voltage;
}

// Устанавливает напряжение питания
void ElectricTransport::setVoltage(double volt) {
    voltage = volt;
}

// Сериализация в строку: "type|model|licensePlate|capacity|voltage"
std::string ElectricTransport::serialize() const {
    return Vehicle::serialize() + "|" + std::to_string(capacity) + "|" + 
           std::to_string(voltage);
}

