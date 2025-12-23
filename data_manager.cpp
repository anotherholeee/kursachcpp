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
#include <algorithm>
#include <cctype>

DataManager::DataManager(const std::string& dir) {
    // Нормализуем путь к папке data
    std::filesystem::path dataPath(dir);

    // Если путь относительный, делаем его абсолютным относительно текущей рабочей директории
    if (dataPath.is_relative()) {
        dataPath = std::filesystem::current_path() / dataPath;
    }

    // Нормализуем путь (убираем лишние слеши и т.д.)
    try {
        if (std::filesystem::exists(dataPath)) {
            dataPath = std::filesystem::canonical(dataPath);
        } else {
            // Если папка не существует, создаем её
            std::filesystem::create_directories(dataPath);
            // После создания нормализуем путь
            dataPath = std::filesystem::absolute(dataPath);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // Если не удалось нормализовать, используем абсолютный путь
        dataPath = std::filesystem::absolute(dataPath);
        // Создаем папку, если её нет
        std::filesystem::create_directories(dataPath);
    }

    // Сохраняем путь как строку с завершающим слешем
    dataDirectory = dataPath.string();
    // Нормализуем разделители для Windows
    std::replace(dataDirectory.begin(), dataDirectory.end(), '\\', '/');
    if (dataDirectory.back() != '/') {
        dataDirectory += "/";
    }

    std::cout << "[DEBUG] Папка данных: " << dataDirectory << std::endl;
    std::cout << "[DEBUG] Текущая рабочая директория: " << std::filesystem::current_path().string() << std::endl;
}

void DataManager::saveAllData(TransportSystem& system) {
    std::cout << "Сохранение данных в файлы...\n";

    bool hasErrors = false;

    try {
        saveStops(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении остановок: " << e.what() << "\n";
        hasErrors = true;
    }

    try {
        saveVehicles(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении транспорта: " << e.what() << "\n";
        hasErrors = true;
    }

    try {
        saveDrivers(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении водителей: " << e.what() << "\n";
        hasErrors = true;
    }

    try {
        saveRoutes(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении маршрутов: " << e.what() << "\n";
        hasErrors = true;
    }

    try {
        saveTrips(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении рейсов: " << e.what() << "\n";
        hasErrors = true;
    }

    try {
        saveAdminCredentials(system);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Ошибка при сохранении учетных данных: " << e.what() << "\n";
        hasErrors = true;
    }

    if (hasErrors) {
        std::cout << "Данные сохранены с ошибками!\n";
    } else {
        std::cout << "Данные успешно сохранены!\n";
    }
}

void DataManager::loadAllData(TransportSystem& system) {
    try {
        std::cout << "[DEBUG] Начало загрузки данных из папки: " << dataDirectory << std::endl;

        // Проверяем существование папки и файлов
        std::filesystem::path dataPath(dataDirectory);
        if (!std::filesystem::exists(dataPath)) {
            std::cout << "[DEBUG] ВНИМАНИЕ: Папка данных не существует: " << dataDirectory << std::endl;
            std::cout << "[DEBUG] Создаем папку данных..." << std::endl;
            std::filesystem::create_directories(dataPath);
        }

        // Проверяем существование файлов
        List<std::string> files;
        files.push_back("stops.txt");
        files.push_back("bus.txt");
        files.push_back("trolleybus.txt");
        files.push_back("tram.txt");
        files.push_back("drivers.txt");
        files.push_back("routes.txt");
        files.push_back("busTrips.txt");
        files.push_back("trolleybusTrips.txt");
        files.push_back("tramTrips.txt");
        for (const auto& fileName : files) {
            std::filesystem::path filePath = dataPath / fileName;
            if (std::filesystem::exists(filePath)) {
                auto fileSize = std::filesystem::file_size(filePath);
                std::cout << "[DEBUG] Файл " << fileName << " существует, размер: " << fileSize << " байт" << std::endl;
            } else {
                std::cout << "[DEBUG] ВНИМАНИЕ: Файл " << fileName << " не существует: " << filePath.string() << std::endl;
            }
        }

        loadStops(system);
        std::cout << "[DEBUG] Остановки загружены: " << system.getStops().size() << std::endl;
        loadVehicles(system);
        std::cout << "[DEBUG] Транспорт загружен: " << system.getVehicles().size() << std::endl;
        loadDrivers(system);
        std::cout << "[DEBUG] Водители загружены: " << system.getDrivers().size() << std::endl;
        loadRoutes(system);
        std::cout << "[DEBUG] Маршруты загружены: " << system.getRoutes().size() << std::endl;
        loadTrips(system);
        std::cout << "[DEBUG] Рейсы загружены: " << system.getTrips().size() << std::endl;
        loadAdminCredentials(system);
        std::cout << "[DEBUG] Загрузка данных завершена" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[DEBUG] Ошибка при загрузке данных: " << e.what() << std::endl;
        // Не игнорируем ошибки, выводим их, но не прерываем выполнение программы
        // Это позволяет программе продолжить работу даже если некоторые файлы повреждены
    }
}

void DataManager::saveStops(TransportSystem& system) {
    std::string filePath = dataDirectory + "stops.txt";
    std::cout << "[DEBUG] Сохранение stops.txt в: " << filePath << std::endl;
    std::ofstream file(filePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл для записи. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        throw FileException("stops.txt", "открытие для записи");
    }

    const auto& stops = system.getStops();
    int savedCount = 0;
    for (const auto& stop : stops) {
        file << stop.serialize() << "\n";
        savedCount++;
    }
    file.close();
    std::cout << "[DEBUG] Сохранено остановок: " << savedCount << " в файл " << filePath << std::endl;
}

void DataManager::saveVehicles(TransportSystem& system) {
    const auto& vehicles = system.getVehicles();
    
    // Открываем три файла для разных типов транспорта
    std::ofstream busFile(dataDirectory + "bus.txt", std::ios::trunc);
    std::ofstream trolleybusFile(dataDirectory + "trolleybus.txt", std::ios::trunc);
    std::ofstream tramFile(dataDirectory + "tram.txt", std::ios::trunc);
    
    if (!busFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл bus.txt для записи" << std::endl;
        throw FileException("bus.txt", "открытие для записи");
    }
    if (!trolleybusFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл trolleybus.txt для записи" << std::endl;
        throw FileException("trolleybus.txt", "открытие для записи");
    }
    if (!tramFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл tram.txt для записи" << std::endl;
        throw FileException("tram.txt", "открытие для записи");
    }

    int busCount = 0, trolleybusCount = 0, tramCount = 0;
    
    for (const auto& vehicle : vehicles) {
        std::string vehicleType = vehicle->getType();
        if (vehicleType == "Автобус") {
            busFile << vehicle->serialize() << "\n";
            busCount++;
        } else if (vehicleType == "Троллейбус") {
            trolleybusFile << vehicle->serialize() << "\n";
            trolleybusCount++;
        } else if (vehicleType == "Трамвай") {
            tramFile << vehicle->serialize() << "\n";
            tramCount++;
        }
    }
    
    busFile.close();
    trolleybusFile.close();
    tramFile.close();
    
    std::cout << "[DEBUG] Сохранено автобусов: " << busCount << " в файл " << dataDirectory << "bus.txt" << std::endl;
    std::cout << "[DEBUG] Сохранено троллейбусов: " << trolleybusCount << " в файл " << dataDirectory << "trolleybus.txt" << std::endl;
    std::cout << "[DEBUG] Сохранено трамваев: " << tramCount << " в файл " << dataDirectory << "tram.txt" << std::endl;
}

void DataManager::saveDrivers(TransportSystem& system) {
    std::string filePath = dataDirectory + "drivers.txt";
    std::cout << "[DEBUG] Сохранение drivers.txt в: " << filePath << std::endl;
    std::ofstream file(filePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл для записи. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        throw FileException("drivers.txt", "открытие для записи");
    }

    const auto& drivers = system.getDrivers();
    int savedCount = 0;
    for (const auto& driver : drivers) {
        file << driver->serialize() << "\n";
        savedCount++;
    }
    file.close();
    std::cout << "[DEBUG] Сохранено водителей: " << savedCount << " в файл " << filePath << std::endl;
}

void DataManager::saveRoutes(TransportSystem& system) {
    std::string filePath = dataDirectory + "routes.txt";
    std::cout << "[DEBUG] Сохранение routes.txt в: " << filePath << std::endl;
    std::ofstream file(filePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл для записи. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        throw FileException("routes.txt", "открытие для записи");
    }

    const auto& routes = system.getRoutes();
    const auto& trips = system.getTrips();
    int savedCount = 0;

    for (const auto& route : routes) {
        // Собираем реальные дни недели из рейсов этого маршрута
        std::set<int> actualWeekDays;
        for (const auto& trip : trips) {
            if (trip->getRoute() == route) {
                actualWeekDays.insert(trip->getWeekDay());
            }
        }

        // Если для маршрута нет рейсов, используем дни недели из самого маршрута
        if (actualWeekDays.empty()) {
            actualWeekDays = route->getWeekDays();
        }

        // Формируем строку сериализации с реальными днями недели
        std::string result = std::to_string(route->getNumber()) + "|" + route->getVehicleType() + "|";

        const auto& allStops = route->getAllStops();
        for (size_t i = 0; i < allStops.size(); ++i) {
            result += allStops[i];
            if (i < allStops.size() - 1) result += ";";
        }
        result += "|";

        // Сохраняем реальные дни недели
        for (auto it = actualWeekDays.begin(); it != actualWeekDays.end(); ++it) {
            result += std::to_string(*it);
            if (std::next(it) != actualWeekDays.end()) result += ",";
        }

        file << result << "\n";
        savedCount++;
    }
    file.close();
    std::cout << "[DEBUG] Сохранено маршрутов: " << savedCount << " в файл " << filePath << std::endl;
}

void DataManager::saveTrips(TransportSystem& system) {
    const auto& trips = system.getTrips();
    
    // Открываем три файла для разных типов транспорта
    std::ofstream busTripsFile(dataDirectory + "busTrips.txt", std::ios::trunc);
    std::ofstream trolleybusTripsFile(dataDirectory + "trolleybusTrips.txt", std::ios::trunc);
    std::ofstream tramTripsFile(dataDirectory + "tramTrips.txt", std::ios::trunc);
    
    if (!busTripsFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл busTrips.txt для записи" << std::endl;
        throw FileException("busTrips.txt", "открытие для записи");
    }
    if (!trolleybusTripsFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл trolleybusTrips.txt для записи" << std::endl;
        throw FileException("trolleybusTrips.txt", "открытие для записи");
    }
    if (!tramTripsFile.is_open()) {
        std::cout << "[DEBUG] Ошибка: не удалось открыть файл tramTrips.txt для записи" << std::endl;
        throw FileException("tramTrips.txt", "открытие для записи");
    }

    int busTripsCount = 0, trolleybusTripsCount = 0, tramTripsCount = 0;
    
    for (const auto& trip : trips) {
        auto route = trip->getRoute();
        if (!route) {
            continue; // Пропускаем рейсы без маршрута
        }
        
        std::string vehicleType = route->getVehicleType();
        if (vehicleType == "Автобус") {
            busTripsFile << trip->serialize() << "\n";
            busTripsCount++;
        } else if (vehicleType == "Троллейбус") {
            trolleybusTripsFile << trip->serialize() << "\n";
            trolleybusTripsCount++;
        } else if (vehicleType == "Трамвай") {
            tramTripsFile << trip->serialize() << "\n";
            tramTripsCount++;
        }
    }
    
    busTripsFile.close();
    trolleybusTripsFile.close();
    tramTripsFile.close();
    
    std::cout << "[DEBUG] Сохранено рейсов автобусов: " << busTripsCount << " в файл " << dataDirectory << "busTrips.txt" << std::endl;
    std::cout << "[DEBUG] Сохранено рейсов троллейбусов: " << trolleybusTripsCount << " в файл " << dataDirectory << "trolleybusTrips.txt" << std::endl;
    std::cout << "[DEBUG] Сохранено рейсов трамваев: " << tramTripsCount << " в файл " << dataDirectory << "tramTrips.txt" << std::endl;
}

void DataManager::saveAdminCredentials(TransportSystem& system) {
    std::ofstream file(dataDirectory + "admins.bin", std::ios::binary | std::ios::trunc);
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
    std::string filePath = dataDirectory + "stops.txt";
    std::cout << "[DEBUG] Попытка загрузить stops.txt из: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Файл stops.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int emptyLines = 0;
    int errorLines = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            emptyLines++;
            continue;
        }

        // Убираем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            emptyLines++;
            continue;
        }

        try {
            Stop stop = Stop::deserialize(line);
            // Проверяем на дубликаты перед добавлением
            bool exists = false;
            const auto& existingStops = system.getStops();
            for (const auto& existingStop : existingStops) {
                if (existingStop.getId() == stop.getId()) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                system.addStopDirect(stop);
                loadedCount++;
            } else {
                std::cout << "[DEBUG] Остановка с ID " << stop.getId() << " уже существует, пропускаем" << std::endl;
            }
        } catch (const std::exception& e) {
            errorLines++;
            std::cout << "[DEBUG] Ошибка при загрузке остановки из строки " << lineNumber << ": " << e.what() << " (строка: " << line.substr(0, 50) << ")" << std::endl;
            // Не бросаем исключение, просто пропускаем неправильную строку
            continue;
        }
    }

    if (file.bad() && !file.eof()) {
        throw FileException("stops.txt", "ошибка чтения файла");
    }
    file.close();

    std::cout << "[DEBUG] Всего загружено остановок: " << loadedCount << std::endl;
    if (emptyLines > 0) {
        std::cout << "[DEBUG] Пропущено пустых строк: " << emptyLines << std::endl;
    }
    if (errorLines > 0) {
        std::cout << "[DEBUG] Ошибок при загрузке: " << errorLines << std::endl;
    }

    if (loadedCount == 0 && lineNumber == 0) {
        std::cout << "[DEBUG] ВНИМАНИЕ: Файл stops.txt пуст или не содержит данных!" << std::endl;
    }
}

void DataManager::loadVehicles(TransportSystem& system) {
    int totalLoaded = 0;
    
    // Загружаем автобусы
    std::string busFilePath = dataDirectory + "bus.txt";
    std::cout << "[DEBUG] Попытка загрузить bus.txt из: " << busFilePath << std::endl;
    std::ifstream busFile(busFilePath);
    if (busFile.is_open()) {
        int loaded = loadVehiclesFromFile(busFile, system, "bus.txt");
        totalLoaded += loaded;
        busFile.close();
    } else {
        std::cout << "[DEBUG] Файл bus.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(busFilePath).string() << std::endl;
    }
    
    // Загружаем троллейбусы
    std::string trolleybusFilePath = dataDirectory + "trolleybus.txt";
    std::cout << "[DEBUG] Попытка загрузить trolleybus.txt из: " << trolleybusFilePath << std::endl;
    std::ifstream trolleybusFile(trolleybusFilePath);
    if (trolleybusFile.is_open()) {
        int loaded = loadVehiclesFromFile(trolleybusFile, system, "trolleybus.txt");
        totalLoaded += loaded;
        trolleybusFile.close();
    } else {
        std::cout << "[DEBUG] Файл trolleybus.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(trolleybusFilePath).string() << std::endl;
    }
    
    // Загружаем трамваи
    std::string tramFilePath = dataDirectory + "tram.txt";
    std::cout << "[DEBUG] Попытка загрузить tram.txt из: " << tramFilePath << std::endl;
    std::ifstream tramFile(tramFilePath);
    if (tramFile.is_open()) {
        int loaded = loadVehiclesFromFile(tramFile, system, "tram.txt");
        totalLoaded += loaded;
        tramFile.close();
    } else {
        std::cout << "[DEBUG] Файл tram.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(tramFilePath).string() << std::endl;
    }
    
    // Для обратной совместимости: если новые файлы не найдены, пытаемся загрузить из старого vehicles.txt
    if (totalLoaded == 0) {
        std::string oldFilePath = dataDirectory + "vehicles.txt";
        std::cout << "[DEBUG] Новые файлы не найдены, пытаемся загрузить из старого vehicles.txt" << std::endl;
        std::ifstream oldFile(oldFilePath);
        if (oldFile.is_open()) {
            int loaded = loadVehiclesFromFile(oldFile, system, "vehicles.txt");
            totalLoaded += loaded;
            oldFile.close();
        }
    }
    
    std::cout << "[DEBUG] Всего загружено транспорта: " << totalLoaded << std::endl;
}

// Вспомогательный метод для загрузки транспорта из файла
int DataManager::loadVehiclesFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName) {
    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int emptyLines = 0;
    int errorLines = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            emptyLines++;
            continue;
        }

        // Убираем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            emptyLines++;
            continue;
        }

        try {
            std::istringstream ss(line);
            std::string type, model, licensePlate;
            std::getline(ss, type, '|');
            std::getline(ss, model, '|');
            std::getline(ss, licensePlate, '|');

            // Убираем пробелы из полей
            type.erase(0, type.find_first_not_of(" \t"));
            type.erase(type.find_last_not_of(" \t") + 1);
            model.erase(0, model.find_first_not_of(" \t"));
            model.erase(model.find_last_not_of(" \t") + 1);
            licensePlate.erase(0, licensePlate.find_first_not_of(" \t"));
            licensePlate.erase(licensePlate.find_last_not_of(" \t") + 1);

            // Проверяем на дубликаты перед добавлением
            bool exists = false;
            const auto& existingVehicles = system.getVehicles();
            for (const auto& existingVehicle : existingVehicles) {
                if (existingVehicle->getLicensePlate() == licensePlate) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                std::shared_ptr<Vehicle> vehicle;
                if (type == "Автобус") {
                    // Читаем capacity и fuelType для автобуса
                    std::string capacityStr, fuelType;
                    std::getline(ss, capacityStr, '|');
                    std::getline(ss, fuelType);
                    int capacity = 50; // значение по умолчанию
                    if (!capacityStr.empty()) {
                        try {
                            capacity = std::stoi(capacityStr);
                        } catch (...) {
                            // Используем значение по умолчанию
                        }
                    }
                    if (fuelType.empty()) {
                        fuelType = "дизель";
                    }
                    // Убираем пробелы
                    fuelType.erase(0, fuelType.find_first_not_of(" \t"));
                    fuelType.erase(fuelType.find_last_not_of(" \t") + 1);
                    vehicle = std::make_shared<Bus>(model, licensePlate, capacity, fuelType);
                } else if (type == "Трамвай") {
                    // Читаем capacity и voltage для трамвая
                    std::string capacityStr, voltageStr;
                    std::getline(ss, capacityStr, '|');
                    std::getline(ss, voltageStr);
                    int capacity = 100; // значение по умолчанию
                    double voltage = 600.0; // значение по умолчанию
                    if (!capacityStr.empty()) {
                        try {
                            capacity = std::stoi(capacityStr);
                        } catch (...) {
                            // Используем значение по умолчанию
                        }
                    }
                    if (!voltageStr.empty()) {
                        try {
                            voltage = std::stod(voltageStr);
                        } catch (...) {
                            // Используем значение по умолчанию
                        }
                    }
                    vehicle = std::make_shared<Tram>(model, licensePlate, capacity, voltage);
                } else if (type == "Троллейбус") {
                    // Читаем capacity и voltage для троллейбуса
                    std::string capacityStr, voltageStr;
                    std::getline(ss, capacityStr, '|');
                    std::getline(ss, voltageStr);
                    int capacity = 50; // значение по умолчанию
                    double voltage = 600.0; // значение по умолчанию
                    if (!capacityStr.empty()) {
                        try {
                            capacity = std::stoi(capacityStr);
                        } catch (...) {
                            // Используем значение по умолчанию
                        }
                    }
                    if (!voltageStr.empty()) {
                        try {
                            voltage = std::stod(voltageStr);
                        } catch (...) {
                            // Используем значение по умолчанию
                        }
                    }
                    vehicle = std::make_shared<Trolleybus>(model, licensePlate, capacity, voltage);
                } else {
                    throw InputException("Неизвестный тип транспорта: " + type);
                }

                system.addVehicleDirect(vehicle);
                loadedCount++;
            } else {
                std::cout << "[DEBUG] Транспорт с номером " << licensePlate << " уже существует, пропускаем" << std::endl;
            }
        } catch (const std::exception& e) {
            errorLines++;
            std::cout << "[DEBUG] Ошибка при загрузке транспорта из файла " << fileName << ", строка " << lineNumber << ": " << e.what() << std::endl;
            std::cout << "[DEBUG] Содержимое строки (первые 100 символов): " << line.substr(0, 100) << std::endl;
            // Не бросаем исключение, просто пропускаем неправильную строку
            continue;
        }
    }

    if (file.bad() && !file.eof()) {
        throw FileException(fileName, "ошибка чтения файла");
    }

    if (emptyLines > 0) {
        std::cout << "[DEBUG] Пропущено пустых строк в " << fileName << ": " << emptyLines << std::endl;
    }
    if (errorLines > 0) {
        std::cout << "[DEBUG] Ошибок при загрузке транспорта из " << fileName << ": " << errorLines << std::endl;
    }

    if (loadedCount == 0 && lineNumber == 0) {
        std::cout << "[DEBUG] ВНИМАНИЕ: Файл " << fileName << " пуст или не содержит данных!" << std::endl;
    }
    
    return loadedCount;
}

void DataManager::loadDrivers(TransportSystem& system) {
    std::string filePath = dataDirectory + "drivers.txt";
    std::cout << "[DEBUG] Попытка загрузить drivers.txt из: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Файл drivers.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int emptyLines = 0;
    int errorLines = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            emptyLines++;
            continue;
        }

        // Убираем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            emptyLines++;
            continue;
        }

        try {
                // Проверяем, что строка не содержит данные транспорта (формат: тип|модель|номер)
                // Водители должны быть в формате: имя|фамилия|отчество
                // Если в строке есть цифры в первой части или формат не соответствует, пропускаем
                std::istringstream testStream(line);
                std::string firstPart, secondPart, thirdPart;
                std::getline(testStream, firstPart, '|');
                std::getline(testStream, secondPart, '|');
                std::getline(testStream, thirdPart);

                // Проверяем, что первая часть не является типом транспорта
                if (firstPart == "Автобус" || firstPart == "Трамвай" || firstPart == "Троллейбус") {
                    std::cout << "[DEBUG] Пропущена строка с данными транспорта в drivers.txt (строка " << lineNumber << "): " << line << std::endl;
                    continue;
                }

                // Проверяем, что первая часть выглядит как имя (не номерной знак транспорта)
                // Номерные знаки обычно содержат пробелы и цифры в определенном формате
                // Имена обычно начинаются с буквы и не содержат только цифры
                if (firstPart.length() > 0 && std::isdigit(static_cast<unsigned char>(firstPart[0]))) {
                    // Начинается с цифры - похоже на номерной знак или ID, пропускаем
                    std::cout << "[DEBUG] Пропущена строка с неправильным форматом в drivers.txt (строка " << lineNumber << "): " << line << std::endl;
                    continue;
                }

                auto driver = Driver::deserialize(line);
                // Проверяем на дубликаты перед добавлением
                bool exists = false;
                const auto& existingDrivers = system.getDrivers();
                for (const auto& existingDriver : existingDrivers) {
                    if (existingDriver->getFirstName() == driver->getFirstName() &&
                        existingDriver->getLastName() == driver->getLastName() &&
                        existingDriver->getMiddleName() == driver->getMiddleName()) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    system.addDriverDirect(driver);
                    loadedCount++;
                } else {
                    std::cout << "[DEBUG] Водитель " << driver->getFullName() << " уже существует, пропускаем" << std::endl;
                }
        } catch (const std::exception& e) {
            errorLines++;
            std::cout << "[DEBUG] Ошибка при загрузке водителя из строки " << lineNumber << ": " << e.what() << std::endl;
            std::cout << "[DEBUG] Содержимое строки (первые 100 символов): " << line.substr(0, 100) << std::endl;
            // Не бросаем исключение, просто пропускаем неправильную строку
            continue;
        }
    }

    if (file.bad() && !file.eof()) {
        throw FileException("drivers.txt", "ошибка чтения файла");
    }
    file.close();

    std::cout << "[DEBUG] Всего загружено водителей: " << loadedCount << std::endl;
    if (emptyLines > 0) {
        std::cout << "[DEBUG] Пропущено пустых строк: " << emptyLines << std::endl;
    }
    if (errorLines > 0) {
        std::cout << "[DEBUG] Ошибок при загрузке водителей: " << errorLines << std::endl;
    }

    if (loadedCount == 0 && lineNumber == 0) {
        std::cout << "[DEBUG] ВНИМАНИЕ: Файл drivers.txt пуст или не содержит данных!" << std::endl;
    }
}

void DataManager::loadRoutes(TransportSystem& system) {
    std::string filePath = dataDirectory + "routes.txt";
    std::cout << "[DEBUG] Попытка загрузить routes.txt из: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[DEBUG] Файл routes.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(filePath).string() << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int emptyLines = 0;
    int errorLines = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            emptyLines++;
            continue;
        }

        // Убираем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            emptyLines++;
            continue;
        }

        try {
            auto route = Route::deserialize(line);
            // Проверяем на дубликаты перед добавлением
            bool exists = false;
            const auto& existingRoutes = system.getRoutes();
            for (const auto& existingRoute : existingRoutes) {
                if (existingRoute->getNumber() == route->getNumber()) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                system.addRouteDirect(route);
                loadedCount++;
            } else {
                std::cout << "[DEBUG] Маршрут с номером " << route->getNumber() << " уже существует, пропускаем" << std::endl;
            }
        } catch (const std::exception& e) {
            errorLines++;
            std::cout << "[DEBUG] Ошибка при загрузке маршрута из строки " << lineNumber << ": " << e.what() << std::endl;
            std::cout << "[DEBUG] Содержимое строки (первые 100 символов): " << line.substr(0, 100) << std::endl;
            // Не бросаем исключение, просто пропускаем неправильную строку
            continue;
        }
    }

    if (file.bad() && !file.eof()) {
        throw FileException("routes.txt", "ошибка чтения файла");
    }
    file.close();

    std::cout << "[DEBUG] Всего загружено маршрутов: " << loadedCount << std::endl;
    if (emptyLines > 0) {
        std::cout << "[DEBUG] Пропущено пустых строк: " << emptyLines << std::endl;
    }
    if (errorLines > 0) {
        std::cout << "[DEBUG] Ошибок при загрузке маршрутов: " << errorLines << std::endl;
    }

    if (loadedCount == 0 && lineNumber == 0) {
        std::cout << "[DEBUG] ВНИМАНИЕ: Файл routes.txt пуст или не содержит данных!" << std::endl;
    }
}

void DataManager::loadTrips(TransportSystem& system) {
    int totalLoaded = 0;
    
    // Загружаем рейсы автобусов
    std::string busTripsFilePath = dataDirectory + "busTrips.txt";
    std::cout << "[DEBUG] Попытка загрузить busTrips.txt из: " << busTripsFilePath << std::endl;
    std::ifstream busTripsFile(busTripsFilePath);
    if (busTripsFile.is_open()) {
        int loaded = loadTripsFromFile(busTripsFile, system, "busTrips.txt");
        totalLoaded += loaded;
        busTripsFile.close();
    } else {
        std::cout << "[DEBUG] Файл busTrips.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(busTripsFilePath).string() << std::endl;
    }
    
    // Загружаем рейсы троллейбусов
    std::string trolleybusTripsFilePath = dataDirectory + "trolleybusTrips.txt";
    std::cout << "[DEBUG] Попытка загрузить trolleybusTrips.txt из: " << trolleybusTripsFilePath << std::endl;
    std::ifstream trolleybusTripsFile(trolleybusTripsFilePath);
    if (trolleybusTripsFile.is_open()) {
        int loaded = loadTripsFromFile(trolleybusTripsFile, system, "trolleybusTrips.txt");
        totalLoaded += loaded;
        trolleybusTripsFile.close();
    } else {
        std::cout << "[DEBUG] Файл trolleybusTrips.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(trolleybusTripsFilePath).string() << std::endl;
    }
    
    // Загружаем рейсы трамваев
    std::string tramTripsFilePath = dataDirectory + "tramTrips.txt";
    std::cout << "[DEBUG] Попытка загрузить tramTrips.txt из: " << tramTripsFilePath << std::endl;
    std::ifstream tramTripsFile(tramTripsFilePath);
    if (tramTripsFile.is_open()) {
        int loaded = loadTripsFromFile(tramTripsFile, system, "tramTrips.txt");
        totalLoaded += loaded;
        tramTripsFile.close();
    } else {
        std::cout << "[DEBUG] Файл tramTrips.txt не найден или не может быть открыт. Полный путь: " << std::filesystem::absolute(tramTripsFilePath).string() << std::endl;
    }
    
    // Для обратной совместимости: если новые файлы не найдены, пытаемся загрузить из старого trips.txt
    if (totalLoaded == 0) {
        std::string oldFilePath = dataDirectory + "trips.txt";
        std::cout << "[DEBUG] Новые файлы не найдены, пытаемся загрузить из старого trips.txt" << std::endl;
        std::ifstream oldFile(oldFilePath);
        if (oldFile.is_open()) {
            int loaded = loadTripsFromFile(oldFile, system, "trips.txt");
            totalLoaded += loaded;
            oldFile.close();
        }
    }
    
    std::cout << "[DEBUG] Всего загружено рейсов: " << totalLoaded << std::endl;
}

// Вспомогательный метод для загрузки рейсов из файла
int DataManager::loadTripsFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName) {
    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int emptyLines = 0;
    int errorLines = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            emptyLines++;
            continue;
        }

        // Убираем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            emptyLines++;
            continue;
        }

        try {
            auto trip = Trip::deserialize(line, &system);
            // Проверяем на дубликаты перед добавлением
            bool exists = false;
            const auto& existingTrips = system.getTrips();
            for (const auto& existingTrip : existingTrips) {
                if (existingTrip->getTripId() == trip->getTripId()) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                system.addTripDirect(trip);
                loadedCount++;
                std::cout << "[DEBUG] Загружен рейс ID: " << trip->getTripId() << " из " << fileName << std::endl;
            } else {
                std::cout << "[DEBUG] Рейс с ID " << trip->getTripId() << " уже существует, пропускаем" << std::endl;
            }
        } catch (const std::exception& e) {
            errorLines++;
            std::cout << "[DEBUG] Ошибка при загрузке рейса из файла " << fileName << ", строка " << lineNumber << ": " << e.what() << std::endl;
            std::cout << "[DEBUG] Содержимое строки (первые 100 символов): " << line.substr(0, 100) << std::endl;
            // Не бросаем исключение, просто пропускаем неправильную строку
            continue;
        }
    }

    if (file.bad() && !file.eof()) {
        throw FileException(fileName, "ошибка чтения файла");
    }

    if (emptyLines > 0) {
        std::cout << "[DEBUG] Пропущено пустых строк в " << fileName << ": " << emptyLines << std::endl;
    }
    if (errorLines > 0) {
        std::cout << "[DEBUG] Ошибок при загрузке рейсов из " << fileName << ": " << errorLines << std::endl;
    }

    if (loadedCount == 0 && lineNumber == 0) {
        std::cout << "[DEBUG] ВНИМАНИЕ: Файл " << fileName << " пуст или не содержит данных!" << std::endl;
    }
    
    return loadedCount;
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