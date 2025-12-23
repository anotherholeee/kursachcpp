#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <memory>
#include <type_traits>
#include <iterator>
#include "list.h"
#include "journey.h"
#include "time.h"
#include "route.h"
#include "trip.h"

class TransportSystem;

// Базовый интерфейс для алгоритмов (для полиморфизма)
class IAlgorithm {
public:
    virtual ~IAlgorithm() = default;
    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;
};

// Шаблонный базовый класс для алгоритмов
// Может принимать либо лямбда-функцию, либо шаблонный параметр (тип)
template<typename T>
class Algorithm : public IAlgorithm {
protected:
    TransportSystem* system;
    T algorithmObject;  // Единственный объект алгоритма

public:
    // Итератор для Algorithm (singleton iterator - для одного элемента)
    class Iterator {
    private:
        T* ptr;  // Указатель на объект алгоритма
        bool visited;  // Флаг, указывающий, был ли элемент посещен

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* p, bool v = false) : ptr(p), visited(v) {}

        T& operator*() { return *ptr; }
        const T& operator*() const { return *ptr; }
        T* operator->() { return ptr; }
        const T* operator->() const { return ptr; }

        Iterator& operator++() {
            visited = true;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            visited = true;
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            // Итераторы равны, если оба указывают на nullptr или оба посетили элемент
            return (ptr == nullptr && other.ptr == nullptr) ||
                   (ptr == other.ptr && visited && other.visited);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

public:
    // Конструктор для лямбда-функций и функциональных объектов (копирование)
    explicit Algorithm(TransportSystem* sys, const T& obj) 
        : system(sys), algorithmObject(obj) {}
    
    // Конструктор для лямбда-функций и функциональных объектов (перемещение)
    explicit Algorithm(TransportSystem* sys, T&& obj) 
        : system(sys), algorithmObject(std::move(obj)) {}

    // Конструктор по умолчанию (для типов, которые можно создать без параметров)
    explicit Algorithm(TransportSystem* sys) 
        : system(sys), algorithmObject() {}

    // Конструктор с параметрами (для типов, требующих аргументов при создании)
    template<typename... Args>
    explicit Algorithm(TransportSystem* sys, Args&&... args) 
        : system(sys), algorithmObject(std::forward<Args>(args)...) {}

    virtual ~Algorithm() = default;

    // Виртуальный метод выполнения алгоритма
    void execute() override {
        // Вызываем алгоритм, если он функциональный объект
        if constexpr (std::is_invocable_v<T>) {
            algorithmObject();
        }
    }

    // Получение описания алгоритма
    std::string getDescription() const override {
        return "Шаблонный алгоритм";
    }

    // Установка системы (для работы с const методами)
    void setSystem(TransportSystem* sys) { system = sys; }

    // Метод для выполнения алгоритма с аргументами
    template<typename... Args>
    auto executeAlgorithm(Args&&... args) -> decltype(algorithmObject(std::forward<Args>(args)...)) {
        return algorithmObject(std::forward<Args>(args)...);
    }

    // Оператор вызова функции
    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(algorithmObject(std::forward<Args>(args)...)) {
        return algorithmObject(std::forward<Args>(args)...);
    }

    // Метод для получения объекта алгоритма
    T& getAlgorithm() {
        return algorithmObject;
    }

    const T& getAlgorithm() const {
        return algorithmObject;
    }

    // Методы для работы с итераторами
    Iterator begin() {
        return Iterator(&algorithmObject, false);
    }

    Iterator end() {
        return Iterator(&algorithmObject, true);
    }

    Iterator begin() const {
        return Iterator(const_cast<T*>(&algorithmObject), false);
    }

    Iterator end() const {
        return Iterator(const_cast<T*>(&algorithmObject), true);
    }
};

// Базовый класс для алгоритмов с TransportSystem (для обратной совместимости)
class BaseAlgorithm : public IAlgorithm {
protected:
    TransportSystem* system;

public:
    explicit BaseAlgorithm(TransportSystem* sys) : system(sys) {}
    virtual ~BaseAlgorithm() = default;

    void setSystem(TransportSystem* sys) { system = sys; }
};

// Класс алгоритма поиска пути (Strategy pattern)
class PathFindingAlgorithm : public BaseAlgorithm {
public:
    explicit PathFindingAlgorithm(TransportSystem* sys) : BaseAlgorithm(sys) {}

    virtual List<Journey> findPath(const std::string& start,
                                         const std::string& end,
                                         const Time& departureTime) = 0;

    void execute() override {}
    
    std::string getDescription() const override {
        return "Алгоритм поиска пути";
    }
};

// Алгоритм поиска в ширину (BFS) для поиска маршрутов с пересадками
class BFSAlgorithm : public PathFindingAlgorithm {
private:
    int maxTransfers;

public:
    BFSAlgorithm(TransportSystem* sys, int maxTransfers = 2)
        : PathFindingAlgorithm(sys), maxTransfers(maxTransfers) {}

    List<Journey> findPath(const std::string& start,
                                 const std::string& end,
                                 const Time& departureTime) override;

    void execute() override {
        // Реализация может быть добавлена при необходимости
    }

    std::string getDescription() const override {
        return "Алгоритм поиска в ширину (BFS) для поиска маршрутов с пересадками";
    }
};

// Алгоритм поиска самого быстрого маршрута
class FastestPathAlgorithm : public PathFindingAlgorithm {
public:
    explicit FastestPathAlgorithm(TransportSystem* sys) : PathFindingAlgorithm(sys) {}

    List<Journey> findPath(const std::string& start,
                                 const std::string& end,
                                 const Time& departureTime) override;

    void execute() override {}

    std::string getDescription() const override {
        return "Алгоритм поиска самого быстрого маршрута";
    }
};

// Алгоритм поиска маршрута с минимальными пересадками
class MinimalTransfersAlgorithm : public PathFindingAlgorithm {
public:
    explicit MinimalTransfersAlgorithm(TransportSystem* sys) : PathFindingAlgorithm(sys) {}

    List<Journey> findPath(const std::string& start,
                                 const std::string& end,
                                 const Time& departureTime) override;

    void execute() override {}

    std::string getDescription() const override {
        return "Алгоритм поиска маршрута с минимальными пересадками";
    }
};

// Алгоритм расчета времени прибытия
class ArrivalTimeCalculationAlgorithm : public BaseAlgorithm {
public:
    explicit ArrivalTimeCalculationAlgorithm(TransportSystem* sys) : BaseAlgorithm(sys) {}

    void calculateArrivalTimes(int tripId, double averageSpeed);

    void execute() override {
        // Реализация может быть добавлена при необходимости
    }

    std::string getDescription() const override {
        return "Алгоритм расчета времени прибытия на остановки";
    }
};

// Алгоритм поиска маршрутов между остановками
class RouteSearchAlgorithm : public BaseAlgorithm {
public:
    explicit RouteSearchAlgorithm(TransportSystem* sys) : BaseAlgorithm(sys) {}

    List<std::shared_ptr<Route>> findRoutes(const std::string& stopA,
                                                   const std::string& stopB);

    void execute() override {}

    std::string getDescription() const override {
        return "Алгоритм поиска маршрутов между остановками";
    }
};

// Шаблонный класс алгоритмов
// Требования:
// - Шаблонный класс
// - Принимает только 1 объект
// - Может принимать либо лямбда-функцию, либо шаблонный параметр (тип)
// 
// Примеры использования:
// 
// 1. С лямбда-функцией:
//    TemplateAlgorithm algo([](int x) { return x * 2; });
//    int result = algo.execute(5);  // result = 10
//    или
//    int result2 = algo(5);  // result2 = 10
//
// 2. С шаблонным параметром (типом):
//    struct MyAlgorithm {
//        int operator()(int x) const { return x * 3; }
//    };
//    TemplateAlgorithm<MyAlgorithm> algo;
//    int result = algo.execute(5);  // result = 15
//
// 3. С шаблонным параметром и аргументами конструктора:
//    struct AlgorithmWithParam {
//        int multiplier;
//        AlgorithmWithParam(int m) : multiplier(m) {}
//        int operator()(int x) const { return x * multiplier; }
//    };
//    TemplateAlgorithm<AlgorithmWithParam> algo(10);  // multiplier = 10
//    int result = algo.execute(5);  // result = 50
template<typename T>
class TemplateAlgorithm {
private:
    T algorithmObject;  // Единственный объект алгоритма

public:
    // Конструктор для лямбда-функций и функциональных объектов (копирование)
    explicit TemplateAlgorithm(const T& obj) : algorithmObject(obj) {}
    
    // Конструктор для лямбда-функций и функциональных объектов (перемещение)
    explicit TemplateAlgorithm(T&& obj) : algorithmObject(std::move(obj)) {}

    // Конструктор по умолчанию (для типов, которые можно создать без параметров)
    TemplateAlgorithm() : algorithmObject() {}

    // Конструктор с параметрами (для типов, требующих аргументов при создании)
    template<typename... Args>
    explicit TemplateAlgorithm(Args&&... args) 
        : algorithmObject(std::forward<Args>(args)...) {}

    // Метод для выполнения алгоритма
    // Работает как с лямбда-функциями, так и с функциональными объектами
    template<typename... Args>
    auto execute(Args&&... args) -> decltype(algorithmObject(std::forward<Args>(args)...)) {
        return algorithmObject(std::forward<Args>(args)...);
    }

    // Оператор вызова функции (альтернативный способ выполнения алгоритма)
    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(algorithmObject(std::forward<Args>(args)...)) {
        return algorithmObject(std::forward<Args>(args)...);
    }

    // Метод для получения объекта алгоритма
    T& getAlgorithm() {
        return algorithmObject;
    }

    const T& getAlgorithm() const {
        return algorithmObject;
    }
};

#endif // ALGORITHM_H