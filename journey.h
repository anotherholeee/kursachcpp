#ifndef JOURNEY_H
#define JOURNEY_H

#include <string>
#include <memory>
#include <iostream>
#include "list.h"
#include "trip.h"
#include "time.h"

/**
 * @class Journey
 * @brief Класс, представляющий поездку с возможными пересадками
 * 
 * Хранит информацию о поездке, которая может состоять из нескольких рейсов
 * с пересадками между ними. Содержит список рейсов, точки пересадок,
 * время начала и окончания поездки.
 */
class Journey {
private:
    List<std::shared_ptr<Trip>> trips;      // Список рейсов, составляющих поездку
    List<std::string> transferPoints;      // Остановки, где происходят пересадки
    Time startTime;                         // Время начала поездки
    Time endTime;                           // Время окончания поездки
    int transferCount;                      // Количество пересадок

public:
    /**
     * @brief Конструктор поездки
     * @param tripList Список рейсов, составляющих поездку
     * @param transfers Список остановок, где происходят пересадки
     * @param start Время начала поездки
     * @param end Время окончания поездки
     */
    Journey(const List<std::shared_ptr<Trip>>& tripList,
            const List<std::string>& transfers,
            Time start, Time end);

    /**
     * @brief Получить общую продолжительность поездки в минутах
     */
    int getTotalDuration() const;

    /**
     * @brief Получить количество пересадок
     */
    int getTransferCount() const;

    /**
     * @brief Получить время начала поездки
     */
    Time getStartTime() const;

    /**
     * @brief Получить время окончания поездки
     */
    Time getEndTime() const;

    /**
     * @brief Получить список рейсов, составляющих поездку
     */
    const List<std::shared_ptr<Trip>>& getTrips() const;

    /**
     * @brief Получить список остановок, где происходят пересадки
     */
    const List<std::string>& getTransferPoints() const;

    /**
     * @brief Вывести информацию о поездке в консоль
     */
    void display() const;
};

#endif // JOURNEY_H