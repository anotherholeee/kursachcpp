#include "driver_schedule.h"

// Назначает рейс водителю
void DriverSchedule::assignTripToDriver(std::shared_ptr<Driver> driver,
                           std::shared_ptr<Trip> trip) {
    driverTrips[driver].push_back(trip);
}

// Удаляет рейс из расписания водителя
void DriverSchedule::removeTripFromDriver(std::shared_ptr<Driver> driver, int tripId) {
    auto& trips = driverTrips[driver];
    for (auto it = trips.begin(); it != trips.end(); ) {
        if ((*it)->getTripId() == tripId) {
            it = trips.erase(it);
        } else {
            ++it;
        }
    }
}

// Проверяет доступность водителя в указанный временной интервал
// Проверяет, не пересекается ли указанный временной интервал с уже назначенными рейсами водителя
bool DriverSchedule::isDriverAvailable(std::shared_ptr<Driver> driver,
                          const Time& startTime,
                          const Time& endTime) const {
    auto it = driverTrips.find(driver);
    if (it == driverTrips.end()) return true;  // У водителя нет рейсов - свободен

    // Проверяем пересечение временных интервалов
    for (const auto& trip : it->second) {
        Time tripStart = trip->getStartTime();
        Time tripEnd = tripStart + 60;  // Предполагаем длительность рейса 60 минут

        // Интервалы пересекаются, если не выполняется условие непересечения
        if (!(endTime < tripStart || startTime > tripEnd)) {
            return false;  // Водитель занят
        }
    }
    return true;  // Водитель свободен
}

// Проверяет соблюдение лимита рабочих часов водителя (12 часов)
bool DriverSchedule::checkWorkingHoursCompliance(std::shared_ptr<Driver> driver) const {
    auto it = driverTrips.find(driver);
    if (it == driverTrips.end()) return true;  // У водителя нет рейсов

    // Считаем общее время работы (каждый рейс = 60 минут)
    int totalMinutes = 0;
    for (const auto& trip : it->second) {
        totalMinutes += 60;
    }

    return totalMinutes <= MAX_WORKING_HOURS;
}

// Получает список рейсов водителя
List<std::shared_ptr<Trip>> DriverSchedule::getDriverTrips(
    std::shared_ptr<Driver> driver) const {
    auto it = driverTrips.find(driver);
    if (it != driverTrips.end()) {
        return it->second;
    }
    return List<std::shared_ptr<Trip>>();  // Пустой список, если водитель не найден
}

// Получает общее время работы водителя в минутах (количество рейсов * 60)
int DriverSchedule::getTotalWorkingMinutes(std::shared_ptr<Driver> driver) const {
    auto trips = getDriverTrips(driver);
    return static_cast<int>(trips.size()) * 60;
}