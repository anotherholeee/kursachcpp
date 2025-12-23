#include "stop.h"
#include <sstream>

// Конструктор: инициализирует остановку с заданным ID и названием
Stop::Stop(int stopId, std::string stopName) : id(stopId), name(std::move(stopName)) {}

// Возвращает уникальный идентификатор остановки
int Stop::getId() const {
    return id;
}

// Возвращает название остановки
std::string Stop::getName() const {
    return name;
}

// Сравнение остановок по идентификатору
bool Stop::operator==(const Stop& other) const {
    return id == other.id;
}

// Преобразует остановку в строку формата "id|name" для сохранения
std::string Stop::serialize() const {
    return std::to_string(id) + "|" + name;
}

// Создает объект остановки из строки формата "id|name"
Stop Stop::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string idStr, name;
    std::getline(ss, idStr, '|');  // Читаем ID до разделителя
    std::getline(ss, name);         // Читаем название до конца строки
    return Stop(std::stoi(idStr), name);
}

