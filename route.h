#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <set>
#include <memory>
#include <algorithm>
#include "list.h"
#include "exceptions.h"

// Класс, представляющий маршрут общественного транспорта
// Хранит информацию о маршруте: номер, тип транспорта, список остановок
// в порядке следования, дни недели работы маршрута
class Route {
private:
    int number;                        // Номер маршрута
    std::string vehicleType;            // Тип транспортного средства (Автобус, Трамвай, Троллейбус)
    std::string startStop;              // Начальная остановка
    std::string endStop;                // Конечная остановка
    List<std::string> allStops;         // Список всех остановок в порядке следования
    std::set<int> weekDays;             // Дни недели работы: 1-понедельник, 2-вторник, ..., 7-воскресенье

public:
    // Конструктор маршрута
    // Выбрасывает ContainerException если список остановок пуст
    Route(int num, const std::string& vType, const List<std::string>& stops,
          const std::set<int>& days = {1,2,3,4,5,6,7});

    // Проверить, содержит ли маршрут указанную остановку
    bool containsStop(const std::string& stop) const;

    // Получить позицию остановки в маршруте (0 - первая остановка) или -1, если не найдена
    int getStopPosition(const std::string& stop) const;

    // Проверить, идет ли остановка stopA раньше stopB в маршруте
    bool isStopBefore(const std::string& stopA, const std::string& stopB) const;

    // Геттеры
    int getNumber() const;
    std::string getVehicleType() const;
    std::string getStartStop() const;
    std::string getEndStop() const;
    const List<std::string>& getAllStops() const;
    const std::set<int>& getWeekDays() const;

    // Проверить, работает ли маршрут в указанный день недели (1-7)
    bool operatesOnDay(int day) const;

    // Сериализация маршрута в строку формата "number|vehicleType|stop1;stop2;...|1,2,3,4,5,6,7"
    std::string serialize() const;

    // Десериализация маршрута из строки
    static std::shared_ptr<Route> deserialize(const std::string& data);
};

#endif // ROUTE_H