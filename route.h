#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <set>
#include <memory>
#include <algorithm>
#include "list.h"
#include "exceptions.h"

/**
 * @class Route
 * @brief Класс, представляющий маршрут общественного транспорта
 * 
 * Хранит информацию о маршруте: номер, тип транспорта, список остановок
 * в порядке следования, дни недели работы маршрута.
 */
class Route {
private:
    int number;                        // Номер маршрута
    std::string vehicleType;            // Тип транспортного средства (Автобус, Трамвай, Троллейбус)
    std::string startStop;              // Начальная остановка
    std::string endStop;                // Конечная остановка
    List<std::string> allStops;         // Список всех остановок в порядке следования
    std::set<int> weekDays;             // Дни недели работы: 1-понедельник, 2-вторник, ..., 7-воскресенье

public:
    /**
     * @brief Конструктор маршрута
     * @param num Номер маршрута
     * @param vType Тип транспортного средства
     * @param stops Список остановок маршрута
     * @param days Дни недели работы маршрута (по умолчанию все дни)
     * @throws ContainerException если список остановок пуст
     */
    Route(int num, const std::string& vType, const List<std::string>& stops,
          const std::set<int>& days = {1,2,3,4,5,6,7});

    /**
     * @brief Проверить, содержит ли маршрут указанную остановку
     * @param stop Название остановки
     * @return true, если остановка есть в маршруте
     */
    bool containsStop(const std::string& stop) const;

    /**
     * @brief Получить позицию остановки в маршруте
     * @param stop Название остановки
     * @return Позиция остановки (0 - первая остановка) или -1, если не найдена
     */
    int getStopPosition(const std::string& stop) const;

    /**
     * @brief Проверить, идет ли остановка stopA раньше stopB в маршруте
     * @param stopA Первая остановка
     * @param stopB Вторая остановка
     * @return true, если stopA идет раньше stopB
     */
    bool isStopBefore(const std::string& stopA, const std::string& stopB) const;

    // Геттеры
    int getNumber() const;
    std::string getVehicleType() const;
    std::string getStartStop() const;
    std::string getEndStop() const;
    const List<std::string>& getAllStops() const;
    const std::set<int>& getWeekDays() const;

    /**
     * @brief Проверить, работает ли маршрут в указанный день недели
     * @param day День недели (1-7)
     * @return true, если маршрут работает в этот день
     */
    bool operatesOnDay(int day) const;

    // Сериализация
    /**
     * @brief Сериализация маршрута в строку
     * @return Строка формата "number|vehicleType|stop1;stop2;...|1,2,3,4,5,6,7"
     */
    std::string serialize() const;

    /**
     * @brief Десериализация маршрута из строки
     * @param data Строка с данными маршрута
     * @return Указатель на созданный маршрут
     */
    static std::shared_ptr<Route> deserialize(const std::string& data);
};

#endif // ROUTE_H