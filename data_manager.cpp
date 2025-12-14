#include "data_manager.h"
#include "transport_system.h"
#include "stop.h"
#include "vehicle.h"
#include "bus.h"
#include "tram.h"
#include "trolleybus.h"
#include "driver.h"
#include "route.h"
#include "trip.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

DataManager::DataManager(const std::string& dir) : dataDirectory(dir) {
    std::filesystem::create_directories(dataDirectory);
}

void DataManager::saveAllData(TransportSystem& system) {
    try {
        std::cout << "Сохранение данных в файлы...\n";

        saveStops(system);
        saveVehicles(system);
        saveDrivers(system);
        saveRoutes(system);
        saveTrips(system);
        saveAdminCredentials(system);

        std::cout << "Данные успешно сохранены!\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка при сохранении данных: " << e.what() << "\n";
    }
}

void DataManager::loadAllData(TransportSystem& system) {
    try {
        loadStops(system);
        loadVehicles(system);
        loadDrivers(system);
        loadRoutes(system);
        loadTrips(system);
        loadAdminCredentials(system);
    } catch (const std::exception& e) {
        // Тихо игнорируем ошибки загрузки
    }
}

void DataManager::saveStops(TransportSystem& system) {
    std::ofstream file(dataDirectory + "stops.txt");
    if (!file.is_open()) throw FileException("stops.txt", "открытие для записи");

    const auto& stops = system.getStops();
    for (const auto& stop : stops) {
        file << stop.serialize() << "\n";
    }
    file.close();
}

void DataManager::saveVehicles(TransportSystem& system) {
    std::ofstream file(dataDirectory + "vehicles.txt");
    if (!file.is_open()) throw FileException("vehicles.txt", "открытие для записи");

    const auto& vehicles = system.getVehicles();
    for (const auto& vehicle : vehicles) {
        file << vehicle->serialize() << "\n";
    }
    file.close();
}

void DataManager::saveDrivers(TransportSystem& system) {
    std::ofstream file(dataDirectory + "drivers.txt");
    if (!file.is_open()) throw FileException("drivers.txt", "открытие для записи");

    const auto& drivers = system.getDrivers();
    for (const auto& driver : drivers) {
        file << driver->serialize() << "\n";
    }
    file.close();
}

void DataManager::saveRoutes(TransportSystem& system) {
    std::ofstream file(dataDirectory + "routes.txt");
    if (!file.is_open()) throw FileException("routes.txt", "открытие для записи");

    const auto& routes = system.getRoutes();
    for (const auto& route : routes) {
        file << route->serialize() << "\n";
    }
    file.close();
}

void DataManager::saveTrips(TransportSystem& system) {
    std::ofstream file(dataDirectory + "trips.txt");
    if (!file.is_open()) throw FileException("trips.txt", "открытие для записи");

    const auto& trips = system.getTrips();
    for (const auto& trip : trips) {
        file << trip->serialize() << "\n";
    }
    file.close();
}

void DataManager::saveAdminCredentials(TransportSystem& system) {
    std::ofstream file(dataDirectory + "admins.bin", std::ios::binary);
    if (!file.is_open()) throw FileException("admins.bin", "открытие для записи");

    const auto& creds = system.getAdminCredentials();
    size_t count = creds.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& [username, password] : creds) {
        size_t usernameLen = username.size();
        size_t passwordLen = password.size();
        file.write(reinterpret_cast<const char*>(&usernameLen), sizeof(usernameLen));
        file.write(username.c_str(), usernameLen);
        file.write(reinterpret_cast<const char*>(&passwordLen), sizeof(passwordLen));
        file.write(password.c_str(), passwordLen);
    }
    file.close();
}

void DataManager::loadStops(TransportSystem& system) {
    std::ifstream file(dataDirectory + "stops.txt");
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty()) {
            try {
                system.addStop(Stop::deserialize(line));
            } catch (const std::exception& e) {
                throw FileException("stops.txt", "чтение строки " + std::to_string(lineNumber) + ": " + e.what());
            }
        }
    }
    if (file.bad() && !file.eof()) {
        throw FileException("stops.txt", "ошибка чтения файла");
    }
    file.close();
}

void DataManager::loadVehicles(TransportSystem& system) {
    std::ifstream file(dataDirectory + "vehicles.txt");
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty()) {
            try {
                std::istringstream ss(line);
                std::string type, model, licensePlate;
                std::getline(ss, type, '|');
                std::getline(ss, model, '|');
                std::getline(ss, licensePlate);

                std::shared_ptr<Vehicle> vehicle;
                if (type == "Автобус") {
                    vehicle = std::make_shared<Bus>(model, licensePlate);
                } else if (type == "Трамвай") {
                    vehicle = std::make_shared<Tram>(model, licensePlate);
                } else if (type == "Троллейбус") {
                    vehicle = std::make_shared<Trolleybus>(model, licensePlate);
                } else {
                    throw InputException("Неизвестный тип транспорта: " + type);
                }

                system.addVehicle(vehicle);
            } catch (const std::exception& e) {
                throw FileException("vehicles.txt", "чтение строки " + std::to_string(lineNumber) + ": " + e.what());
            }
        }
    }
    if (file.bad() && !file.eof()) {
        throw FileException("vehicles.txt", "ошибка чтения файла");
    }
    file.close();
}

void DataManager::loadDrivers(TransportSystem& system) {
    std::ifstream file(dataDirectory + "drivers.txt");
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty()) {
            try {
                system.addDriver(Driver::deserialize(line));
            } catch (const std::exception& e) {
                throw FileException("drivers.txt", "чтение строки " + std::to_string(lineNumber) + ": " + e.what());
            }
        }
    }
    if (file.bad() && !file.eof()) {
        throw FileException("drivers.txt", "ошибка чтения файла");
    }
    file.close();
}

void DataManager::loadRoutes(TransportSystem& system) {
    std::ifstream file(dataDirectory + "routes.txt");
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty()) {
            try {
                system.addRoute(Route::deserialize(line));
            } catch (const std::exception& e) {
                throw FileException("routes.txt", "чтение строки " + std::to_string(lineNumber) + ": " + e.what());
            }
        }
    }
    if (file.bad() && !file.eof()) {
        throw FileException("routes.txt", "ошибка чтения файла");
    }
    file.close();
}

void DataManager::loadTrips(TransportSystem& system) {
    std::ifstream file(dataDirectory + "trips.txt");
    if (!file.is_open()) return;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (!line.empty()) {
            try {
                system.addTrip(Trip::deserialize(line, &system));
            } catch (const std::exception& e) {
                throw FileException("trips.txt", "чтение строки " + std::to_string(lineNumber) + ": " + e.what());
            }
        }
    }
    if (file.bad() && !file.eof()) {
        throw FileException("trips.txt", "ошибка чтения файла");
    }
    file.close();
}

void DataManager::loadAdminCredentials(TransportSystem& system) {
    std::ifstream file(dataDirectory + "admins.bin", std::ios::binary);
    if (!file.is_open()) return;

    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::unordered_map<std::string, std::string> creds;
    for (size_t i = 0; i < count; ++i) {
        size_t usernameLen, passwordLen;
        file.read(reinterpret_cast<char*>(&usernameLen), sizeof(usernameLen));
        std::string username(usernameLen, '\0');
        file.read(&username[0], usernameLen);

        file.read(reinterpret_cast<char*>(&passwordLen), sizeof(passwordLen));
        std::string password(passwordLen, '\0');
        file.read(&password[0], passwordLen);

        creds[username] = password;
    }
    file.close();

    system.setAdminCredentials(creds);
}

