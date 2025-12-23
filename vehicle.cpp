#include "vehicle.h"
#include <sstream>

// Конструктор транспортного средства: инициализирует тип, модель и гос. номер
Vehicle::Vehicle(const std::string& t, const std::string& m, const std::string& lp)
    : type(t), model(m), licensePlate(lp) {}

// Возвращает строку с информацией о транспортном средстве: "Тип Модель Номер"
std::string Vehicle::getInfo() const {
    return type + " " + model + " " + licensePlate;
}

// Возвращает тип транспортного средства (Автобус, Трамвай, Троллейбус)
std::string Vehicle::getType() const {
    return type;
}

// Возвращает модель транспортного средства
std::string Vehicle::getModel() const {
    return model;
}

// Возвращает государственный номер транспортного средства
std::string Vehicle::getLicensePlate() const {
    return licensePlate;
}

// Сериализация транспортного средства в строку формата "type|model|licensePlate"
std::string Vehicle::serialize() const {
    return type + "|" + model + "|" + licensePlate;
}

// Десериализация транспортного средства (не используется напрямую)
// Для создания транспортных средств следует использовать DataManager
std::shared_ptr<Vehicle> Vehicle::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string type, model, licensePlate;
    std::getline(ss, type, '|');
    std::getline(ss, model, '|');
    std::getline(ss, licensePlate);

    throw ContainerException("Используйте DataManager для создания транспортных средств");
}

