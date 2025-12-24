#ifndef JOURNEY_H
#define JOURNEY_H

#include <string>
#include <memory>
#include <iostream>
#include "list.h"
#include "trip.h"
#include "time.h"

// Класс, представляющий поездку с возможными пересадками
// Хранит информацию о поездке, которая может состоять из нескольких рейсов
// с пересадками между ними. Содержит список рейсов, точки пересадок,
// время начала и окончания поездки
class Journey {
private:
    List<std::shared_ptr<Trip>> trips;      // Список рейсов, составляющих поездку
    List<std::string> transferPoints;      // Остановки, где происходят пересадки
    Time startTime;                         // Время начала поездки
    Time endTime;                           // Время окончания поездки
    int transferCount;                      // Количество пересадок

public:
    Journey(const List<std::shared_ptr<Trip>>& tripList,
            const List<std::string>& transfers,
            Time start, Time end);

    // Получить общую продолжительность поездки в минутах
    int getTotalDuration() const;

    // Получить количество пересадок
    int getTransferCount() const;

    // Получить время начала поездки
    Time getStartTime() const;

    // Получить время окончания поездки
    Time getEndTime() const;

    // Получить список рейсов, составляющих поездку
    const List<std::shared_ptr<Trip>>& getTrips() const;

    // Получить список остановок, где происходят пересадки
    const List<std::string>& getTransferPoints() const;

    // Вывести информацию о поездке в консоль
    void display() const;
};

#endif // JOURNEY_H