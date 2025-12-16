#include "qt_ui.h"
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
#include <QApplication>
#include <iostream>

// Функция инициализации тестовых данных
void initializeTestData(TransportSystem& system) {
    system.addStopDirect(Stop(1, "Центральный вокзал"));
    system.addStopDirect(Stop(2, "Площадь Ленина"));
    system.addStopDirect(Stop(3, "Улица Гагарина"));
    system.addStopDirect(Stop(4, "Парк Победы"));
    system.addStopDirect(Stop(5, "Стадион"));
    system.addStopDirect(Stop(6, "Больница"));
    system.addStopDirect(Stop(7, "Университет"));

    auto bus1 = std::make_shared<Bus>("МАЗ-203", "АН 8669-7");
    auto bus2 = std::make_shared<Bus>("ПАЗ-3205", "ВС 1234-5");
    auto tram1 = std::make_shared<Tram>("71-931", "ТР 5678-9");

    system.addVehicleDirect(bus1);
    system.addVehicleDirect(bus2);
    system.addVehicleDirect(tram1);

    auto driver1 = std::make_shared<Driver>("Иван", "Петров", "Сергеевич");
    auto driver2 = std::make_shared<Driver>("Мария", "Сидорова", "Ивановна");
    auto driver3 = std::make_shared<Driver>("Алексей", "Козлов");

    system.addDriverDirect(driver1);
    system.addDriverDirect(driver2);
    system.addDriverDirect(driver3);

    std::vector<std::string> route1Stops = {"Центральный вокзал", "Площадь Ленина", "Улица Гагарина", "Стадион"};
    auto route1 = std::make_shared<Route>(101, "Автобус", route1Stops);

    std::vector<std::string> route2Stops = {"Центральный вокзал", "Площадь Ленина", "Больница", "Университет"};
    auto route2 = std::make_shared<Route>(202, "Автобус", route2Stops);

    std::vector<std::string> route3Stops = {"Парк Победы", "Улица Гагарина", "Больница", "Университет"};
    auto route3 = std::make_shared<Route>(5, "Трамвай", route3Stops);

    system.addRouteDirect(route1);
    system.addRouteDirect(route2);
    system.addRouteDirect(route3);

    try {
        auto trip1 = std::make_shared<Trip>(1, route1, bus1, driver1, Time("08:00"), 1);
        auto trip2 = std::make_shared<Trip>(2, route2, bus2, driver2, Time("09:00"), 1);
        auto trip3 = std::make_shared<Trip>(3, route3, tram1, driver3, Time("10:00"), 1);
        
        auto trip4 = std::make_shared<Trip>(4, route1, bus1, driver1, Time("09:00"), 6);
        auto trip5 = std::make_shared<Trip>(5, route2, bus2, driver2, Time("10:00"), 6);
        auto trip6 = std::make_shared<Trip>(6, route1, bus1, driver1, Time("10:00"), 7);

        system.addTripDirect(trip1);
        system.addTripDirect(trip2);
        system.addTripDirect(trip3);
        system.addTripDirect(trip4);
        system.addTripDirect(trip5);
        system.addTripDirect(trip6);

        system.calculateArrivalTimes(1, 30.0);
        system.calculateArrivalTimes(2, 30.0);
        system.calculateArrivalTimes(3, 25.0);
        system.calculateArrivalTimes(4, 30.0);
        system.calculateArrivalTimes(5, 30.0);
        system.calculateArrivalTimes(6, 30.0);

    } catch (const std::exception& e) {
        std::cout << "Ошибка при создании тестовых рейсов: " << e.what() << "\n";
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        TransportSystem system;
        
        // Загружаем данные из файлов
        std::cout << "[INFO] Загрузка данных из файлов..." << std::endl;
        system.loadData();
        
        // Проверяем, есть ли данные вообще (только если ВСЕ категории пустые)
        // Это означает, что файлы не существуют или пустые
        bool needsInit = system.getStops().empty() && 
                        system.getRoutes().empty() && 
                        system.getVehicles().empty() && 
                        system.getDrivers().empty() &&
                        system.getTrips().empty();
        
        // Если нет данных вообще, инициализируем тестовые данные
        if (needsInit) {
            std::cout << "[INFO] Файлы данных пусты или не существуют. Инициализация тестовых данных..." << std::endl;
            initializeTestData(system);
            // Сохраняем инициализированные данные в файлы
            system.saveData();
            std::cout << "[INFO] Тестовые данные инициализированы и сохранены в файлы." << std::endl;
        } else {
            std::cout << "[INFO] Данные успешно загружены из файлов." << std::endl;
            std::cout << "[INFO] Статистика: " 
                      << system.getStops().size() << " остановок, "
                      << system.getRoutes().size() << " маршрутов, "
                      << system.getTrips().size() << " рейсов, "
                      << system.getVehicles().size() << " транспорта, "
                      << system.getDrivers().size() << " водителей" << std::endl;
        }

        MainWindow window(&system);
        window.show();

        return app.exec();
    } catch (const InputException& e) {
        std::cerr << "Критическая ошибка ввода: " << e.what() << std::endl;
        try {
            TransportSystem tempSystem;
            tempSystem.saveData();
        } catch (...) {
        }
        return 1;
    } catch (const FileException& e) {
        std::cerr << "Критическая ошибка файла: " << e.what() << std::endl;
        return 1;
    } catch (const ContainerException& e) {
        std::cerr << "Критическая ошибка данных: " << e.what() << std::endl;
        return 1;
    } catch (const TransportException& e) {
        std::cerr << "Критическая ошибка системы: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
}

