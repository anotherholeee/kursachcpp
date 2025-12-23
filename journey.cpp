#include "journey.h"

/**
 * @brief Конструктор поездки
 * @param tripList Список рейсов, составляющих поездку
 * @param transfers Список остановок, где происходят пересадки
 * @param start Время начала поездки
 * @param end Время окончания поездки
 * 
 * Создает объект поездки, который может состоять из нескольких рейсов с пересадками.
 */
Journey::Journey(const List<std::shared_ptr<Trip>>& tripList,
            const List<std::string>& transfers,
            Time start, Time end)
    : trips(tripList), transferPoints(transfers),
      startTime(start), endTime(end),
      transferCount(static_cast<int>(transfers.size())) {}

// Возвращает общую продолжительность поездки в минутах
int Journey::getTotalDuration() const {
    return endTime - startTime;
}

// Возвращает количество пересадок в поездке
int Journey::getTransferCount() const {
    return transferCount;
}

// Возвращает время начала поездки
Time Journey::getStartTime() const {
    return startTime;
}

// Возвращает время окончания поездки
Time Journey::getEndTime() const {
    return endTime;
}

// Возвращает список рейсов, составляющих поездку
const List<std::shared_ptr<Trip>>& Journey::getTrips() const {
    return trips;
}

// Возвращает список остановок, где происходят пересадки
const List<std::string>& Journey::getTransferPoints() const {
    return transferPoints;
}

// Выводит информацию о поездке в консоль
void Journey::display() const {
    std::cout << "\nМаршрут поездки:\n";
    std::cout << "Общее время: " << getTotalDuration() << " минут\n";
    std::cout << "Пересадок: " << transferCount << "\n";
    std::cout << "Время отправления: " << startTime << "\n";
    std::cout << "Время прибытия: " << endTime << "\n";

    // Выводим информацию о каждом этапе поездки
    for (size_t i = 0; i < trips.size(); ++i) {
        std::cout << "\nЭтап " << (i + 1) << ":\n";
        std::cout << "  Маршрут: " << trips[i]->getRoute()->getNumber() << "\n";
        std::cout << "  Транспорт: " << trips[i]->getVehicle()->getInfo() << "\n";

        // Если это не первый этап, выводим информацию о пересадке
        if (i > 0) {
            std::cout << "  Пересадка на: " << transferPoints[i - 1] << "\n";
        }
    }
}