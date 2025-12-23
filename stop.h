#ifndef STOP_H
#define STOP_H

#include <string>

/**
 * @class Stop
 * @brief Класс, представляющий остановку общественного транспорта
 * 
 * Хранит информацию об остановке: уникальный идентификатор и название.
 * Поддерживает сериализацию для сохранения и загрузки данных.
 */
class Stop {
private:
    int id;              // Уникальный идентификатор остановки
    std::string name;    // Название остановки

public:
    /**
     * @brief Конструктор остановки
     * @param stopId Уникальный идентификатор остановки
     * @param stopName Название остановки
     */
    Stop(int stopId, std::string stopName);

    /**
     * @brief Получить идентификатор остановки
     * @return Идентификатор остановки
     */
    int getId() const;

    /**
     * @brief Получить название остановки
     * @return Название остановки
     */
    std::string getName() const;

    /**
     * @brief Оператор сравнения остановок
     * @param other Другая остановка для сравнения
     * @return true, если остановки имеют одинаковый ID
     */
    bool operator==(const Stop& other) const;

    /**
     * @brief Сериализация остановки в строку
     * @return Строка в формате "id|name"
     */
    std::string serialize() const;

    /**
     * @brief Десериализация остановки из строки
     * @param data Строка в формате "id|name"
     * @return Объект Stop
     */
    static Stop deserialize(const std::string& data);
};

#endif // STOP_H

