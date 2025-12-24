#ifndef STOP_H
#define STOP_H

#include <string>

// Класс, представляющий остановку общественного транспорта
// Хранит информацию об остановке: уникальный идентификатор и название.
// Поддерживает сериализацию для сохранения и загрузки данных
class Stop {
private:
    int id;              // Уникальный идентификатор остановки
    std::string name;    // Название остановки

public:
    Stop(int stopId, std::string stopName);

    int getId() const;

    std::string getName() const;

    // Оператор сравнения остановок по ID
    bool operator==(const Stop& other) const;

    // Сериализация остановки в строку формата "id|name"
    std::string serialize() const;

    // Десериализация остановки из строки формата "id|name"
    static Stop deserialize(const std::string& data);
};

#endif // STOP_H

