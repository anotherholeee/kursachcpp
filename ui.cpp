#include "ui.h"
#include "transport_system.h"
#include "exceptions.h"
#include "bus.h"
#include "tram.h"
#include "trolleybus.h"
#include "driver.h"
#include "route.h"
#include "trip.h"
#include "time.h"
#include "stop.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <cctype>
#include <locale>

// ==================== Функции валидации ввода ====================

bool containsOnlyDigits(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool containsOnlyLetters(const std::string& str) {
    if (str.empty()) return false;
    
    bool hasLetter = false;
    for (size_t i = 0; i < str.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        
        // Разрешенные символы: буквы (английские и русские), пробелы, дефисы, апострофы
        if (std::isalpha(c)) {
            hasLetter = true;
        } else if (c == ' ' || c == '-' || c == '\'') {
            // Разрешаем пробелы, дефисы и апострофы
            continue;
        } else if ((c & 0xE0) == 0xC0) {
            // UTF-8: начало двухбайтового символа (русские буквы)
            // Проверяем, что следующий байт существует и корректен
            if (i + 1 < str.length()) {
                unsigned char next = static_cast<unsigned char>(str[i + 1]);
                if ((next & 0xC0) == 0x80) {
                    hasLetter = true;
                    ++i; // Пропускаем следующий байт
                    continue;
                }
            }
            return false;
        } else {
            // Недопустимый символ
            return false;
        }
    }
    
    // Должна быть хотя бы одна буква
    return hasLetter;
}

bool isValidNumber(const std::string& str) {
    return containsOnlyDigits(str);
}

bool isValidText(const std::string& str) {
    // Текст должен содержать только буквы, пробелы, дефисы и апострофы
    // И должен содержать хотя бы одну букву
    return containsOnlyLetters(str);
}

// Проверка, что первая буква слова заглавная (для русского и английского алфавита)
bool hasCapitalFirstLetter(const std::string& str) {
    if (str.empty()) return false;
    
    // Пропускаем пробелы в начале
    size_t start = 0;
    while (start < str.length() && (str[start] == ' ' || str[start] == '\t')) {
        start++;
    }
    if (start >= str.length()) return false;
    
    // Проверяем первый символ
    unsigned char first = static_cast<unsigned char>(str[start]);
    
    // Для английских букв
    if (std::isalpha(first)) {
        return std::isupper(first);
    }
    
    // Для русских букв в UTF-8
    // Русские заглавные буквы: А-Я (0xD0 0x90 - 0xD0 0xAF) и Ё (0xD0 0x81)
    if (str.length() >= start + 2) {
        unsigned char byte1 = static_cast<unsigned char>(str[start]);
        unsigned char byte2 = static_cast<unsigned char>(str[start + 1]);
        
        // Проверяем, что это начало UTF-8 символа (110xxxxx)
        if ((byte1 & 0xE0) == 0xC0) {
            // Русские заглавные буквы: А-Я (0xD0 0x90-0xAF), Ё (0xD0 0x81)
            if (byte1 == 0xD0) {
                // Ё (0x81) или А-Я (0x90-0xAF)
                if (byte2 == 0x81 || (byte2 >= 0x90 && byte2 <= 0xAF)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Проверка, что каждое слово начинается с заглавной буквы
bool isValidNameFormat(const std::string& str) {
    if (str.empty()) return false;
    
    // Проверяем, что первое слово начинается с заглавной буквы
    if (!hasCapitalFirstLetter(str)) {
        return false;
    }
    
    // Проверяем остальные слова (после пробелов, дефисов)
    for (size_t i = 1; i < str.length(); ++i) {
        // Если предыдущий символ - пробел или дефис, следующий должен быть заглавной буквой
        if (str[i-1] == ' ' || str[i-1] == '-') {
            // Пропускаем пробелы/дефисы подряд
            while (i < str.length() && (str[i] == ' ' || str[i] == '-')) {
                i++;
            }
            if (i < str.length()) {
                // Проверяем, что после пробела/дефиса идет заглавная буква
                std::string remaining = str.substr(i);
                if (!hasCapitalFirstLetter(remaining)) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

// Проверка минимальной и максимальной длины
bool isValidLength(const std::string& str, size_t minLen, size_t maxLen) {
    return str.length() >= minLen && str.length() <= maxLen;
}

// Проверка формата номера (может содержать буквы, цифры, пробелы)
bool isValidLicensePlate(const std::string& str) {
    if (str.empty() || str.length() < 2 || str.length() > 20) {
        return false;
    }
    
    // Номерной знак должен содержать хотя бы одну букву или цифру
    bool hasValidChar = false;
    for (char c : str) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            hasValidChar = true;
        } else if (c != ' ' && c != '-' && c != '_') {
            return false; // Недопустимый символ
        }
    }
    
    return hasValidChar;
}

// Проверка формата категории водительских прав
bool isValidDriverCategory(const std::string& str) {
    if (str.empty() || str.length() > 10) {
        return false;
    }
    
    // Категория должна содержать только буквы и цифры
    for (char c : str) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    
    return true;
}

void displayGuestMenu() {
    std::cout << "\n=== ГОСТЕВОЙ РЕЖИМ ===\n";
    std::cout << "1. Просмотр расписания транспорта\n";
    std::cout << "2. Просмотр расписания остановки\n";
    std::cout << "3. Поиск маршрутов между остановками\n";
    std::cout << "0. Выход в главное меню\n";
    std::cout << "Выберите опцию: ";
}

void displayAdminMenu() {
    std::cout << "\n=== АДМИНИСТРАТИВНЫЙ РЕЖИМ ===\n";
    std::cout << "1. Просмотр расписания транспорта\n";
    std::cout << "2. Просмотр расписания остановки\n";
    std::cout << "3. Поиск маршрутов между остановками\n";
    std::cout << "4. Расчет времени прибытия для рейса\n";
    std::cout << "--- АДМИНИСТРАТИВНЫЕ ФУНКЦИИ ---\n";
    std::cout << "5. Добавить маршрут\n";
    std::cout << "6. Добавить рейс\n";
    std::cout << "7. Добавить транспортное средство\n";
    std::cout << "8. Добавить остановку\n";
    std::cout << "9. Добавить водителя\n";
    std::cout << "10. Удалить маршрут\n";
    std::cout << "11. Удалить рейс\n";
    std::cout << "12. Просмотр всех данных\n";
    std::cout << "13. Сохранить данные\n";
    std::cout << "14. Отменить последнее действие\n";
    std::cout << "15. Выход\n";
    std::cout << "Выберите опцию: ";
}

void displayLoginMenu() {
    std::cout << "\n=== СИСТЕМА РАСПИСАНИЯ ГОРОДСКОГО ТРАНСПОРТА ===\n";
    std::cout << "1. Войти как администратор\n";
    std::cout << "2. Войти как гость\n";
    std::cout << "3. Сохранить и выйти\n";
    std::cout << "Выберите опцию: ";
}

void displayAllStopsForSelection(const TransportSystem& system) {
    const auto& stops = system.getStops();
    std::cout << "\n=== СПИСОК ДОСТУПНЫХ ОСТАНОВКИ ===\n";
    for (const auto& stop : stops) {
        std::cout << "• " << stop.getName() << " (ID: " << stop.getId() << ")\n";
    }
    std::cout << "=================================\n";
}

std::string getStopNameByInput(const TransportSystem& system, const std::string& input) {
    const auto& stops = system.getStops();

    try {
        int id = std::stoi(input);
        for (const auto& stop : stops) {
            if (stop.getId() == id) {
                return stop.getName();
            }
        }
    } catch (...) {
    }

    for (const auto& stop : stops) {
        if (stop.getName() == input) {
            return stop.getName();
        }
    }

    throw ContainerException("Остановка не найдена: " + input);
}

void adminAddRoute(TransportSystem& system) {
    try {
        int number;
        std::string vehicleType;
        List<std::string> stops;
        std::string stop;

        std::cout << "Введите номер маршрута: ";
        if (!(std::cin >> number)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для номера маршрута");
        }
        std::cin.ignore();

        std::cout << "Введите тип транспорта (Автобус/Трамвай/Троллейбус): ";
        std::getline(std::cin, vehicleType);
        vehicleType.erase(0, vehicleType.find_first_not_of(" \t"));
        vehicleType.erase(vehicleType.find_last_not_of(" \t") + 1);
        
        if (vehicleType.empty() || !isValidText(vehicleType)) {
            throw InputException("Тип транспорта должен содержать только буквы");
        }
        if (vehicleType != "Автобус" && vehicleType != "Трамвай" && vehicleType != "Троллейбус") {
            throw InputException("Тип транспорта должен быть: Автобус, Трамвай или Троллейбус");
        }

        std::cout << "Введите остановки маршрута (введите 'конец' для завершения):\n";
        while (true) {
            std::cout << "Остановка: ";
            std::getline(std::cin, stop);
            if (stop == "конец") break;
            
            stop.erase(0, stop.find_first_not_of(" \t"));
            stop.erase(stop.find_last_not_of(" \t") + 1);
            
            if (stop.empty()) {
                std::cout << "Ошибка: название остановки не может быть пустым. Попробуйте снова.\n";
                continue;
            }
            if (!isValidText(stop)) {
                std::cout << "Ошибка: название остановки должно содержать только буквы, пробелы, дефисы и апострофы. Попробуйте снова.\n";
                continue;
            }
            if (!isValidLength(stop, 3, 100)) {
                std::cout << "Ошибка: название остановки должно быть от 3 до 100 символов. Попробуйте снова.\n";
                continue;
            }
            if (!isValidNameFormat(stop)) {
                std::cout << "Ошибка: название остановки должно начинаться с заглавной буквы. Попробуйте снова.\n";
                continue;
            }
            stops.push_back(stop);
        }

        auto route = std::make_shared<Route>(number, vehicleType, stops);
        system.addRoute(route);
        std::cout << "Маршрут успешно добавлен!\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void adminAddTrip(TransportSystem& system) {
    try {
        std::cout << "\n=== ДОСТУПНЫЕ МАРШРУТЫ ===\n";
        system.displayAllRoutes();
        std::cout << "==========================\n\n";

        std::cout << "=== ДОСТУПНЫЙ ТРАНСПОРТ ===\n";
        system.displayAllVehicles();
        std::cout << "===========================\n\n";

        const auto& drivers = system.getDrivers();
        if (drivers.empty()) {
            std::cout << "[DEBUG] Список водителей пуст. Количество водителей: " << drivers.size() << '\n';
            throw ContainerException("В системе нет водителей. Сначала добавьте водителя.");
        }

        std::cout << "=== ДОСТУПНЫЕ ВОДИТЕЛИ (всего: " << drivers.size() << ") ===\n";
        for (size_t i = 0; i < drivers.size(); ++i) {
            std::cout << (i+1) << ". " << drivers[i]->getFullName()
                      << " (Имя: " << drivers[i]->getFirstName()
                      << ", Фамилия: " << drivers[i]->getLastName();
            if (!drivers[i]->getMiddleName().empty()) {
                std::cout << ", Отчество: " << drivers[i]->getMiddleName();
            }
            std::cout << ")\n";
        }
        std::cout << "==========================\n\n";

        int tripId, routeNumber, driverChoice;
        std::string licensePlate, startTimeStr;

        std::cout << "Введите ID рейса: ";
        if (!(std::cin >> tripId)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для ID рейса");
        }
        std::cin.ignore();

        std::cout << "Введите номер маршрута: ";
        if (!(std::cin >> routeNumber)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для номера маршрута");
        }
        std::cin.ignore();

        auto route = system.findRouteByNumber(routeNumber);
        if (!route) {
            throw ContainerException("Маршрут не найден!");
        }

        std::cout << "Введите номерной знак транспортного средства: ";
        std::getline(std::cin, licensePlate);
        if (licensePlate.empty()) {
            throw InputException("Номерной знак не может быть пустым");
        }

        auto vehicle = system.findVehicleByLicensePlate(licensePlate);
        if (!vehicle) {
            throw ContainerException("Транспортное средство не найдено!");
        }

        std::cout << "Выберите водителя (введите номер из списка): ";
        if (!(std::cin >> driverChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для выбора водителя");
        }
        std::cin.ignore();

        if (driverChoice < 1 || driverChoice > static_cast<int>(drivers.size())) {
            throw InputException("Неверный выбор водителя. Допустимые значения: 1-" +
                                std::to_string(drivers.size()));
        }

        auto driver = drivers[driverChoice - 1];

        std::cout << "Введите время отправления (HH:MM): ";
        std::getline(std::cin, startTimeStr);
        // Проверка формата времени HH:MM
        if (startTimeStr.length() != 5 || startTimeStr[2] != ':' ||
            !std::isdigit(static_cast<unsigned char>(startTimeStr[0])) ||
            !std::isdigit(static_cast<unsigned char>(startTimeStr[1])) ||
            !std::isdigit(static_cast<unsigned char>(startTimeStr[3])) ||
            !std::isdigit(static_cast<unsigned char>(startTimeStr[4]))) {
            throw InputException("Неверный формат времени. Используйте формат HH:MM (например, 08:30)");
        }

        std::cout << "Введите день недели (1-понедельник, 2-вторник, ..., 7-воскресенье): ";
        int weekDay;
        if (!(std::cin >> weekDay)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для дня недели");
        }
        std::cin.ignore();

        if (weekDay < 1 || weekDay > 7) {
            throw InputException("День недели должен быть от 1 до 7");
        }

        Time startTime(startTimeStr);
        auto trip = std::make_shared<Trip>(tripId, route, vehicle, driver, startTime, weekDay);
        system.addTrip(trip);
        std::cout << "Рейс успешно добавлен!\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void adminAddVehicle(TransportSystem& system) {
    try {
        std::string type, model, licensePlate;

        std::cout << "Введите тип транспорта (Автобус/Трамвай/Троллейбус): ";
        std::getline(std::cin, type);
        type.erase(0, type.find_first_not_of(" \t"));
        type.erase(type.find_last_not_of(" \t") + 1);
        
        if (type.empty() || !isValidText(type)) {
            throw InputException("Тип транспорта должен содержать только буквы");
        }
        if (type != "Автобус" && type != "Трамвай" && type != "Троллейбус") {
            throw InputException("Тип транспорта должен быть: Автобус, Трамвай или Троллейбус");
        }

        std::cout << "Введите модель: ";
        std::getline(std::cin, model);
        model.erase(0, model.find_first_not_of(" \t"));
        model.erase(model.find_last_not_of(" \t") + 1);
        
        if (model.empty()) {
            throw InputException("Модель не может быть пустой");
        }
        if (!isValidLength(model, 2, 50)) {
            throw InputException("Модель должна быть от 2 до 50 символов");
        }
        if (!isValidNameFormat(model)) {
            throw InputException("Модель должна начинаться с заглавной буквы (например: ЛиАЗ-5256, Трамвай-71)");
        }

        std::cout << "Введите номерной знак: ";
        std::getline(std::cin, licensePlate);
        licensePlate.erase(0, licensePlate.find_first_not_of(" \t"));
        licensePlate.erase(licensePlate.find_last_not_of(" \t") + 1);
        
        if (licensePlate.empty()) {
            throw InputException("Номерной знак не может быть пустым");
        }
        if (!isValidLicensePlate(licensePlate)) {
            throw InputException("Номерной знак должен содержать буквы, цифры, пробелы, дефисы или подчеркивания (от 2 до 20 символов)");
        }

        std::shared_ptr<Vehicle> vehicle;
        if (type == "Автобус") {
            vehicle = std::make_shared<Bus>(model, licensePlate);
        } else if (type == "Трамвай") {
            vehicle = std::make_shared<Tram>(model, licensePlate);
        } else if (type == "Троллейбус") {
            vehicle = std::make_shared<Trolleybus>(model, licensePlate);
        }

        system.addVehicle(vehicle);
        std::cout << "Транспортное средство успешно добавлено!\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void adminAddStop(TransportSystem& system) {
    try {
        int id;
        std::string name;

        std::cout << "Введите ID остановки: ";
        if (!(std::cin >> id)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для ID остановки");
        }
        std::cin.ignore();
        
        if (id < 1 || id > 9999) {
            throw InputException("ID остановки должен быть от 1 до 9999");
        }

        std::cout << "Введите название остановки: ";
        std::getline(std::cin, name);
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);
        
        if (name.empty()) {
            throw InputException("Название остановки не может быть пустым");
        }
        if (!isValidText(name)) {
            throw InputException("Название остановки должно содержать только буквы, пробелы, дефисы и апострофы");
        }
        if (!isValidLength(name, 3, 100)) {
            throw InputException("Название остановки должно быть от 3 до 100 символов");
        }
        if (!isValidNameFormat(name)) {
            throw InputException("Название остановки должно начинаться с заглавной буквы (например: Центральная площадь, Улица Ленина)");
        }

        system.addStop(Stop(id, name));
        std::cout << "Остановка успешно добавлена!\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void adminAddDriver(TransportSystem& system) {
    try {
        std::string firstName, lastName, middleName, category;

        std::cout << "Введите имя водителя: ";
        std::getline(std::cin, firstName);
        // Убираем пробелы в начале и конце
        firstName.erase(0, firstName.find_first_not_of(" \t"));
        firstName.erase(firstName.find_last_not_of(" \t") + 1);
        
        if (firstName.empty()) {
            throw InputException("Имя водителя не может быть пустым");
        }
        if (!isValidText(firstName)) {
            throw InputException("Имя водителя должно содержать только буквы, пробелы, дефисы и апострофы");
        }
        if (!isValidLength(firstName, 2, 50)) {
            throw InputException("Имя водителя должно быть от 2 до 50 символов");
        }
        if (!isValidNameFormat(firstName)) {
            throw InputException("Имя водителя должно начинаться с заглавной буквы (например: Иван, Мария-Луиза)");
        }
        
        std::cout << "Введите фамилию водителя: ";
        std::getline(std::cin, lastName);
        lastName.erase(0, lastName.find_first_not_of(" \t"));
        lastName.erase(lastName.find_last_not_of(" \t") + 1);
        
        if (lastName.empty()) {
            throw InputException("Фамилия водителя не может быть пустой");
        }
        if (!isValidText(lastName)) {
            throw InputException("Фамилия водителя должна содержать только буквы, пробелы, дефисы и апострофы");
        }
        if (!isValidLength(lastName, 2, 50)) {
            throw InputException("Фамилия водителя должна быть от 2 до 50 символов");
        }
        if (!isValidNameFormat(lastName)) {
            throw InputException("Фамилия водителя должна начинаться с заглавной буквы (например: Иванов, Петров-Сидоров)");
        }
        
        std::cout << "Введите отчество водителя (если есть, иначе Enter): ";
        std::getline(std::cin, middleName);
        middleName.erase(0, middleName.find_first_not_of(" \t"));
        middleName.erase(middleName.find_last_not_of(" \t") + 1);
        
        if (!middleName.empty()) {
            if (!isValidText(middleName)) {
                throw InputException("Отчество водителя должно содержать только буквы, пробелы, дефисы и апострофы");
            }
            if (!isValidLength(middleName, 2, 50)) {
                throw InputException("Отчество водителя должно быть от 2 до 50 символов");
            }
            if (!isValidNameFormat(middleName)) {
                throw InputException("Отчество водителя должно начинаться с заглавной буквы (например: Иванович, Петровна)");
            }
        }
        
        std::cout << "Введите категорию водительских прав (D, T, DT, B, C и т.д.): ";
        std::getline(std::cin, category);
        category.erase(0, category.find_first_not_of(" \t"));
        category.erase(category.find_last_not_of(" \t") + 1);
        
        if (category.empty()) {
            throw InputException("Категория водительских прав не может быть пустой");
        }
        if (!isValidDriverCategory(category)) {
            throw InputException("Категория водительских прав должна содержать только буквы и цифры (например: D, T, DT, B, C)");
        }
        if (containsOnlyDigits(category)) {
            throw InputException("Категория водительских прав не может содержать только цифры");
        }

        auto driver = std::make_shared<Driver>(firstName, lastName, middleName, category);
        system.addDriver(driver);
        std::cout << "Водитель успешно добавлен!\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}


void searchRoutes(TransportSystem& system) {
    try {
        std::string stopAInput, stopBInput;

        displayAllStopsForSelection(system);

        std::cout << "\nВведите начальную остановку (ID или название): ";
        std::getline(std::cin, stopAInput);
        std::cout << "Введите конечную остановку (ID или название): ";
        std::getline(std::cin, stopBInput);

        std::string stopA = getStopNameByInput(system, stopAInput);
        std::string stopB = getStopNameByInput(system, stopBInput);

        auto routes = system.findRoutes(stopA, stopB);
        std::cout << "\nНайдено маршрутов: " << routes.size() << '\n';

        if (routes.empty()) {
            std::cout << "Прямых маршрутов не найдено. Попробуйте поиск с пересадками.\n";
        } else {
            for (const auto& route : routes) {
                std::cout << "\nМаршрут " << route->getNumber() << " ("
                          << route->getVehicleType() << ")\n";

                const auto& allStops = route->getAllStops();
                int startPos = route->getStopPosition(stopA);
                int endPos = route->getStopPosition(stopB);

                if (startPos != -1 && endPos != -1 && startPos < endPos) {
                    std::cout << "Путь: " << allStops[startPos];
                    for (int i = startPos + 1; i <= endPos; i++) {
                        std::cout << " → " << allStops[i];
                    }
                    std::cout << '\n';
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void viewStopTimetable(TransportSystem& system) {
    try {
        displayAllStopsForSelection(system);

        std::string stopInput;
        std::cout << "\nВведите остановку (ID или название): ";
        std::getline(std::cin, stopInput);

        std::string stopName = getStopNameByInput(system, stopInput);
        system.getStopTimetableAll(stopName);
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void viewTransportScheduleGuest(TransportSystem& system) {
    try {
        std::cout << "\n=== ВЫБОР ТИПА ТРАНСПОРТА ===\n";
        std::cout << "1. Автобус\n";
        std::cout << "2. Троллейбус\n";
        std::cout << "3. Трамвай\n";
        std::cout << "Выберите тип транспорта: ";

        int transportTypeChoice;
        if (!(std::cin >> transportTypeChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для типа транспорта");
        }
        std::cin.ignore();

        std::string selectedType;
        switch (transportTypeChoice) {
            case 1: selectedType = "Автобус"; break;
            case 2: selectedType = "Троллейбус"; break;
            case 3: selectedType = "Трамвай"; break;
            default:
                throw InputException("Неверный выбор типа транспорта. Допустимые значения: 1-3");
        }

        const auto& trips = system.getTrips();
        List<std::shared_ptr<Trip>> filteredTrips;

        for (const auto& trip : trips) {
            if (trip->getRoute()->getVehicleType() == selectedType) {
                filteredTrips.push_back(trip);
            }
        }

        if (filteredTrips.empty()) {
            std::cout << "\nРейсов типа '" << selectedType << "' не найдено.\n";
            return;
        }

        std::string dayNames[] = {"", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};

        std::cout << "\n=== РАСПИСАНИЕ ТРАНСПОРТА: " << selectedType << " ===\n";
        std::cout << "========================================\n";

        for (const auto& trip : filteredTrips) {
            std::cout << "\nРейс " << trip->getTripId() << ":\n";
            std::cout << "  Маршрут: " << trip->getRoute()->getNumber() << "\n";
            std::cout << "  День недели: " << dayNames[trip->getWeekDay()] << "\n";
            std::cout << "  Время отправления: " << trip->getStartTime() << "\n";
            std::cout << "  Транспорт: " << trip->getVehicle()->getInfo() << "\n";
            std::cout << "  Водитель: " << trip->getDriver()->getFullName() << "\n";

            const auto& schedule = trip->getSchedule();
            if (!schedule.empty()) {
                std::cout << "  Расписание:\n";
                for (const auto& [stop, time] : schedule) {
                    std::cout << "    " << stop << " - " << time << "\n";
                }
            } else {
                std::cout << "  Расписание: не рассчитано\n";
            }
            std::cout << "  ---\n";
        }
        std::cout << "========================================\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void viewTransportSchedule(TransportSystem& system) {
    try {
        std::cout << "\n=== ВЫБОР ТИПА ТРАНСПОРТА ===\n";
        std::cout << "1. Автобус\n";
        std::cout << "2. Троллейбус\n";
        std::cout << "3. Трамвай\n";
        std::cout << "Выберите тип транспорта: ";

        int transportTypeChoice;
        if (!(std::cin >> transportTypeChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для типа транспорта");
        }
        std::cin.ignore();

        std::string selectedType;
        switch (transportTypeChoice) {
            case 1: selectedType = "Автобус"; break;
            case 2: selectedType = "Троллейбус"; break;
            case 3: selectedType = "Трамвай"; break;
            default:
                throw InputException("Неверный выбор типа транспорта. Допустимые значения: 1-3");
        }

        const auto& routes = system.getRoutes();
        List<std::shared_ptr<Route>> filteredRoutes;
        for (const auto& route : routes) {
            if (route->getVehicleType() == selectedType) {
                filteredRoutes.push_back(route);
            }
        }

        if (filteredRoutes.empty()) {
            std::cout << "Маршрутов типа '" << selectedType << "' не найдено.\n";
            return;
        }

        std::cout << "\n=== ДОСТУПНЫЕ МАРШРУТЫ (" << selectedType << ") ===\n";
        for (size_t i = 0; i < filteredRoutes.size(); ++i) {
            std::cout << (i + 1) << ". Маршрут " << filteredRoutes[i]->getNumber()
                      << ": " << filteredRoutes[i]->getStartStop()
                      << " → " << filteredRoutes[i]->getEndStop() << "\n";
        }
        std::cout << "0. Назад\n";
        std::cout << "Выберите маршрут: ";

        int routeChoice;
        if (!(std::cin >> routeChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для выбора маршрута");
        }
        std::cin.ignore();

        if (routeChoice == 0) {
            return;
        }

        if (routeChoice < 1 || routeChoice > static_cast<int>(filteredRoutes.size())) {
            throw InputException("Неверный выбор маршрута. Допустимые значения: 0-" +
                                std::to_string(filteredRoutes.size()));
        }

        auto selectedRoute = filteredRoutes[routeChoice - 1];

        std::cout << "\n=== ВЫБОР НАПРАВЛЕНИЯ ===\n";
        std::cout << "1. Прямой (" << selectedRoute->getStartStop()
                  << " → " << selectedRoute->getEndStop() << ")\n";
        std::cout << "2. Обратный (" << selectedRoute->getEndStop()
                  << " → " << selectedRoute->getStartStop() << ")\n";
        std::cout << "0. Назад\n";
        std::cout << "Выберите направление: ";

        int directionChoice;
        if (!(std::cin >> directionChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для выбора направления");
        }
        std::cin.ignore();

        if (directionChoice == 0) {
            return;
        }

        if (directionChoice != 1 && directionChoice != 2) {
            throw InputException("Неверный выбор направления. Допустимые значения: 0, 1 или 2");
        }

        List<std::string> routeStops = selectedRoute->getAllStops();
        if (directionChoice == 2) {
            // Используем метод reverse
            routeStops.reverse();
        }

        std::cout << "\n=== ОСТАНОВКИ МАРШРУТА ===\n";
        for (size_t i = 0; i < routeStops.size(); ++i) {
            std::cout << (i + 1) << ". " << routeStops[i] << "\n";
        }
        std::cout << "0. Назад\n";
        std::cout << "Выберите остановку: ";

        int stopChoice;
        if (!(std::cin >> stopChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для выбора остановки");
        }
        std::cin.ignore();

        if (stopChoice == 0) {
            return;
        }

        if (stopChoice < 1 || stopChoice > static_cast<int>(routeStops.size())) {
            throw InputException("Неверный выбор остановки. Допустимые значения: 0-" +
                                std::to_string(routeStops.size()));
        }

        std::string selectedStop = routeStops[stopChoice - 1];

        std::cout << "\n=== ВЫБОР ДНЯ НЕДЕЛИ ===\n";
        std::cout << "1. Понедельник\n";
        std::cout << "2. Вторник\n";
        std::cout << "3. Среда\n";
        std::cout << "4. Четверг\n";
        std::cout << "5. Пятница\n";
        std::cout << "6. Суббота\n";
        std::cout << "7. Воскресенье\n";
        std::cout << "0. Назад\n";
        std::cout << "Выберите день недели (0-7): ";

        int weekDayChoice;
        if (!(std::cin >> weekDayChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для дня недели");
        }
        std::cin.ignore();

        if (weekDayChoice == 0) {
            return;
        }

        if (weekDayChoice < 1 || weekDayChoice > 7) {
            throw InputException("Неверный выбор дня недели. Допустимые значения: 0-7");
        }

        const auto& trips = system.getTrips();
        List<std::pair<int, Time>> stopTimes;

        for (const auto& trip : trips) {
            if (trip->getRoute()->getNumber() == selectedRoute->getNumber() &&
                trip->getRoute()->getVehicleType() == selectedType) {
                int tripDay = trip->getWeekDay();
                bool dayMatches = false;
                if (weekDayChoice >= 1 && weekDayChoice <= 5) {
                    dayMatches = (tripDay >= 1 && tripDay <= 5);
                } else {
                    dayMatches = (tripDay == weekDayChoice);
                }

                if (dayMatches && trip->hasStop(selectedStop)) {
                    Time arrivalTime = trip->getArrivalTime(selectedStop);
                    stopTimes.push_back({trip->getTripId(), arrivalTime});
                }
            }
        }

        // Сортируем используя метод sort
        stopTimes.sort([](const auto& a, const auto& b) { return a.second < b.second; });

        std::string dayNames[] = {"", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
        std::cout << "\n=== РАСПИСАНИЕ ДЛЯ ОСТАНОВКИ '" << selectedStop << "' ===\n";
        std::cout << "Маршрут: " << selectedRoute->getNumber() << " (" << selectedType << ")\n";
        std::cout << "Направление: " << (directionChoice == 1 ? "Прямое" : "Обратное") << "\n";
        std::cout << "День недели: " << dayNames[weekDayChoice] << "\n";
        std::cout << "----------------------------------------\n";

        if (stopTimes.empty()) {
            std::cout << "Рейсов не найдено для этой остановки.\n";
        } else {
            for (const auto& [tripId, time] : stopTimes) {
                std::cout << "Рейс " << tripId << " - прибытие в " << time << "\n";
            }
        }
        std::cout << "========================================\n";

    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
}

void calculateArrivalTime(TransportSystem& system) {
    try {
        const auto& trips = system.getTrips();
        if (trips.empty()) {
            std::cout << "В системе нет рейсов.\n";
            return;
        }

        std::cout << "\n=== ДОСТУПНЫЕ РЕЙСЫ ===\n";
        for (const auto& trip : trips) {
            std::cout << "ID: " << trip->getTripId()
                      << " | Маршрут: " << trip->getRoute()->getNumber()
                      << " | Отправление: " << trip->getStartTime()
                      << " | Направление: " << trip->getRoute()->getStartStop()
                      << " → " << trip->getRoute()->getEndStop() << '\n';
        }
        std::cout << "========================\n\n";

        int tripId;
        double speed;

        std::cout << "Введите ID рейса: ";
        if (!(std::cin >> tripId)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для ID рейса");
        }
        std::cout << "Введите среднюю скорость (км/ч): ";
        if (!(std::cin >> speed)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InputException("Неверный формат ввода для скорости");
        }
        std::cin.ignore();

        system.calculateArrivalTimes(tripId, speed);

        auto tripIt = std::find_if(trips.begin(), trips.end(),
                                  [tripId](const auto& t) { return t->getTripId() == tripId; });
        if (tripIt != trips.end()) {
            auto trip = *tripIt;
            std::cout << "\nОбновленное расписание для рейса " << tripId << ":\n";
            const auto& schedule = trip->getSchedule();
            for (const auto& [stop, time] : schedule) {
                std::cout << "  " << stop << " - " << time << '\n';
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void showAllTrips(const TransportSystem& system) {
    const auto& trips = system.getTrips();
    if (trips.empty()) {
        std::cout << "\nВ системе нет рейсов.\n";
        return;
    }
    std::cout << "\nВсе рейсы в системе:\n";
    for (const auto& trip : trips) {
        std::cout << "Рейс " << trip->getTripId()
                  << ": Маршрут " << trip->getRoute()->getNumber()
                  << ", отправление: " << trip->getStartTime()
                  << ", водитель: " << trip->getDriver()->getFullName()
                  << ", транспорт: " << trip->getVehicle()->getLicensePlate() << '\n';
    }
}

void runGuestMode(TransportSystem& system) {
    int choice;
    bool running = true;

    while (running) {
        displayGuestMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: ожидается число. Пожалуйста, введите только цифры.\n";
            continue;
        }
        std::cin.ignore();

        try {
            switch (choice) {
                case 0: running = false; break;
                case 1: viewTransportScheduleGuest(system); break;
                case 2: viewStopTimetable(system); break;
                case 3: searchRoutes(system); break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const InputException& e) {
            std::cout << e.what() << '\n';
        } catch (const FileException& e) {
            std::cout << "Ошибка файла: " << e.what() << '\n';
        } catch (const ContainerException& e) {
            std::cout << "Ошибка данных: " << e.what() << '\n';
        } catch (const TransportException& e) {
            std::cout << "Ошибка системы: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "Неожиданная ошибка: " << e.what() << '\n';
        }
    }
}

void runAdminMode(TransportSystem& system) {
    std::string username, password;

    std::cout << "Введите логин: ";
    std::getline(std::cin, username);
    std::cout << "Введите пароль: ";
    std::getline(std::cin, password);

    if (!system.authenticateAdmin(username, password)) {
        std::cout << "Неверный логин или пароль!\n";
        return;
    }

    std::cout << "Успешный вход!\n";

    int choice;
    bool running = true;

    while (running) {
        displayAdminMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: ожидается число. Пожалуйста, введите только цифры.\n";
            continue;
        }
        std::cin.ignore();

        try {
            switch (choice) {
                case 1: viewTransportSchedule(system); break;
                case 2: viewStopTimetable(system); break;
                case 3: searchRoutes(system); break;
                case 4: calculateArrivalTime(system); break;
                case 5: adminAddRoute(system); break;
                case 6: adminAddTrip(system); break;
                case 7: adminAddVehicle(system); break;
                case 8: adminAddStop(system); break;
                case 9: adminAddDriver(system); break;
                case 10: {
                    const auto& routes = system.getRoutes();
                    if (routes.empty()) {
                        std::cout << "\nВ системе нет маршрутов для удаления.\n";
                        break;
                    }

                    std::cout << "\n=== ДОСТУПНЫЕ МАРШРУТЫ ДЛЯ УДАЛЕНИЯ ===\n";
                    for (size_t i = 0; i < routes.size(); ++i) {
                        std::cout << (i + 1) << ". Маршрут " << routes[i]->getNumber()
                                  << " (" << routes[i]->getVehicleType() << "): "
                                  << routes[i]->getStartStop() << " → " << routes[i]->getEndStop() << "\n";
                    }
                    std::cout << "==========================================\n";
                    std::cout << "Выберите номер маршрута для удаления: ";

                    int routeChoice;
                    if (!(std::cin >> routeChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw InputException("Неверный формат ввода");
                    }
                    std::cin.ignore();

                    if (routeChoice < 1 || routeChoice > static_cast<int>(routes.size())) {
                        throw InputException("Неверный выбор маршрута. Допустимые значения: 1-" + std::to_string(routes.size()));
                    }

                    int routeNumber = routes[routeChoice - 1]->getNumber();
                    system.removeRoute(routeNumber);
                    break;
                }
                case 11: {
                    const auto& trips = system.getTrips();
                    if (trips.empty()) {
                        std::cout << "\nВ системе нет рейсов для удаления.\n";
                        break;
                    }

                    std::cout << "\n=== ДОСТУПНЫЕ РЕЙСЫ ДЛЯ УДАЛЕНИЯ ===\n";
                    for (size_t i = 0; i < trips.size(); ++i) {
                        std::cout << (i + 1) << ". Рейс " << trips[i]->getTripId()
                                  << ": Маршрут " << trips[i]->getRoute()->getNumber()
                                  << ", отправление: " << trips[i]->getStartTime()
                                  << ", водитель: " << trips[i]->getDriver()->getFullName()
                                  << ", транспорт: " << trips[i]->getVehicle()->getLicensePlate() << "\n";
                    }
                    std::cout << "=====================================\n";
                    std::cout << "Выберите номер рейса для удаления: ";

                    int tripChoice;
                    if (!(std::cin >> tripChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw InputException("Неверный формат ввода");
                    }
                    std::cin.ignore();

                    if (tripChoice < 1 || tripChoice > static_cast<int>(trips.size())) {
                        throw InputException("Неверный выбор рейса. Допустимые значения: 1-" + std::to_string(trips.size()));
                    }

                    int tripId = trips[tripChoice - 1]->getTripId();
                    system.removeTrip(tripId);
                    break;
                }
                case 12: {
                    system.displayAllRoutes();
                    system.displayAllTrips();
                    system.displayAllVehicles();
                    system.displayAllStops();
                    break;
                }
                case 13: system.saveData(); break;
                case 14: {
                    if (system.canUndo()) {
                        std::string lastAction = system.getLastCommandDescription();
                        system.undo();
                        std::cout << "Отменено действие: " << lastAction << "\n";
                    } else {
                        std::cout << "Нет действий для отмены.\n";
                    }
                    break;
                }
                case 15:
                    system.saveData();
                    std::cout << "Данные сохранены. Выход из административного режима.\n";
                    running = false;
                    break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const InputException& e) {
            std::cout << e.what() << '\n';
        } catch (const FileException& e) {
            std::cout << "Ошибка файла: " << e.what() << '\n';
        } catch (const ContainerException& e) {
            std::cout << "Ошибка данных: " << e.what() << '\n';
        } catch (const TransportException& e) {
            std::cout << "Ошибка системы: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "Неожиданная ошибка: " << e.what() << '\n';
        }
    }
}