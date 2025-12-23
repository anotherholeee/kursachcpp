#include "transport_system.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Конструктор транспортной системы
 * 
 * Инициализирует систему транспорта:
 * - Создает планировщик поездок
 * - Создает менеджер данных
 * - Инициализирует алгоритмы расчета времени и поиска маршрутов
 * - Устанавливает учетные данные администраторов по умолчанию
 */
TransportSystem::TransportSystem() 
    : journeyPlanner(this), 
      dataManager(),
      arrivalTimeAlgorithm(std::make_unique<ArrivalTimeCalculationAlgorithm>(this)),
      routeSearchAlgorithm(std::make_unique<RouteSearchAlgorithm>(this)) {
    // Учетные данные администраторов по умолчанию
    adminCredentials["admin"] = "admin123";
    adminCredentials["manager"] = "manager123";
}

// Проверяет, можно ли отменить последнее действие
bool TransportSystem::canUndo() const {
    return commandHistory.canUndo();
}

// Отменяет последнее выполненное действие
void TransportSystem::undo() {
    if (!canUndo()) {
        throw ContainerException("Нет действий для отмены");
    }
    commandHistory.undo();
    std::cout << "Действие отменено.\n";
}

// Проверяет, можно ли повторить отмененное действие
bool TransportSystem::canRedo() const {
    return commandHistory.canRedo();
}

// Повторяет последнее отмененное действие
void TransportSystem::redo() {
    if (!canRedo()) {
        throw ContainerException("Нет действий для повтора");
    }
    commandHistory.redo();
    std::cout << "Действие повторено.\n";
}

std::string TransportSystem::getLastCommandDescription() const {
    return commandHistory.getLastCommandDescription();
}

std::string TransportSystem::getNextCommandDescription() const {
    return commandHistory.getNextCommandDescription();
}

// Проверяет учетные данные администратора
bool TransportSystem::authenticateAdmin(const std::string& username, const std::string& password) {
    auto it = adminCredentials.find(username);
    return it != adminCredentials.end() && it->second == password;
}

// Добавляет нового администратора в систему
void TransportSystem::addAdmin(const std::string& username, const std::string& password) {
    adminCredentials[username] = password;
}

const std::unordered_map<std::string, std::string>& TransportSystem::getAdminCredentials() const {
    return adminCredentials;
}

void TransportSystem::setAdminCredentials(const std::unordered_map<std::string, std::string>& creds) {
    adminCredentials = creds;
}

void TransportSystem::saveData() {
    dataManager.saveAllData(*this);
}

void TransportSystem::loadData() {
    dataManager.loadAllData(*this);
}

/**
 * @brief Поиск маршрутов между двумя остановками
 * @param stopA Начальная остановка
 * @param stopB Конечная остановка
 * @return Список маршрутов, проходящих через обе остановки
 * 
 * Использует алгоритм поиска маршрутов для нахождения всех маршрутов,
 * которые проходят через обе остановки и stopA идет раньше stopB.
 */
List<std::shared_ptr<Route>> TransportSystem::findRoutes(const std::string& stopA, const std::string& stopB) {
    // Используем алгоритм поиска маршрутов
    return routeSearchAlgorithm->findRoutes(stopA, stopB);
}

/**
 * @brief Получение расписания остановки в указанном временном интервале
 * @param stopId Идентификатор остановки
 * @param startTime Начало временного интервала
 * @param endTime Конец временного интервала
 * 
 * Выводит в консоль все рейсы, которые прибывают на указанную остановку
 * в заданном временном интервале, отсортированные по времени прибытия.
 */
void TransportSystem::getStopTimetable(int stopId, const Time& startTime, const Time& endTime) {
    // Находим название остановки по ID
    auto it = stopIdToName.find(stopId);
    if (it == stopIdToName.end()) {
        throw ContainerException("Остановка с ID " + std::to_string(stopId) + " не найдена");
    }
    const std::string& stopName = it->second;

    // Собираем рейсы, прибывающие на остановку в указанном интервале
    List<std::pair<int, Time>> relevantTrips;

    for (const auto& trip : trips) {
        if (trip->hasStop(stopName)) {
            Time arrivalTime = trip->getArrivalTime(stopName);
            // Проверяем, попадает ли время прибытия в заданный интервал
            if (startTime <= arrivalTime && arrivalTime <= endTime) {
                relevantTrips.push_back({trip->getRoute()->getNumber(), arrivalTime});
            }
        }
    }

    // Сортируем рейсы по времени прибытия
    relevantTrips.sort([](const auto& a, const auto& b) { return a.second < b.second; });

    // Выводим расписание
    std::cout << "\nРасписание для остановки '" << stopName << "' с "
              << startTime << " по " << endTime << ":\n";
    if (relevantTrips.empty()) {
        std::cout << "Рейсов не найдено.\n";
    } else {
        for (const auto& trip : relevantTrips) {
            std::cout << "Маршрут " << trip.first << " - прибытие в " << trip.second << '\n';
        }
    }
}

void TransportSystem::getStopTimetableAll(const std::string& stopName) {
    List<std::pair<int, Time>> relevantTrips;

    for (const auto& trip : trips) {
        if (trip->hasStop(stopName)) {
            Time arrivalTime = trip->getArrivalTime(stopName);
            relevantTrips.push_back({trip->getRoute()->getNumber(), arrivalTime});
        }
    }

    // Сортируем используя метод sort
    relevantTrips.sort([](const auto& a, const auto& b) { return a.second < b.second; });

    std::cout << "\nРасписание для остановки '" << stopName << "':\n";
    if (relevantTrips.empty()) {
        std::cout << "Рейсов не найдено.\n";
    } else {
        for (const auto& trip : relevantTrips) {
            std::cout << "Маршрут " << trip.first << " - прибытие в " << trip.second << '\n';
        }
    }
}

/**
 * @brief Расчет времени прибытия на остановки для рейса
 * @param tripId Идентификатор рейса
 * @param averageSpeed Средняя скорость движения в км/ч
 * 
 * Делегирует расчет времени прибытия алгоритму расчета времени.
 * Время рассчитывается на основе средней скорости и расстояния между остановками.
 */
void TransportSystem::calculateArrivalTimes(int tripId, double averageSpeed) {
    // Используем алгоритм расчета времени прибытия
    arrivalTimeAlgorithm->calculateArrivalTimes(tripId, averageSpeed);
}

ArrivalTimeCalculationAlgorithm* TransportSystem::getArrivalTimeAlgorithm() const {
    return arrivalTimeAlgorithm.get();
}

RouteSearchAlgorithm* TransportSystem::getRouteSearchAlgorithm() const {
    return routeSearchAlgorithm.get();
}

/**
 * @brief Добавление маршрута в систему
 * @param route Указатель на маршрут для добавления
 * 
 * Проверяет уникальность номера маршрута и добавляет его через систему команд
 * (для возможности отмены операции).
 */
void TransportSystem::addRoute(std::shared_ptr<Route> route) {
    // Проверяем, что маршрут с таким номером еще не существует
    for (const auto& existingRoute : routes) {
        if (existingRoute->getNumber() == route->getNumber()) {
            throw ContainerException("Маршрут с номером " + std::to_string(route->getNumber()) + " уже существует");
        }
    }
    // Добавляем через систему команд (для поддержки undo/redo)
    commandHistory.executeCommand(std::make_unique<AddRouteCommand>(this, route));
}

/**
 * @brief Добавление рейса в систему
 * @param trip Указатель на рейс для добавления
 * 
 * Проверяет уникальность ID рейса. Если водитель или транспортное средство
 * рейса еще не добавлены в систему, добавляет их автоматически.
 * Затем добавляет рейс через систему команд.
 */
void TransportSystem::addTrip(std::shared_ptr<Trip> trip) {
    // Проверяем уникальность ID рейса
    for (const auto& existingTrip : trips) {
        if (existingTrip->getTripId() == trip->getTripId()) {
            throw ContainerException("Рейс с ID " + std::to_string(trip->getTripId()) + " уже существует");
        }
    }

    // Проверяем и добавляем водителя, если его еще нет в системе
    bool driverExists = false;
    const auto& tripDriver = trip->getDriver();
    for (const auto& existingDriver : drivers) {
        if (existingDriver->getFirstName() == tripDriver->getFirstName() &&
            existingDriver->getLastName() == tripDriver->getLastName() &&
            existingDriver->getMiddleName() == tripDriver->getMiddleName()) {
            driverExists = true;
            break;
        }
    }
    if (!driverExists) {
        addDriver(trip->getDriver());
    }

    // Проверяем и добавляем транспортное средство, если его еще нет в системе
    bool vehicleExists = false;
    const auto& tripVehicle = trip->getVehicle();
    for (const auto& existingVehicle : vehicles) {
        if (existingVehicle->getLicensePlate() == tripVehicle->getLicensePlate()) {
            vehicleExists = true;
            break;
        }
    }
    if (!vehicleExists) {
        addVehicle(trip->getVehicle());
    }

    // Добавляем рейс через систему команд
    commandHistory.executeCommand(std::make_unique<AddTripCommand>(this, trip));
}

// Добавляет транспортное средство в систему (с проверкой уникальности номера)
void TransportSystem::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    for (const auto& existingVehicle : vehicles) {
        if (existingVehicle->getLicensePlate() == vehicle->getLicensePlate()) {
            throw ContainerException("Транспортное средство с номером " + vehicle->getLicensePlate() + " уже существует");
        }
    }
    commandHistory.executeCommand(std::make_unique<AddVehicleCommand>(this, vehicle));
}

// Добавляет водителя в систему через систему команд
void TransportSystem::addDriver(std::shared_ptr<Driver> driver) {
    commandHistory.executeCommand(std::make_unique<AddDriverCommand>(this, driver));
}

// Добавляет остановку в систему (с проверкой уникальности ID)
void TransportSystem::addStop(const Stop& stop) {
    for (const auto& existingStop : stops) {
        if (existingStop.getId() == stop.getId()) {
            throw ContainerException("Остановка с ID " + std::to_string(stop.getId()) + " уже существует");
        }
    }
    commandHistory.executeCommand(std::make_unique<AddStopCommand>(this, stop));
}

// Удаляет маршрут из системы (с проверкой существования)
void TransportSystem::removeRoute(int routeNumber) {
    auto it = std::find_if(routes.begin(), routes.end(),
                          [routeNumber](const auto& r) { return r->getNumber() == routeNumber; });
    if (it == routes.end()) {
        throw ContainerException("Маршрут с номером " + std::to_string(routeNumber) + " не найден");
    }
    commandHistory.executeCommand(std::make_unique<RemoveRouteCommand>(this, routeNumber));
    std::cout << "Маршрут " << routeNumber << " удален.\n";
}

// Удаляет рейс из системы (с проверкой существования)
void TransportSystem::removeTrip(int tripId) {
    auto it = std::find_if(trips.begin(), trips.end(),
                          [tripId](const auto& t) { return t->getTripId() == tripId; });
    if (it == trips.end()) {
        throw ContainerException("Рейс с ID " + std::to_string(tripId) + " не найден");
    }
    commandHistory.executeCommand(std::make_unique<RemoveTripCommand>(this, tripId));
    std::cout << "Рейс " << tripId << " удален.\n";
}

// Выводит в консоль информацию о всех маршрутах
void TransportSystem::displayAllRoutes() const {
    std::cout << "\n=== ВСЕ МАРШРУТЫ ===\n";
    for (const auto& route : routes) {
        std::cout << "Маршрут " << route->getNumber() << " (" << route->getVehicleType()
                  << "): " << route->getStartStop() << " -> " << route->getEndStop() << "\n";
    }
}

// Выводит в консоль информацию о всех рейсах
void TransportSystem::displayAllTrips() const {
    std::cout << "\n=== ВСЕ РЕЙСЫ ===\n";
    for (const auto& trip : trips) {
        std::cout << "Рейс " << trip->getTripId() << ": Маршрут " << trip->getRoute()->getNumber()
                  << ", ТС: " << trip->getVehicle()->getInfo()
                  << ", Водитель: " << trip->getDriver()->getFullName()
                  << ", Отправление: " << trip->getStartTime() << "\n";
    }
}

// Выводит в консоль информацию о всех транспортных средствах
void TransportSystem::displayAllVehicles() const {
    std::cout << "\n=== ВСЕ ТРАНСПОРТНЫЕ СРЕДСТВА ===\n";
    for (const auto& vehicle : vehicles) {
        std::cout << vehicle->getInfo() << '\n';
    }
}

// Выводит в консоль информацию о всех остановках
void TransportSystem::displayAllStops() const {
    std::cout << "\n=== ВСЕ ОСТАНОВКИ ===\n";
    for (const auto& stop : stops) {
        std::cout << "ID: " << stop.getId() << " - " << stop.getName() << '\n';
    }
}

const List<std::shared_ptr<Trip>>& TransportSystem::getTrips() const {
    return trips;
}

const List<std::shared_ptr<Route>>& TransportSystem::getRoutes() const {
    return routes;
}

const List<std::shared_ptr<Vehicle>>& TransportSystem::getVehicles() const {
    return vehicles;
}

const List<Stop>& TransportSystem::getStops() const {
    return stops;
}

const List<std::shared_ptr<Driver>>& TransportSystem::getDrivers() const {
    return drivers;
}

JourneyPlanner& TransportSystem::getJourneyPlanner() {
    return journeyPlanner;
}

DriverSchedule& TransportSystem::getDriverSchedule() {
    return driverSchedule;
}

// Находит водителя по ФИО
std::shared_ptr<Driver> TransportSystem::findDriverByName(const std::string& firstName,
                                        const std::string& lastName,
                                        const std::string& middleName) const {
    for (const auto& driver : drivers) {
        if (driver->getFirstName() == firstName &&
            driver->getLastName() == lastName &&
            (middleName.empty() || driver->getMiddleName() == middleName)) {
            return driver;
        }
    }
    return nullptr;  // Водитель не найден
}

// Находит транспортное средство по государственному номеру
std::shared_ptr<Vehicle> TransportSystem::findVehicleByLicensePlate(const std::string& licensePlate) const {
    for (const auto& vehicle : vehicles) {
        if (vehicle->getLicensePlate() == licensePlate) {
            return vehicle;
        }
    }
    return nullptr;  // Транспортное средство не найдено
}

// Находит маршрут по номеру
std::shared_ptr<Route> TransportSystem::findRouteByNumber(int number) const {
    for (const auto& route : routes) {
        if (route->getNumber() == number) {
            return route;
        }
    }
    return nullptr;  // Маршрут не найден
}

// Получает список всех рейсов, проходящих через указанную остановку
List<std::shared_ptr<Trip>> TransportSystem::getTripsThroughStop(const std::string& stopName) const {
    List<std::shared_ptr<Trip>> result;
    for (const auto& trip : trips) {
        if (trip->hasStop(stopName)) {
            result.push_back(trip);
        }
    }
    return result;
}

std::string TransportSystem::getStopNameById(int id) const {
    auto it = stopIdToName.find(id);
    if (it != stopIdToName.end()) {
        return it->second;
    }
    return "";
}

std::shared_ptr<Route> TransportSystem::getRouteByNumber(int number) {
    for (const auto& route : routes) {
        if (route->getNumber() == number) {
            return route;
        }
    }
    return nullptr;
}

std::shared_ptr<Trip> TransportSystem::getTripById(int id) {
    for (const auto& trip : trips) {
        if (trip->getTripId() == id) {
            return trip;
        }
    }
    return nullptr;
}

std::shared_ptr<Vehicle> TransportSystem::getVehicleByLicensePlate(const std::string& licensePlate) {
    for (const auto& vehicle : vehicles) {
        if (vehicle->getLicensePlate() == licensePlate) {
            return vehicle;
        }
    }
    return nullptr;
}

Stop TransportSystem::getStopById(int id) {
    for (const auto& stop : stops) {
        if (stop.getId() == id) {
            return stop;
        }
    }
    throw ContainerException("Остановка с ID " + std::to_string(id) + " не найдена");
}

void TransportSystem::addRouteDirect(std::shared_ptr<Route> route) {
    routes.push_back(std::move(route));
}

void TransportSystem::removeRouteDirect(int routeNumber) {
    auto it = std::find_if(routes.begin(), routes.end(),
                          [routeNumber](const auto& r) { return r->getNumber() == routeNumber; });
    if (it != routes.end()) {
        routes.erase(it);
    }
}

void TransportSystem::addTripDirect(std::shared_ptr<Trip> trip) {
    trips.push_back(std::move(trip));
}

void TransportSystem::removeTripDirect(int tripId) {
    auto it = std::find_if(trips.begin(), trips.end(),
                          [tripId](const auto& t) { return t->getTripId() == tripId; });
    if (it != trips.end()) {
        trips.erase(it);
    }
}

void TransportSystem::addVehicleDirect(std::shared_ptr<Vehicle> vehicle) {
    vehicles.push_back(std::move(vehicle));
}

void TransportSystem::removeVehicleDirect(const std::string& licensePlate) {
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
                          [&licensePlate](const auto& v) {
                              return v->getLicensePlate() == licensePlate;
                          });
    if (it != vehicles.end()) {
        vehicles.erase(it);
    }
}

void TransportSystem::addStopDirect(const Stop& stop) {
    stops.push_back(stop);
    stopIdToName[stop.getId()] = stop.getName();
}

void TransportSystem::removeStopDirect(int stopId) {
    auto it = std::find_if(stops.begin(), stops.end(),
                          [stopId](const auto& s) { return s.getId() == stopId; });
    if (it != stops.end()) {
        stopIdToName.erase(stopId);
        stops.erase(it);
    }
}

void TransportSystem::addDriverDirect(std::shared_ptr<Driver> driver) {
    drivers.push_back(std::move(driver));
}

void TransportSystem::removeDriverDirect(std::shared_ptr<Driver> driver) {
    auto it = std::find_if(drivers.begin(), drivers.end(),
                          [&driver](const auto& d) {
                              return d->getFirstName() == driver->getFirstName() &&
                                     d->getLastName() == driver->getLastName() &&
                                     d->getMiddleName() == driver->getMiddleName();
                          });
    if (it != drivers.end()) {
        drivers.erase(it);
    }
}

