#include "route.h"
#include <sstream>

// Конструктор маршрута
// Первая остановка становится начальной, последняя - конечной
Route::Route(int num, const std::string& vType, const List<std::string>& stops,
          const std::set<int>& days)
    : number(num), vehicleType(vType), allStops(stops), weekDays(days) {
    // Проверка: маршрут должен содержать хотя бы одну остановку
    if (stops.empty()) {
        throw ContainerException("Маршрут не может быть пустым");
    }
    // Первая остановка - начальная, последняя - конечная
    startStop = stops.front();
    endStop = stops.back();
}

// Проверяет, содержит ли маршрут указанную остановку
bool Route::containsStop(const std::string& stop) const {
    return std::find(allStops.begin(), allStops.end(), stop) != allStops.end();
}

// Возвращает позицию остановки в маршруте (0 - первая остановка)
// Возвращает -1, если остановка не найдена
int Route::getStopPosition(const std::string& stop) const {
    int pos = 0;
    for (const auto& s : allStops) {
        if (s == stop) {
            return pos;
        }
        pos++;
    }
    return -1;  // Остановка не найдена
}

// Проверяет, идет ли остановка stopA раньше остановки stopB в маршруте
bool Route::isStopBefore(const std::string& stopA, const std::string& stopB) const {
    int posA = getStopPosition(stopA);
    int posB = getStopPosition(stopB);
    // Обе остановки должны существовать в маршруте, и stopA должна быть раньше stopB
    return posA != -1 && posB != -1 && posA < posB;
}

// Возвращает номер маршрута
int Route::getNumber() const {
    return number;
}

// Возвращает тип транспортного средства для маршрута
std::string Route::getVehicleType() const {
    return vehicleType;
}

// Возвращает название начальной остановки
std::string Route::getStartStop() const {
    return startStop;
}

// Возвращает название конечной остановки
std::string Route::getEndStop() const {
    return endStop;
}

// Возвращает список всех остановок маршрута в порядке следования
const List<std::string>& Route::getAllStops() const {
    return allStops;
}

// Возвращает множество дней недели, когда работает маршрут (1-7)
const std::set<int>& Route::getWeekDays() const {
    return weekDays;
}

// Проверяет, работает ли маршрут в указанный день недели (1-понедельник, 7-воскресенье)
bool Route::operatesOnDay(int day) const {
    return weekDays.find(day) != weekDays.end();
}

// Сериализация маршрута в строку формата "number|vehicleType|stop1;stop2;...|1,2,3,4,5,6,7"
std::string Route::serialize() const {
    std::string result = std::to_string(number) + "|" + vehicleType + "|";
    // Добавляем остановки через точку с запятой
    for (size_t i = 0; i < allStops.size(); ++i) {
        result += allStops[i];
        if (i < allStops.size() - 1) result += ";";
    }
    result += "|";
    // Добавляем дни недели через запятую
    for (auto it = weekDays.begin(); it != weekDays.end(); ++it) {
        result += std::to_string(*it);
        if (std::next(it) != weekDays.end()) result += ",";
    }
    return result;
}

// Десериализация маршрута из строки
// Если дни недели не указаны, используется значение по умолчанию (все дни недели)
std::shared_ptr<Route> Route::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string numberStr, vehicleType, stopsStr, daysStr;
    std::getline(ss, numberStr, '|');    // Номер маршрута
    std::getline(ss, vehicleType, '|');   // Тип транспорта
    std::getline(ss, stopsStr, '|');     // Список остановок
    std::getline(ss, daysStr);           // Дни недели

    // Парсим остановки (разделены точкой с запятой)
    List<std::string> stops;
    std::istringstream stopsStream(stopsStr);
    std::string stop;
    while (std::getline(stopsStream, stop, ';')) {
        stops.push_back(stop);
    }

    // Парсим дни недели (разделены запятой)
    std::set<int> weekDays;
    if (!daysStr.empty()) {
        std::istringstream daysStream(daysStr);
        std::string day;
        while (std::getline(daysStream, day, ',')) {
            weekDays.insert(std::stoi(day));
        }
    } else {
        // По умолчанию - все дни недели
        weekDays = {1,2,3,4,5,6,7};
    }

    return std::make_shared<Route>(std::stoi(numberStr), vehicleType, stops, weekDays);
}