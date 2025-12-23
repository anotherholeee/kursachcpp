#ifndef DRIVER_SCHEDULE_H
#define DRIVER_SCHEDULE_H

#include <unordered_map>
#include <memory>
#include <algorithm>
#include "list.h"
#include "driver.h"
#include "trip.h"
#include "time.h"

/**
 * @class DriverSchedule
 * @brief Класс для управления расписанием водителей
 * 
 * Отслеживает назначенные рейсы для каждого водителя, проверяет доступность
 * водителей в заданное время и соблюдение лимита рабочих часов.
 */
class DriverSchedule {
private:
    // Карта: водитель -> список назначенных рейсов
    std::unordered_map<std::shared_ptr<Driver>,
                       List<std::shared_ptr<Trip>>> driverTrips;
    const int MAX_WORKING_HOURS = 12 * 60;  // Максимальное рабочее время: 12 часов в минутах

public:
    /**
     * @brief Назначить рейс водителю
     * @param driver Водитель
     * @param trip Рейс для назначения
     */
    void assignTripToDriver(std::shared_ptr<Driver> driver,
                           std::shared_ptr<Trip> trip);

    /**
     * @brief Удалить рейс из расписания водителя
     * @param driver Водитель
     * @param tripId Идентификатор рейса для удаления
     */
    void removeTripFromDriver(std::shared_ptr<Driver> driver, int tripId);

    /**
     * @brief Проверить доступность водителя в указанный временной интервал
     * @param driver Водитель
     * @param startTime Начало временного интервала
     * @param endTime Конец временного интервала
     * @return true, если водитель свободен в указанное время
     */
    bool isDriverAvailable(std::shared_ptr<Driver> driver,
                          const Time& startTime,
                          const Time& endTime) const;

    /**
     * @brief Проверить соблюдение лимита рабочих часов водителя
     * @param driver Водитель
     * @return true, если общее время работы не превышает лимит
     */
    bool checkWorkingHoursCompliance(std::shared_ptr<Driver> driver) const;

    /**
     * @brief Получить список рейсов водителя
     * @param driver Водитель
     * @return Список рейсов, назначенных водителю
     */
    List<std::shared_ptr<Trip>> getDriverTrips(
        std::shared_ptr<Driver> driver) const;

    /**
     * @brief Получить общее время работы водителя в минутах
     * @param driver Водитель
     * @return Общее время работы в минутах
     */
    int getTotalWorkingMinutes(std::shared_ptr<Driver> driver) const;
};

#endif // DRIVER_SCHEDULE_H