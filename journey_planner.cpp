#include "journey_planner.h"
#include "transport_system.h"
#include <queue>
#include <algorithm>
#include <set>
#include "exceptions.h"

JourneyPlanner::JourneyPlanner(TransportSystem* sys) 
    : system(sys),
      bfsAlgorithm(std::make_unique<BFSAlgorithm>(sys, 2)),
      fastestAlgorithm(std::make_unique<FastestPathAlgorithm>(sys)),
      minimalTransfersAlgorithm(std::make_unique<MinimalTransfersAlgorithm>(sys)) {}

std::vector<Journey> JourneyPlanner::findJourneysWithTransfers(
    const std::string& startStop,
    const std::string& endStop,
    const Time& departureTime,
    int maxTransfers) const {
    
    // Используем алгоритм BFS (создаем временный объект для const метода)
    BFSAlgorithm bfs(const_cast<TransportSystem*>(system), maxTransfers);
    return bfs.findPath(startStop, endStop, departureTime);
}

std::vector<Journey> JourneyPlanner::findAllJourneysWithTransfers(
    const std::string& startStop,
    const std::string& endStop,
    int maxTransfers) const {

    std::vector<Journey> journeys;

    struct SearchNode {
        std::string currentStop;
        Time currentTime;
        Time startTime;
        std::vector<std::shared_ptr<Trip>> pathTrips;
        std::vector<std::string> transferPoints;
        int transfers;
    };

    auto initialTrips = system->getTripsThroughStop(startStop);

    // Фильтруем только рейсы с рассчитанным временем прибытия
    std::vector<std::shared_ptr<Trip>> validInitialTrips;
    for (const auto& trip : initialTrips) {
        if (trip->hasStop(startStop)) {
            validInitialTrips.push_back(trip);
        }
    }

    std::sort(validInitialTrips.begin(), validInitialTrips.end(),
              [&startStop](const auto& a, const auto& b) {
                  return a->getArrivalTime(startStop) < b->getArrivalTime(startStop);
              });

    std::queue<SearchNode> q;
    // Используем set для отслеживания уже посещенных комбинаций (остановка + количество пересадок)
    // чтобы избежать бесконечных циклов
    std::set<std::pair<std::string, int>> visited;
    // Ограничение на количество итераций для предотвращения зависания
    const int MAX_ITERATIONS = 10000;
    int iterations = 0;

    for (const auto& trip : validInitialTrips) {
        Time arrivalAtStart = trip->getArrivalTime(startStop);
        q.push({startStop, arrivalAtStart, arrivalAtStart, {}, {}, 0});
    }

    while (!q.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        auto node = q.front();
        q.pop();

        if (node.currentStop == endStop) {
            journeys.emplace_back(node.pathTrips, node.transferPoints,
                                 node.startTime, node.currentTime);
            continue;
        }

        if (node.transfers >= maxTransfers) {
            continue;
        }

        // Проверяем, не посещали ли мы уже эту остановку с таким же количеством пересадок
        auto visitKey = std::make_pair(node.currentStop, node.transfers);
        if (visited.find(visitKey) != visited.end()) {
            continue;
        }
        visited.insert(visitKey);

        auto trips = system->getTripsThroughStop(node.currentStop);

        for (const auto& trip : trips) {
            // Проверяем, что время прибытия рассчитано для текущей остановки
            if (!trip->hasStop(node.currentStop)) {
                continue;
            }

            Time arrivalAtStop = trip->getArrivalTime(node.currentStop);

            if (node.currentStop != startStop && arrivalAtStop < node.currentTime) {
                continue;
            }

            // Проверяем, что мы не используем тот же рейс дважды подряд
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

                if (nextNode.pathTrips.size() == 1) {
                    nextNode.startTime = arrivalAtStop;
                }

                // Пересадка считается только если мы переходим на другой рейс
                if (!node.pathTrips.empty() && node.pathTrips.back() != trip) {
                    nextNode.transferPoints.push_back(node.currentStop);
                    nextNode.transfers++;
                }

                q.push(nextNode);
            }
        }
    }

    std::sort(journeys.begin(), journeys.end(),
              [](const Journey& a, const Journey& b) {
                  if (a.getStartTime() != b.getStartTime()) {
                      return a.getStartTime() < b.getStartTime();
                  }
                  return a.getTotalDuration() < b.getTotalDuration();
              });

    return journeys;
}

Journey JourneyPlanner::findFastestJourney(const std::string& startStop,
                                          const std::string& endStop,
                                          const Time& departureTime) {
    auto journeys = fastestAlgorithm->findPath(startStop, endStop, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    return journeys[0];
}

Journey JourneyPlanner::findJourneyWithLeastTransfers(const std::string& startStop,
                                                     const std::string& endStop,
                                                     const Time& departureTime) {
    auto journeys = minimalTransfersAlgorithm->findPath(startStop, endStop, departureTime);

    if (journeys.empty()) {
        throw ContainerException("Маршрут не найден");
    }

    return journeys[0];
}

void JourneyPlanner::displayJourney(const Journey& journey) const {
    journey.display();
}

