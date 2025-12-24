#ifndef DRIVER_SCHEDULE_H
#define DRIVER_SCHEDULE_H

#include <unordered_map>
#include <memory>
#include <algorithm>
#include "list.h"
#include "driver.h"
#include "trip.h"
#include "time.h"

// Класс для управления расписанием водителей
// Отслеживает назначенные рейсы для каждого водителя, проверяет доступность
// водителей в заданное время и соблюдение лимита рабочих часов
class DriverSchedule {
private:
    // Карта: водитель -> список назначенных рейсов
    std::unordered_map<std::shared_ptr<Driver>,
                       List<std::shared_ptr<Trip>>> driverTrips;
    const int MAX_WORKING_HOURS = 12 * 60;  // Максимальное рабочее время: 12 часов в минутах

public:
    // Назначить рейс водителю
    void assignTripToDriver(std::shared_ptr<Driver> driver,
                           std::shared_ptr<Trip> trip);

    // Удалить рейс из расписания водителя
    void removeTripFromDriver(std::shared_ptr<Driver> driver, int tripId);

    // Проверить доступность водителя в указанный временной интервал
    bool isDriverAvailable(std::shared_ptr<Driver> driver,
                          const Time& startTime,
                          const Time& endTime) const;

    // Проверить соблюдение лимита рабочих часов водителя
    bool checkWorkingHoursCompliance(std::shared_ptr<Driver> driver) const;

    // Получить список рейсов водителя
    List<std::shared_ptr<Trip>> getDriverTrips(
        std::shared_ptr<Driver> driver) const;

    // Получить общее время работы водителя в минутах
    int getTotalWorkingMinutes(std::shared_ptr<Driver> driver) const;
};

#endif // DRIVER_SCHEDULE_H