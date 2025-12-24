#include "algorithm.h"
#include "transport_system.h"
#include "exceptions.h"
#include <queue>
#include <algorithm>

// Поиск маршрутов с использованием алгоритма BFS (поиск в ширину)
// Находит все возможные маршруты между остановками с учетом ограничения на количество пересадок
List<Journey> BFSAlgorithm::findPath(const std::string& start,
                                           const std::string& end,
                                           const Time& departureTime) {
    List<Journey> journeys;

    // Узел поиска: содержит текущую остановку, время, пройденный путь и пересадки
    struct SearchNode {
        std::string currentStop;              // Текущая остановка
        Time currentTime;                     // Текущее время
        List<std::shared_ptr<Trip>> pathTrips; // Рейсы, использованные в пути
        List<std::string> transferPoints;     // Остановки, где были пересадки
        int transfers;                        // Количество пересадок
    };

    std::queue<SearchNode> q;
    // Начинаем поиск с начальной остановки
    q.push({start, departureTime, {}, {}, 0});

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        // Если достигли конечной остановки - сохраняем найденный маршрут
        if (node.currentStop == end) {
            journeys.push_back(Journey(node.pathTrips, node.transferPoints,
                                 departureTime, node.currentTime));
            continue;
        }

        // Пропускаем узлы с превышением лимита пересадок
        if (node.transfers >= maxTransfers) {
            continue;
        }

        // Получаем все рейсы, проходящие через текущую остановку
        auto trips = system->getTripsThroughStop(node.currentStop);

        for (const auto& trip : trips) {
            // Проверяем, что время прибытия рассчитано для текущей остановки
            if (!trip->hasStop(node.currentStop)) {
                continue;
            }

            Time arrivalAtStop = trip->getArrivalTime(node.currentStop);

            // Пропускаем рейсы, которые уже прошли (время прибытия раньше текущего)
            if (arrivalAtStop < node.currentTime) {
                continue;
            }

            // Пропускаем, если это тот же рейс, что и предыдущий (избегаем циклов)
            if (!node.pathTrips.empty() && node.pathTrips.back() == trip) {
                continue;
            }

            // Получаем список остановок маршрута и позицию текущей остановки
            const auto& routeStops = trip->getRoute()->getAllStops();
            int currentPos = trip->getRoute()->getStopPosition(node.currentStop);

            if (currentPos == -1) continue;  // Остановка не найдена в маршруте

            // Проверяем все последующие остановки на этом маршруте
            for (int i = currentPos + 1; i < routeStops.size(); ++i) {
                std::string nextStop = routeStops[i];

                // Проверяем, что время прибытия рассчитано для следующей остановки
                if (!trip->hasStop(nextStop)) {
                    continue;
                }

                Time arrivalAtNext = trip->getArrivalTime(nextStop);

                // Создаем новый узел для следующей остановки
                SearchNode nextNode = node;
                nextNode.currentStop = nextStop;
                nextNode.currentTime = arrivalAtNext;
                nextNode.pathTrips.push_back(trip);

                // Пересадка считается только если мы переходим на другой рейс
                if (!node.pathTrips.empty() && node.pathTrips.back() != trip) {
                    nextNode.transferPoints.push_back(node.currentStop);
                    nextNode.transfers++;
                }

                // Добавляем новый узел в очередь для дальнейшего поиска
                q.push(nextNode);
            }
        }
    }

    // Сортируем найденные маршруты по времени в пути (от быстрых к медленным)
    journeys.sort([](const Journey& a, const Journey& b) {
        return a.getTotalDuration() < b.getTotalDuration();
    });

    return journeys;
}

// Поиск самого быстрого маршрута
// Использует BFS для поиска всех маршрутов, затем выбирает самый быстрый
List<Journey> FastestPathAlgorithm::findPath(const std::string& start,
                                                   const std::string& end,
                                                   const Time& departureTime) {
    BFSAlgorithm bfs(system, 2);
    auto journeys = bfs.findPath(start, end, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    List<Journey> result;
    // BFS уже отсортировал маршруты по времени, берем первый (самый быстрый)
    result.push_back(journeys[0]);
    return result;
}

// Поиск маршрута с минимальным количеством пересадок
// Использует BFS для поиска всех маршрутов, затем выбирает маршрут с наименьшим количеством пересадок
List<Journey> MinimalTransfersAlgorithm::findPath(const std::string& start,
                                                         const std::string& end,
                                                         const Time& departureTime) {
    BFSAlgorithm bfs(system, 2);
    auto journeys = bfs.findPath(start, end, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    // Находим маршрут с минимальным количеством пересадок
    auto it = std::min_element(journeys.begin(), journeys.end(),
                               [](const Journey& a, const Journey& b) {
                                   return a.getTransferCount() < b.getTransferCount();
                               });

    List<Journey> result;
    result.push_back(*it);
    return result;
}

// Расчет времени прибытия на остановки для рейса
// Вычисляет время прибытия на каждую остановку маршрута на основе времени отправления,
// средней скорости движения, расстояния между остановками (1.5 км) и времени стоянки (1 минута)
void ArrivalTimeCalculationAlgorithm::calculateArrivalTimes(int tripId, double averageSpeed) {
    // Проверка корректности скорости
    if (averageSpeed <= 0) {
        throw InputException("Средняя скорость должна быть положительной");
    }

    // Поиск рейса по ID
    const auto& trips = system->getTrips();
    auto tripIt = std::find_if(trips.begin(), trips.end(),
                               [tripId](const auto& t) { return t->getTripId() == tripId; });

    if (tripIt == trips.end()) {
        throw ContainerException("Рейс с ID " + std::to_string(tripId) + " не найден");
    }

    auto trip = *tripIt;
    const auto& stopsList = trip->getRoute()->getAllStops();

    if (stopsList.empty()) {
        throw ContainerException("Маршрут не содержит остановок");
    }

    // Время прибытия на первую остановку = время отправления
    Time currentTime = trip->getStartTime();
    trip->setArrivalTime(stopsList[0], currentTime);

    // Константы для расчета
    const double distanceBetweenStops = 1.5; // Расстояние между остановками в км
    const int stopTime = 1;                   // Время стоянки на остановке в минутах

    // Рассчитываем время прибытия для каждой последующей остановки
    for (size_t i = 1; i < stopsList.size(); ++i) {
        // Время в пути в минутах: (расстояние / скорость) * 60
        double travelTimeMinutes = (distanceBetweenStops / averageSpeed) * 60;
        // Время прибытия = текущее время + время в пути (округляем)
        Time arrivalTime = currentTime + static_cast<int>(travelTimeMinutes + 0.5);
        trip->setArrivalTime(stopsList[i], arrivalTime);
        // Время отправления со следующей остановки = время прибытия + время стоянки
        currentTime = arrivalTime + stopTime;
    }
}

// Поиск маршрутов между двумя остановками
// Ищет все маршруты, которые содержат обе остановки и stopA идет раньше stopB в маршруте
List<std::shared_ptr<Route>> RouteSearchAlgorithm::findRoutes(const std::string& stopA,
                                                                    const std::string& stopB) {
    List<std::shared_ptr<Route>> foundRoutes;
    const auto& routes = system->getRoutes();

    // Проверяем каждый маршрут
    for (const auto& route : routes) {
        // Маршрут должен содержать обе остановки и stopA должна быть раньше stopB
        if (route->containsStop(stopA) &&
            route->containsStop(stopB) &&
            route->isStopBefore(stopA, stopB)) {
            foundRoutes.push_back(route);
        }
    }
    return foundRoutes;
}