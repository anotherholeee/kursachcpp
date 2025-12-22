#include "algorithm.h"
#include "transport_system.h"
#include "exceptions.h"
#include <queue>
#include <algorithm>

List<Journey> BFSAlgorithm::findPath(const std::string& start,
                                           const std::string& end,
                                           const Time& departureTime) {
    List<Journey> journeys;

    struct SearchNode {
        std::string currentStop;
        Time currentTime;
        List<std::shared_ptr<Trip>> pathTrips;
        List<std::string> transferPoints;
        int transfers;
    };

    std::queue<SearchNode> q;
    q.push({start, departureTime, {}, {}, 0});

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node.currentStop == end) {
            journeys.push_back(Journey(node.pathTrips, node.transferPoints,
                                 departureTime, node.currentTime));
            continue;
        }

        if (node.transfers >= maxTransfers) {
            continue;
        }

        auto trips = system->getTripsThroughStop(node.currentStop);

        for (const auto& trip : trips) {
            // Проверяем, что время прибытия рассчитано для текущей остановки
            if (!trip->hasStop(node.currentStop)) {
                continue;
            }

            Time arrivalAtStop = trip->getArrivalTime(node.currentStop);

            if (arrivalAtStop < node.currentTime) {
                continue;
            }

            if (!node.pathTrips.empty() && node.pathTrips.back() == trip) {
                continue;
            }

            const auto& routeStops = trip->getRoute()->getAllStops();
            int currentPos = trip->getRoute()->getStopPosition(node.currentStop);

            if (currentPos == -1) continue;

            for (int i = currentPos + 1; i < routeStops.size(); ++i) {
                std::string nextStop = routeStops[i];

                // Проверяем, что время прибытия рассчитано для следующей остановки
                if (!trip->hasStop(nextStop)) {
                    continue;
                }

                Time arrivalAtNext = trip->getArrivalTime(nextStop);

                SearchNode nextNode = node;
                nextNode.currentStop = nextStop;
                nextNode.currentTime = arrivalAtNext;
                nextNode.pathTrips.push_back(trip);

                // Пересадка считается только если мы переходим на другой рейс
                if (!node.pathTrips.empty() && node.pathTrips.back() != trip) {
                    nextNode.transferPoints.push_back(node.currentStop);
                    nextNode.transfers++;
                }

                q.push(nextNode);
            }
        }
    }

    // Сортируем используя метод sort
    journeys.sort([](const Journey& a, const Journey& b) {
        return a.getTotalDuration() < b.getTotalDuration();
    });

    return journeys;
}

List<Journey> FastestPathAlgorithm::findPath(const std::string& start,
                                                   const std::string& end,
                                                   const Time& departureTime) {
    BFSAlgorithm bfs(system, 2);
    auto journeys = bfs.findPath(start, end, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    List<Journey> result;
    result.push_back(journeys[0]); // Возвращаем только самый быстрый
    return result;
}

List<Journey> MinimalTransfersAlgorithm::findPath(const std::string& start,
                                                         const std::string& end,
                                                         const Time& departureTime) {
    BFSAlgorithm bfs(system, 2);
    auto journeys = bfs.findPath(start, end, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    auto it = std::min_element(journeys.begin(), journeys.end(),
                               [](const Journey& a, const Journey& b) {
                                   return a.getTransferCount() < b.getTransferCount();
                               });

    List<Journey> result;
    result.push_back(*it);
    return result;
}

void ArrivalTimeCalculationAlgorithm::calculateArrivalTimes(int tripId, double averageSpeed) {
    if (averageSpeed <= 0) {
        throw InputException("Средняя скорость должна быть положительной");
    }

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

    Time currentTime = trip->getStartTime();
    trip->setArrivalTime(stopsList[0], currentTime);

    const double distanceBetweenStops = 1.5; // км
    const int stopTime = 1; // минута

    for (size_t i = 1; i < stopsList.size(); ++i) {
        double travelTimeMinutes = (distanceBetweenStops / averageSpeed) * 60;
        Time arrivalTime = currentTime + static_cast<int>(travelTimeMinutes + 0.5);
        trip->setArrivalTime(stopsList[i], arrivalTime);
        currentTime = arrivalTime + stopTime;
    }
}

List<std::shared_ptr<Route>> RouteSearchAlgorithm::findRoutes(const std::string& stopA,
                                                                    const std::string& stopB) {
    List<std::shared_ptr<Route>> foundRoutes;
    const auto& routes = system->getRoutes();

    for (const auto& route : routes) {
        if (route->containsStop(stopA) &&
            route->containsStop(stopB) &&
            route->isStopBefore(stopA, stopB)) {
            foundRoutes.push_back(route);
        }
    }
    return foundRoutes;
}