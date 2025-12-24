#include "journey_planner.h"
#include "transport_system.h"
#include <queue>
#include <algorithm>
#include <set>
#include "exceptions.h"

// Конструктор планировщика поездок
// Инициализирует планировщик и создает алгоритмы поиска маршрутов:
// BFS для поиска всех маршрутов с пересадками,
// FastestPath для поиска самого быстрого маршрута,
// MinimalTransfers для поиска маршрута с минимальными пересадками
JourneyPlanner::JourneyPlanner(TransportSystem* sys) 
    : system(sys),
      bfsAlgorithm(std::make_unique<BFSAlgorithm>(sys, 2)),
      fastestAlgorithm(std::make_unique<FastestPathAlgorithm>(sys)),
      minimalTransfersAlgorithm(std::make_unique<MinimalTransfersAlgorithm>(sys)) {}

// Поиск маршрутов с пересадками с заданным временем отправления
// Использует алгоритм BFS для поиска всех возможных маршрутов
// с учетом ограничения на количество пересадок
List<Journey> JourneyPlanner::findJourneysWithTransfers(
    const std::string& startStop,
    const std::string& endStop,
    const Time& departureTime,
    int maxTransfers) const {

    // Используем алгоритм BFS (создаем временный объект для const метода)
    BFSAlgorithm bfs(const_cast<TransportSystem*>(system), maxTransfers);
    return bfs.findPath(startStop, endStop, departureTime);
}

// Поиск всех возможных маршрутов между остановками (без привязки ко времени)
// Находит все возможные маршруты между остановками, начиная с любого рейса,
// проходящего через начальную остановку. Использует BFS с ограничением итераций
// для предотвращения зависания
List<Journey> JourneyPlanner::findAllJourneysWithTransfers(
    const std::string& startStop,
    const std::string& endStop,
    int maxTransfers) const {

    List<Journey> journeys;

    // Узел поиска: содержит текущую остановку, время, пройденный путь и пересадки
    struct SearchNode {
        std::string currentStop;              // Текущая остановка
        Time currentTime;                     // Текущее время
        Time startTime;                       // Время начала поездки
        List<std::shared_ptr<Trip>> pathTrips; // Рейсы, использованные в пути
        List<std::string> transferPoints;     // Остановки, где были пересадки
        int transfers;                        // Количество пересадок
    };

    // Получаем все рейсы, проходящие через начальную остановку
    auto initialTrips = system->getTripsThroughStop(startStop);

    // Фильтруем только рейсы с рассчитанным временем прибытия
    List<std::shared_ptr<Trip>> validInitialTrips;
    for (const auto& trip : initialTrips) {
        if (trip->hasStop(startStop)) {
            validInitialTrips.push_back(trip);
        }
    }

    // Сортируем рейсы по времени прибытия на начальную остановку
    validInitialTrips.sort([&startStop](const auto& a, const auto& b) {
        return a->getArrivalTime(startStop) < b->getArrivalTime(startStop);
    });

    std::queue<SearchNode> q;
    // Используем set для отслеживания уже посещенных комбинаций (остановка + количество пересадок)
    // чтобы избежать бесконечных циклов
    std::set<std::pair<std::string, int>> visited;
    // Ограничение на количество итераций для предотвращения зависания
    const int MAX_ITERATIONS = 10000;
    int iterations = 0;

    // Добавляем в очередь начальные узлы для каждого рейса через начальную остановку
    for (const auto& trip : validInitialTrips) {
        Time arrivalAtStart = trip->getArrivalTime(startStop);
        q.push({startStop, arrivalAtStart, arrivalAtStart, {}, {}, 0});
    }

    // Основной цикл поиска в ширину
    while (!q.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        auto node = q.front();
        q.pop();

        // Если достигли конечной остановки - сохраняем найденный маршрут
        if (node.currentStop == endStop) {
            journeys.push_back(Journey(node.pathTrips, node.transferPoints,
                                 node.startTime, node.currentTime));
            continue;
        }

        // Пропускаем узлы с превышением лимита пересадок
        if (node.transfers >= maxTransfers) {
            continue;
        }

        // Проверяем, не посещали ли мы уже эту остановку с таким же количеством пересадок
        // Это предотвращает бесконечные циклы
        auto visitKey = std::make_pair(node.currentStop, node.transfers);
        if (visited.find(visitKey) != visited.end()) {
            continue;
        }
        visited.insert(visitKey);

        // Получаем все рейсы, проходящие через текущую остановку
        auto trips = system->getTripsThroughStop(node.currentStop);

        for (const auto& trip : trips) {
            // Проверяем, что время прибытия рассчитано для текущей остановки
            if (!trip->hasStop(node.currentStop)) {
                continue;
            }

            Time arrivalAtStop = trip->getArrivalTime(node.currentStop);

            // Для промежуточных остановок пропускаем рейсы, которые уже прошли
            if (node.currentStop != startStop && arrivalAtStop < node.currentTime) {
                continue;
            }

            // Проверяем, что мы не используем тот же рейс дважды подряд
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

                // Для первого рейса устанавливаем время начала поездки
                if (nextNode.pathTrips.size() == 1) {
                    nextNode.startTime = arrivalAtStop;
                }

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

    // Сортируем найденные маршруты: сначала по времени отправления, затем по длительности
    journeys.sort([](const Journey& a, const Journey& b) {
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