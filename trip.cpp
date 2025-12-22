#include "trip.h"
#include "transport_system.h"
#include "bus.h"
#include "tram.h"
#include "trolleybus.h"
#include <sstream>

Trip::Trip(int id, std::shared_ptr<Route> r, std::shared_ptr<Vehicle> v,
         std::shared_ptr<Driver> d, const Time& start, int day)
    : tripId(id), route(std::move(r)), vehicle(std::move(v)),
      driver(std::move(d)), startTime(start), weekDay(day) {
    if (day < 1 || day > 7) {
        throw InputException("День недели должен быть от 1 до 7");
    }
}

void Trip::setArrivalTime(const std::string& stop, const Time& time) {
    schedule[stop] = time;
}

Time Trip::getArrivalTime(const std::string& stop) const {
    auto it = schedule.find(stop);
    if (it != schedule.end()) {
        return it->second;
    }
    throw ContainerException("Остановка не найдена в расписании рейса");
}

bool Trip::hasStop(const std::string& stop) const {
    return schedule.find(stop) != schedule.end();
}

int Trip::getTripId() const {
    return tripId;
}

std::shared_ptr<Route> Trip::getRoute() const {
    return route;
}

std::shared_ptr<Vehicle> Trip::getVehicle() const {
    return vehicle;
}

std::shared_ptr<Driver> Trip::getDriver() const {
    return driver;
}

Time Trip::getStartTime() const {
    return startTime;
}

const std::map<std::string, Time>& Trip::getSchedule() const {
    return schedule;
}

int Trip::getWeekDay() const {
    return weekDay;
}

Time Trip::getEstimatedEndTime() const {
    return startTime + 60;
}

std::string Trip::serialize() const {
    // Сохраняем только номер маршрута, а не полную информацию о маршруте
    // Полная информация о маршруте уже сохранена в routes.txt
    std::string result = std::to_string(tripId) + "|" + std::to_string(route->getNumber()) + "|" +
                       vehicle->serialize() + "|" + driver->serialize() + "|" +
                       startTime.serialize() + "|" + std::to_string(weekDay) + "|";

    std::string scheduleStr;
    for (const auto& [stop, time] : schedule) {
        scheduleStr += stop + "=" + time.serialize() + ";";
    }
    if (!scheduleStr.empty()) scheduleStr.pop_back();
    result += scheduleStr;

    return result;
}

std::shared_ptr<Trip> Trip::deserialize(const std::string& data, TransportSystem* system) {
    // Парсим строку
    // Новый формат: tripId|route_number|vehicle(5)|driver(4)|time|weekDay|schedule = 13 токенов
    // Старый формат: tripId|route(4)|vehicle(5)|driver(4)|time|weekDay|schedule = 17 токенов

    std::istringstream ss(data);
    std::string token;
    List<std::string> tokens;

    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 6) {
        throw InputException("Некорректные данные рейса");
    }

    int tripId = std::stoi(tokens[0]);

    std::shared_ptr<Route> route;
    int vehicleTokenIndex, driverTokenIndex, timeTokenIndex, scheduleTokenIndex;

    // Определяем формат: новый (только номер маршрута) или старый (полная сериализация маршрута)
    // Новый формат: tripId|route_number|vehicle(5)|driver(4)|time|weekDay|schedule = 13 токенов (или 11 если vehicle без capacity)
    // Старый формат: tripId|route(4)|vehicle(5)|driver(4)|time|weekDay|schedule = 17 токенов

    // Проверяем: если tokens[2] - это тип транспорта (Автобус, Трамвай, Троллейбус), то это старый формат
    // В новом формате tokens[1] - это номер маршрута (число), tokens[2] - это тип транспорта vehicle
    bool isNewFormat = false;
    if (tokens.size() >= 3) {
        // В старом формате: tokens[1] = route_number, tokens[2] = route_vehicleType
        // В новом формате: tokens[1] = route_number, tokens[2] = vehicle_type
        // Проверяем tokens[3]: если это список остановок (содержит ';'), то это старый формат
        if (tokens.size() > 3 && tokens[3].find(';') != std::string::npos) {
            // Это старый формат - tokens[3] содержит остановки маршрута
            isNewFormat = false;
        } else {
            // Это новый формат - tokens[2] это тип транспорта vehicle, а не маршрута
            isNewFormat = true;
        }
    } else {
        // Мало токенов - вероятно новый формат
        isNewFormat = true;
    }

    if (isNewFormat) {
        // Новый формат: только номер маршрута
        if (!system) {
            throw InputException("Для загрузки рейса требуется система (для поиска маршрута по номеру)");
        }

        int routeNumber = std::stoi(tokens[1]);
        route = system->getRouteByNumber(routeNumber);
        if (!route) {
            throw ContainerException("Маршрут с номером " + std::to_string(routeNumber) + " не найден");
        }

        vehicleTokenIndex = 2;

        // Определяем количество токенов vehicle
        // Для Bus, Tram, Trolleybus всегда 5 токенов (type|model|licensePlate|capacity|fuelType/voltage)
        // Для базового Vehicle - 3 токена (type|model|licensePlate)
        // Проверяем тип vehicle (tokens[2]) - если это "Автобус", "Трамвай" или "Троллейбус", то vehicle имеет 5 токенов
        std::string vehicleType = tokens[2];
        bool vehicleHasCapacity = (vehicleType == "Автобус" || vehicleType == "Трамвай" || vehicleType == "Троллейбус");
        int vehicleTokenCount = vehicleHasCapacity ? 5 : 3;

        driverTokenIndex = 2 + vehicleTokenCount;  // driver после vehicle
        timeTokenIndex = driverTokenIndex + 4;      // time после driver (4 токена)
        scheduleTokenIndex = timeTokenIndex + 2;    // schedule после time и weekDay
    } else {
        // Старый формат: полная сериализация маршрута
        // Проверяем наличие weekDays в route (токен с запятыми)
        bool hasRouteWeekDays = (tokens.size() > 4 && tokens[4].find(',') != std::string::npos);

    int routeTokenCount = hasRouteWeekDays ? 4 : 3;
    int routeEndIndex = 1 + routeTokenCount;  // Индекс после route

    // Проверяем, сколько токенов у vehicle (5 для FuelTransport/ElectricTransport или 3 для базового Vehicle)
    // Проверяем токен после vehicle licensePlate (токен routeEndIndex + 2) - если это число, то это capacity
    int vehicleLicensePlateIndex = routeEndIndex + 2;  // vehicle: type(routeEndIndex), model(routeEndIndex+1), licensePlate(routeEndIndex+2)
    bool vehicleHasCapacity = (tokens.size() > vehicleLicensePlateIndex + 1 &&
                               !tokens[vehicleLicensePlateIndex + 1].empty() &&
                               (std::isdigit(static_cast<unsigned char>(tokens[vehicleLicensePlateIndex + 1][0])) ||
                                (tokens[vehicleLicensePlateIndex + 1][0] == '-' &&
                                 tokens[vehicleLicensePlateIndex + 1].length() > 1 &&
                                 std::isdigit(static_cast<unsigned char>(tokens[vehicleLicensePlateIndex + 1][1])))));

    int vehicleTokenCount = vehicleHasCapacity ? 5 : 3;

    if (hasRouteWeekDays) {
        // Route с weekDays (4 токена)
        std::string routeData = tokens[1] + "|" + tokens[2] + "|" + tokens[3] + "|" + tokens[4];
        route = Route::deserialize(routeData);
        vehicleTokenIndex = routeEndIndex;  // vehicle начинается сразу после route (индекс 5)
        driverTokenIndex = routeEndIndex + vehicleTokenCount;  // driver после vehicle (индекс 10)
        timeTokenIndex = driverTokenIndex + 4;  // time после driver (4 токена) (индекс 14)
        scheduleTokenIndex = timeTokenIndex + 2;  // schedule после time и weekDay (индекс 16)
    } else if (tokens.size() > 3 && tokens[3].find(';') != std::string::npos) {
        // Route без weekDays (3 токена)
        std::string routeData = tokens[1] + "|" + tokens[2] + "|" + tokens[3];
        route = Route::deserialize(routeData);
        vehicleTokenIndex = routeEndIndex;  // vehicle начинается сразу после route (индекс 4)
        driverTokenIndex = routeEndIndex + vehicleTokenCount;  // driver после vehicle
        timeTokenIndex = driverTokenIndex + 4;  // time после driver (4 токена)
        scheduleTokenIndex = timeTokenIndex + 2;  // schedule после time и weekDay
    } else {
        // Старый формат - route как один токен
        route = Route::deserialize(tokens[1]);
        vehicleTokenIndex = 2;
        driverTokenIndex = 3;
        timeTokenIndex = 4;
        scheduleTokenIndex = 5;
    }
    }

    std::shared_ptr<Vehicle> vehicle = nullptr;
    if (system) {
        std::string type, model, licensePlate;

        if (tokens.size() >= 11) {
            // Новый формат - vehicle разбит на отдельные токены
            // Vehicle может быть: type|model|licensePlate (3 токена) или type|model|licensePlate|capacity|fuelType/voltage (5 токенов)
            if (vehicleTokenIndex + 2 < tokens.size()) {
                type = tokens[vehicleTokenIndex];
                model = tokens[vehicleTokenIndex + 1];
                licensePlate = tokens[vehicleTokenIndex + 2];
                // capacity и fuelType/voltage игнорируем при поиске vehicle по licensePlate
            } else {
                throw InputException("Недостаточно токенов для vehicle");
            }
        } else {
            // Старый формат - vehicle в одном токене
            std::istringstream vehicleStream(tokens[vehicleTokenIndex]);
            std::getline(vehicleStream, type, '|');
            std::getline(vehicleStream, model, '|');
            std::getline(vehicleStream, licensePlate);
        }

        vehicle = system->findVehicleByLicensePlate(licensePlate);

        if (!vehicle && tokens.size() >= 11) {
            std::shared_ptr<Vehicle> newVehicle;
            if (type == "Автобус") {
                // Пытаемся получить capacity и fuelType, если они есть
                int capacity = 50;
                std::string fuelType = "дизель";
                if (vehicleTokenIndex + 4 < tokens.size()) {
                    try {
                        capacity = std::stoi(tokens[vehicleTokenIndex + 3]);
                        fuelType = tokens[vehicleTokenIndex + 4];
                    } catch (...) {
                        // Используем значения по умолчанию
                    }
                }
                newVehicle = std::make_shared<Bus>(model, licensePlate, capacity, fuelType);
            } else if (type == "Трамвай") {
                int capacity = 50;
                double voltage = 600.0;
                if (vehicleTokenIndex + 4 < tokens.size()) {
                    try {
                        capacity = std::stoi(tokens[vehicleTokenIndex + 3]);
                        voltage = std::stod(tokens[vehicleTokenIndex + 4]);
                    } catch (...) {
                        // Используем значения по умолчанию
                    }
                }
                newVehicle = std::make_shared<Tram>(model, licensePlate, capacity, voltage);
            } else if (type == "Троллейбус") {
                int capacity = 50;
                double voltage = 600.0;
                if (vehicleTokenIndex + 4 < tokens.size()) {
                    try {
                        capacity = std::stoi(tokens[vehicleTokenIndex + 3]);
                        voltage = std::stod(tokens[vehicleTokenIndex + 4]);
                    } catch (...) {
                        // Используем значения по умолчанию
                    }
                }
                newVehicle = std::make_shared<Trolleybus>(model, licensePlate, capacity, voltage);
            }
            if (newVehicle) {
                try {
                    system->addVehicle(newVehicle);
                    vehicle = newVehicle;
                } catch (...) {
                    vehicle = system->findVehicleByLicensePlate(licensePlate);
                }
            }
        }
    }

    std::shared_ptr<Driver> driver;
    if (tokens.size() >= 11) {
        // Новый формат - driver разбит на отдельные токены
        if (driverTokenIndex + 3 < tokens.size()) {
            std::string firstName = tokens[driverTokenIndex];
            std::string lastName = tokens[driverTokenIndex + 1];
            std::string middleName = tokens[driverTokenIndex + 2];
            std::string category = tokens[driverTokenIndex + 3];

            driver = system ? system->findDriverByName(firstName, lastName, middleName) : nullptr;

            if (!driver) {
                driver = std::make_shared<Driver>(firstName, lastName, middleName, category);
                if (system) {
                    try {
                        system->addDriver(driver);
                    } catch (...) {
                        driver = system->findDriverByName(firstName, lastName, middleName);
                    }
                }
            }
        } else {
            throw InputException("Недостаточно токенов для driver");
        }
    } else {
        // Старый формат - driver в одном токене
        driver = Driver::deserialize(tokens[driverTokenIndex]);
        if (system) {
            auto existingDriver = system->findDriverByName(
                driver->getFirstName(),
                driver->getLastName(),
                driver->getMiddleName());
            if (existingDriver) {
                driver = existingDriver;
            } else {
                try {
                    system->addDriver(driver);
                } catch (...) {
                }
            }
        }
    }

    if (timeTokenIndex >= tokens.size()) {
        throw InputException("Недостаточно токенов для времени отправления");
    }
    Time startTime = Time::deserialize(tokens[timeTokenIndex]);

    int weekDay = 1;
    // weekDay находится сразу после startTime
    if (timeTokenIndex + 1 < tokens.size()) {
        try {
            weekDay = std::stoi(tokens[timeTokenIndex + 1]);
            if (weekDay < 1 || weekDay > 7) weekDay = 1;
            // schedule начинается после weekDay
            if (scheduleTokenIndex <= timeTokenIndex + 1) {
                scheduleTokenIndex = timeTokenIndex + 2;
            }
        } catch (...) {
            // Если не удалось распарсить weekDay, используем значение по умолчанию
            if (scheduleTokenIndex <= timeTokenIndex + 1) {
                scheduleTokenIndex = timeTokenIndex + 1;
            }
        }
    } else {
        // weekDay отсутствует, schedule начинается сразу после time
        if (scheduleTokenIndex <= timeTokenIndex) {
            scheduleTokenIndex = timeTokenIndex + 1;
        }
    }

    if (!vehicle) {
        throw ContainerException("Транспортное средство не найдено в системе");
    }

    auto trip = std::make_shared<Trip>(tripId, route, vehicle, driver, startTime, weekDay);

    if (scheduleTokenIndex < tokens.size() && !tokens[scheduleTokenIndex].empty()) {
        std::istringstream scheduleStream(tokens[scheduleTokenIndex]);
        std::string stopTimePair;
        while (std::getline(scheduleStream, stopTimePair, ';')) {
            size_t eqPos = stopTimePair.find('=');
            if (eqPos != std::string::npos) {
                std::string stop = stopTimePair.substr(0, eqPos);
                std::string timeStr = stopTimePair.substr(eqPos + 1);
                trip->setArrivalTime(stop, Time::deserialize(timeStr));
            }
        }
    }

    return trip;
}