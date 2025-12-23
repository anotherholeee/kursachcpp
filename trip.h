#ifndef TRIP_H
#define TRIP_H

#include <memory>
#include <map>
#include <string>
#include "route.h"
#include "vehicle.h"
#include "driver.h"
#include "time.h"
#include "exceptions.h"

class TransportSystem;

/**
 * @class Trip
 * @brief Класс, представляющий рейс общественного транспорта
 * 
 * Содержит информацию о рейсе: маршрут, транспортное средство, водитель,
 * время отправления, расписание прибытия на остановки и день недели.
 */
class Trip {
private:
    int tripId;                                    // Уникальный идентификатор рейса
    std::shared_ptr<Route> route;                  // Маршрут рейса
    std::shared_ptr<Vehicle> vehicle;              // Транспортное средство
    std::shared_ptr<Driver> driver;                // Водитель
    Time startTime;                                // Время отправления
    std::map<std::string, Time> schedule;         // Расписание: остановка -> время прибытия
    int weekDay;                                   // День недели: 1-понедельник, 2-вторник, ..., 7-воскресенье

public:
    /**
     * @brief Конструктор рейса
     * @param id Уникальный идентификатор рейса
     * @param r Маршрут рейса
     * @param v Транспортное средство
     * @param d Водитель
     * @param start Время отправления
     * @param day День недели (1-7, по умолчанию 1)
     * @throws InputException если день недели некорректен
     */
    Trip(int id, std::shared_ptr<Route> r, std::shared_ptr<Vehicle> v,
         std::shared_ptr<Driver> d, const Time& start, int day = 1);

    /**
     * @brief Установить время прибытия на остановку
     * @param stop Название остановки
     * @param time Время прибытия
     */
    void setArrivalTime(const std::string& stop, const Time& time);

    /**
     * @brief Получить время прибытия на остановку
     * @param stop Название остановки
     * @return Время прибытия
     * @throws ContainerException если остановка не найдена в расписании
     */
    Time getArrivalTime(const std::string& stop) const;

    /**
     * @brief Проверить наличие остановки в расписании
     * @param stop Название остановки
     * @return true, если остановка есть в расписании
     */
    bool hasStop(const std::string& stop) const;

    // Геттеры
    int getTripId() const;
    std::shared_ptr<Route> getRoute() const;
    std::shared_ptr<Vehicle> getVehicle() const;
    std::shared_ptr<Driver> getDriver() const;
    Time getStartTime() const;
    const std::map<std::string, Time>& getSchedule() const;
    int getWeekDay() const;

    /**
     * @brief Получить примерное время окончания рейса
     * @return Время отправления + 60 минут
     */
    Time getEstimatedEndTime() const;

    // Сериализация
    std::string serialize() const;
    static std::shared_ptr<Trip> deserialize(const std::string& data, TransportSystem* system = nullptr);
};

#endif // TRIP_H

