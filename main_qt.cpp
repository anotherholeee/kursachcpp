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
    // Остановки (20 остановок)
    system.addStopDirect(Stop(1, "Центральный вокзал"));
    system.addStopDirect(Stop(2, "Площадь Ленина"));
    system.addStopDirect(Stop(3, "Улица Гагарина"));
    system.addStopDirect(Stop(4, "Парк Победы"));
    system.addStopDirect(Stop(5, "Стадион"));
    system.addStopDirect(Stop(6, "Больница"));
    system.addStopDirect(Stop(7, "Университет"));
    system.addStopDirect(Stop(8, "Торговый центр"));
    system.addStopDirect(Stop(9, "Железнодорожный вокзал"));
    system.addStopDirect(Stop(10, "Аэропорт"));
    system.addStopDirect(Stop(11, "Школа №1"));
    system.addStopDirect(Stop(12, "Поликлиника"));
    system.addStopDirect(Stop(13, "Центральный рынок"));
    system.addStopDirect(Stop(14, "Спортивный комплекс"));
    system.addStopDirect(Stop(15, "Театр"));
    system.addStopDirect(Stop(16, "Библиотека"));
    system.addStopDirect(Stop(17, "Парк культуры"));
    system.addStopDirect(Stop(18, "Автовокзал"));
    system.addStopDirect(Stop(19, "Завод"));
    system.addStopDirect(Stop(20, "Микрорайон Северный"));

    // Транспортные средства (15 единиц)
    auto bus1 = std::make_shared<Bus>("МАЗ-203", "АН 8669-7");
    auto bus2 = std::make_shared<Bus>("ПАЗ-3205", "ВС 1234-5");
    auto bus3 = std::make_shared<Bus>("ЛиАЗ-5292", "АВ 2222-2");
    auto bus4 = std::make_shared<Bus>("МАЗ-206", "АН 7777-7");
    auto bus5 = std::make_shared<Bus>("ПАЗ-4234", "ВС 8888-8");
    auto bus6 = std::make_shared<Bus>("ЛиАЗ-5256", "АВ 9999-9");
    
    auto tram1 = std::make_shared<Tram>("71-931", "ТР 5678-9");
    auto tram2 = std::make_shared<Tram>("71-623", "ТР 3333-3");
    auto tram3 = std::make_shared<Tram>("71-407", "ТР 4444-4");
    auto tram4 = std::make_shared<Tram>("71-931М", "ТР 5555-5");
    
    auto trolleybus1 = std::make_shared<Trolleybus>("ЗиУ-682", "ТБ 1111-1");
    auto trolleybus2 = std::make_shared<Trolleybus>("Тролза-5265", "ТБ 2222-2");
    auto trolleybus3 = std::make_shared<Trolleybus>("ЗиУ-683", "ТБ 3333-3");
    auto trolleybus4 = std::make_shared<Trolleybus>("Тролза-5275", "ТБ 4444-4");
    auto trolleybus5 = std::make_shared<Trolleybus>("ЗиУ-6205", "ТБ 5555-5");

    system.addVehicleDirect(bus1);
    system.addVehicleDirect(bus2);
    system.addVehicleDirect(bus3);
    system.addVehicleDirect(bus4);
    system.addVehicleDirect(bus5);
    system.addVehicleDirect(bus6);
    system.addVehicleDirect(tram1);
    system.addVehicleDirect(tram2);
    system.addVehicleDirect(tram3);
    system.addVehicleDirect(tram4);
    system.addVehicleDirect(trolleybus1);
    system.addVehicleDirect(trolleybus2);
    system.addVehicleDirect(trolleybus3);
    system.addVehicleDirect(trolleybus4);
    system.addVehicleDirect(trolleybus5);

    // Водители (15 человек)
    // Категории: D - автобусы, T - трамваи/троллейбусы, DT - универсальная
    auto driver1 = std::make_shared<Driver>("Иван", "Петров", "Сергеевич", "D");
    auto driver2 = std::make_shared<Driver>("Мария", "Сидорова", "Ивановна", "D");
    auto driver3 = std::make_shared<Driver>("Алексей", "Козлов", "Викторович", "DT");
    auto driver4 = std::make_shared<Driver>("Дмитрий", "Смирнов", "Владимирович", "D");
    auto driver5 = std::make_shared<Driver>("Елена", "Волкова", "Петровна", "D");
    auto driver6 = std::make_shared<Driver>("Сергей", "Новиков", "Александрович", "T");
    auto driver7 = std::make_shared<Driver>("Анна", "Морозова", "Дмитриевна", "T");
    auto driver8 = std::make_shared<Driver>("Владимир", "Лебедев", "Олегович", "T");
    auto driver9 = std::make_shared<Driver>("Ольга", "Соколова", "Андреевна", "T");
    auto driver10 = std::make_shared<Driver>("Николай", "Попов", "Игоревич", "T");
    auto driver11 = std::make_shared<Driver>("Татьяна", "Васильева", "Николаевна", "DT");
    auto driver12 = std::make_shared<Driver>("Андрей", "Федоров", "Сергеевич", "T");
    auto driver13 = std::make_shared<Driver>("Наталья", "Михайлова", "Владимировна", "DT");
    auto driver14 = std::make_shared<Driver>("Павел", "Кузнецов", "Борисович", "T");
    auto driver15 = std::make_shared<Driver>("Юлия", "Орлова", "Александровна", "DT");

    system.addDriverDirect(driver1);
    system.addDriverDirect(driver2);
    system.addDriverDirect(driver3);
    system.addDriverDirect(driver4);
    system.addDriverDirect(driver5);
    system.addDriverDirect(driver6);
    system.addDriverDirect(driver7);
    system.addDriverDirect(driver8);
    system.addDriverDirect(driver9);
    system.addDriverDirect(driver10);
    system.addDriverDirect(driver11);
    system.addDriverDirect(driver12);
    system.addDriverDirect(driver13);
    system.addDriverDirect(driver14);
    system.addDriverDirect(driver15);

    // Маршруты (15 маршрутов)
    std::vector<std::string> route1Stops = {"Центральный вокзал", "Площадь Ленина", "Улица Гагарина", "Стадион"};
    auto route1 = std::make_shared<Route>(101, "Автобус", route1Stops);

    std::vector<std::string> route2Stops = {"Центральный вокзал", "Площадь Ленина", "Больница", "Университет"};
    auto route2 = std::make_shared<Route>(202, "Автобус", route2Stops);

    std::vector<std::string> route3Stops = {"Парк Победы", "Улица Гагарина", "Больница", "Университет"};
    auto route3 = std::make_shared<Route>(5, "Трамвай", route3Stops);

    std::vector<std::string> route4Stops = {"Стадион", "Улица Гагарина", "Парк Победы"};
    auto route4 = std::make_shared<Route>(303, "Троллейбус", route4Stops);

    std::vector<std::string> route5Stops = {"Университет", "Больница", "Площадь Ленина", "Центральный вокзал"};
    auto route5 = std::make_shared<Route>(404, "Автобус", route5Stops);

    std::vector<std::string> route6Stops = {"Парк Победы", "Улица Гагарина", "Площадь Ленина"};
    auto route6 = std::make_shared<Route>(7, "Трамвай", route6Stops);

    std::vector<std::string> route7Stops = {"Железнодорожный вокзал", "Центральный вокзал", "Торговый центр", "Поликлиника"};
    auto route7 = std::make_shared<Route>(105, "Автобус", route7Stops);

    std::vector<std::string> route8Stops = {"Аэропорт", "Железнодорожный вокзал", "Центральный вокзал", "Площадь Ленина"};
    auto route8 = std::make_shared<Route>(206, "Автобус", route8Stops);

    std::vector<std::string> route9Stops = {"Университет", "Школа №1", "Поликлиника", "Больница"};
    auto route9 = std::make_shared<Route>(8, "Трамвай", route9Stops);

    std::vector<std::string> route10Stops = {"Центральный рынок", "Площадь Ленина", "Театр", "Библиотека"};
    auto route10 = std::make_shared<Route>(304, "Троллейбус", route10Stops);

    std::vector<std::string> route11Stops = {"Спортивный комплекс", "Стадион", "Парк культуры", "Парк Победы"};
    auto route11 = std::make_shared<Route>(9, "Трамвай", route11Stops);

    std::vector<std::string> route12Stops = {"Автовокзал", "Центральный вокзал", "Площадь Ленина", "Торговый центр"};
    auto route12 = std::make_shared<Route>(107, "Автобус", route12Stops);

    std::vector<std::string> route13Stops = {"Завод", "Микрорайон Северный", "Университет", "Больница"};
    auto route13 = std::make_shared<Route>(305, "Троллейбус", route13Stops);

    std::vector<std::string> route14Stops = {"Парк культуры", "Библиотека", "Театр", "Центральный рынок"};
    auto route14 = std::make_shared<Route>(306, "Троллейбус", route14Stops);

    std::vector<std::string> route15Stops = {"Аэропорт", "Железнодорожный вокзал", "Автовокзал", "Центральный вокзал"};
    auto route15 = std::make_shared<Route>(108, "Автобус", route15Stops);

    system.addRouteDirect(route1);
    system.addRouteDirect(route2);
    system.addRouteDirect(route3);
    system.addRouteDirect(route4);
    system.addRouteDirect(route5);
    system.addRouteDirect(route6);
    system.addRouteDirect(route7);
    system.addRouteDirect(route8);
    system.addRouteDirect(route9);
    system.addRouteDirect(route10);
    system.addRouteDirect(route11);
    system.addRouteDirect(route12);
    system.addRouteDirect(route13);
    system.addRouteDirect(route14);
    system.addRouteDirect(route15);

    try {
        // Рейсы для будних дней (понедельник-пятница)
        // Маршрут 101
        auto trip1 = std::make_shared<Trip>(1, route1, bus1, driver1, Time("06:30"), 1);
        auto trip2 = std::make_shared<Trip>(2, route1, bus1, driver1, Time("08:00"), 1);
        auto trip3 = std::make_shared<Trip>(3, route1, bus2, driver2, Time("09:30"), 1);
        auto trip4 = std::make_shared<Trip>(4, route1, bus1, driver1, Time("11:00"), 1);
        auto trip5 = std::make_shared<Trip>(5, route1, bus3, driver3, Time("13:00"), 1);
        auto trip6 = std::make_shared<Trip>(6, route1, bus2, driver2, Time("15:00"), 1);
        auto trip7 = std::make_shared<Trip>(7, route1, bus1, driver1, Time("17:00"), 1);
        auto trip8 = std::make_shared<Trip>(8, route1, bus3, driver3, Time("19:00"), 1);
        
        // Маршрут 202
        auto trip9 = std::make_shared<Trip>(9, route2, bus2, driver2, Time("07:00"), 1);
        auto trip10 = std::make_shared<Trip>(10, route2, bus4, driver4, Time("08:30"), 1);
        auto trip11 = std::make_shared<Trip>(11, route2, bus2, driver2, Time("10:00"), 1);
        auto trip12 = std::make_shared<Trip>(12, route2, bus5, driver5, Time("12:00"), 1);
        auto trip13 = std::make_shared<Trip>(13, route2, bus4, driver4, Time("14:00"), 1);
        auto trip14 = std::make_shared<Trip>(14, route2, bus2, driver2, Time("16:00"), 1);
        auto trip15 = std::make_shared<Trip>(15, route2, bus5, driver5, Time("18:00"), 1);
        
        // Маршрут 5 (Трамвай)
        auto trip16 = std::make_shared<Trip>(16, route3, tram1, driver6, Time("06:00"), 1);
        auto trip17 = std::make_shared<Trip>(17, route3, tram2, driver7, Time("07:30"), 1);
        auto trip18 = std::make_shared<Trip>(18, route3, tram1, driver6, Time("09:00"), 1);
        auto trip19 = std::make_shared<Trip>(19, route3, tram3, driver8, Time("11:30"), 1);
        auto trip20 = std::make_shared<Trip>(20, route3, tram2, driver7, Time("13:30"), 1);
        auto trip21 = std::make_shared<Trip>(21, route3, tram1, driver6, Time("15:30"), 1);
        auto trip22 = std::make_shared<Trip>(22, route3, tram3, driver8, Time("17:30"), 1);
        auto trip23 = std::make_shared<Trip>(23, route3, tram4, driver9, Time("19:30"), 1);
        
        // Маршрут 303 (Троллейбус)
        auto trip24 = std::make_shared<Trip>(24, route4, trolleybus1, driver10, Time("07:15"), 1);
        auto trip25 = std::make_shared<Trip>(25, route4, trolleybus2, driver11, Time("09:15"), 1);
        auto trip26 = std::make_shared<Trip>(26, route4, trolleybus1, driver10, Time("11:15"), 1);
        auto trip27 = std::make_shared<Trip>(27, route4, trolleybus3, driver12, Time("13:15"), 1);
        auto trip28 = std::make_shared<Trip>(28, route4, trolleybus2, driver11, Time("15:15"), 1);
        auto trip29 = std::make_shared<Trip>(29, route4, trolleybus1, driver10, Time("17:15"), 1);
        
        // Маршрут 404
        auto trip30 = std::make_shared<Trip>(30, route5, bus3, driver13, Time("08:15"), 1);
        auto trip31 = std::make_shared<Trip>(31, route5, bus6, driver14, Time("10:15"), 1);
        auto trip32 = std::make_shared<Trip>(32, route5, bus3, driver13, Time("12:15"), 1);
        auto trip33 = std::make_shared<Trip>(33, route5, bus6, driver14, Time("14:15"), 1);
        auto trip34 = std::make_shared<Trip>(34, route5, bus3, driver13, Time("16:15"), 1);
        auto trip35 = std::make_shared<Trip>(35, route5, bus6, driver14, Time("18:15"), 1);
        
        // Маршрут 7 (Трамвай)
        auto trip36 = std::make_shared<Trip>(36, route6, tram2, driver15, Time("08:45"), 1);
        auto trip37 = std::make_shared<Trip>(37, route6, tram4, driver7, Time("10:45"), 1);
        auto trip38 = std::make_shared<Trip>(38, route6, tram2, driver15, Time("12:45"), 1);
        auto trip39 = std::make_shared<Trip>(39, route6, tram4, driver7, Time("14:45"), 1);
        auto trip40 = std::make_shared<Trip>(40, route6, tram2, driver15, Time("16:45"), 1);
        
        // Дополнительные маршруты
        auto trip41 = std::make_shared<Trip>(41, route7, bus4, driver1, Time("07:20"), 1);
        auto trip42 = std::make_shared<Trip>(42, route7, bus5, driver2, Time("09:20"), 1);
        auto trip43 = std::make_shared<Trip>(43, route7, bus4, driver1, Time("11:20"), 1);
        auto trip44 = std::make_shared<Trip>(44, route7, bus5, driver2, Time("13:20"), 1);
        auto trip45 = std::make_shared<Trip>(45, route7, bus4, driver1, Time("15:20"), 1);
        
        auto trip46 = std::make_shared<Trip>(46, route8, bus6, driver3, Time("06:45"), 1);
        auto trip47 = std::make_shared<Trip>(47, route8, bus1, driver4, Time("08:45"), 1);
        auto trip48 = std::make_shared<Trip>(48, route8, bus6, driver3, Time("10:45"), 1);
        auto trip49 = std::make_shared<Trip>(49, route8, bus1, driver4, Time("12:45"), 1);
        auto trip50 = std::make_shared<Trip>(50, route8, bus6, driver3, Time("14:45"), 1);
        
        auto trip51 = std::make_shared<Trip>(51, route9, tram1, driver5, Time("07:10"), 1);
        auto trip52 = std::make_shared<Trip>(52, route9, tram3, driver6, Time("09:10"), 1);
        auto trip53 = std::make_shared<Trip>(53, route9, tram1, driver5, Time("11:10"), 1);
        auto trip54 = std::make_shared<Trip>(54, route9, tram3, driver6, Time("13:10"), 1);
        auto trip55 = std::make_shared<Trip>(55, route9, tram1, driver5, Time("15:10"), 1);
        
        auto trip56 = std::make_shared<Trip>(56, route10, trolleybus2, driver7, Time("08:00"), 1);
        auto trip57 = std::make_shared<Trip>(57, route10, trolleybus4, driver8, Time("10:00"), 1);
        auto trip58 = std::make_shared<Trip>(58, route10, trolleybus2, driver7, Time("12:00"), 1);
        auto trip59 = std::make_shared<Trip>(59, route10, trolleybus4, driver8, Time("14:00"), 1);
        auto trip60 = std::make_shared<Trip>(60, route10, trolleybus2, driver7, Time("16:00"), 1);
        
        auto trip61 = std::make_shared<Trip>(61, route11, tram2, driver9, Time("08:30"), 1);
        auto trip62 = std::make_shared<Trip>(62, route11, tram4, driver10, Time("10:30"), 1);
        auto trip63 = std::make_shared<Trip>(63, route11, tram2, driver9, Time("12:30"), 1);
        auto trip64 = std::make_shared<Trip>(64, route11, tram4, driver10, Time("14:30"), 1);
        
        auto trip65 = std::make_shared<Trip>(65, route12, bus2, driver11, Time("07:40"), 1);
        auto trip66 = std::make_shared<Trip>(66, route12, bus3, driver12, Time("09:40"), 1);
        auto trip67 = std::make_shared<Trip>(67, route12, bus2, driver11, Time("11:40"), 1);
        auto trip68 = std::make_shared<Trip>(68, route12, bus3, driver12, Time("13:40"), 1);
        auto trip69 = std::make_shared<Trip>(69, route12, bus2, driver11, Time("15:40"), 1);
        
        auto trip70 = std::make_shared<Trip>(70, route13, trolleybus3, driver13, Time("08:20"), 1);
        auto trip71 = std::make_shared<Trip>(71, route13, trolleybus5, driver14, Time("10:20"), 1);
        auto trip72 = std::make_shared<Trip>(72, route13, trolleybus3, driver13, Time("12:20"), 1);
        auto trip73 = std::make_shared<Trip>(73, route13, trolleybus5, driver14, Time("14:20"), 1);
        
        auto trip74 = std::make_shared<Trip>(74, route14, trolleybus1, driver15, Time("09:00"), 1);
        auto trip75 = std::make_shared<Trip>(75, route14, trolleybus4, driver1, Time("11:00"), 1);
        auto trip76 = std::make_shared<Trip>(76, route14, trolleybus1, driver15, Time("13:00"), 1);
        auto trip77 = std::make_shared<Trip>(77, route14, trolleybus4, driver1, Time("15:00"), 1);
        
        auto trip78 = std::make_shared<Trip>(78, route15, bus5, driver2, Time("07:50"), 1);
        auto trip79 = std::make_shared<Trip>(79, route15, bus6, driver3, Time("09:50"), 1);
        auto trip80 = std::make_shared<Trip>(80, route15, bus5, driver2, Time("11:50"), 1);
        auto trip81 = std::make_shared<Trip>(81, route15, bus6, driver3, Time("13:50"), 1);
        auto trip82 = std::make_shared<Trip>(82, route15, bus5, driver2, Time("15:50"), 1);
        
        // Рейсы для вторника (день 2)
        auto trip83 = std::make_shared<Trip>(83, route1, bus1, driver1, Time("06:30"), 2);
        auto trip84 = std::make_shared<Trip>(84, route1, bus2, driver2, Time("08:00"), 2);
        auto trip85 = std::make_shared<Trip>(85, route1, bus3, driver3, Time("09:30"), 2);
        auto trip86 = std::make_shared<Trip>(86, route1, bus1, driver1, Time("11:00"), 2);
        auto trip87 = std::make_shared<Trip>(87, route1, bus2, driver2, Time("13:00"), 2);
        auto trip88 = std::make_shared<Trip>(88, route1, bus3, driver3, Time("15:00"), 2);
        auto trip89 = std::make_shared<Trip>(89, route1, bus1, driver1, Time("17:00"), 2);
        auto trip90 = std::make_shared<Trip>(90, route1, bus2, driver2, Time("19:00"), 2);
        
        auto trip91 = std::make_shared<Trip>(91, route2, bus2, driver2, Time("07:00"), 2);
        auto trip92 = std::make_shared<Trip>(92, route2, bus4, driver4, Time("08:30"), 2);
        auto trip93 = std::make_shared<Trip>(93, route2, bus5, driver5, Time("10:00"), 2);
        auto trip94 = std::make_shared<Trip>(94, route2, bus2, driver2, Time("12:00"), 2);
        auto trip95 = std::make_shared<Trip>(95, route2, bus4, driver4, Time("14:00"), 2);
        auto trip96 = std::make_shared<Trip>(96, route2, bus5, driver5, Time("16:00"), 2);
        auto trip97 = std::make_shared<Trip>(97, route2, bus2, driver2, Time("18:00"), 2);
        
        auto trip98 = std::make_shared<Trip>(98, route3, tram1, driver6, Time("06:00"), 2);
        auto trip99 = std::make_shared<Trip>(99, route3, tram2, driver7, Time("07:30"), 2);
        auto trip100 = std::make_shared<Trip>(100, route3, tram3, driver8, Time("09:00"), 2);
        auto trip101 = std::make_shared<Trip>(101, route3, tram1, driver6, Time("11:30"), 2);
        auto trip102 = std::make_shared<Trip>(102, route3, tram2, driver7, Time("13:30"), 2);
        auto trip103 = std::make_shared<Trip>(103, route3, tram3, driver8, Time("15:30"), 2);
        auto trip104 = std::make_shared<Trip>(104, route3, tram1, driver6, Time("17:30"), 2);
        auto trip105 = std::make_shared<Trip>(105, route3, tram4, driver9, Time("19:30"), 2);
        
        auto trip106 = std::make_shared<Trip>(106, route4, trolleybus1, driver10, Time("07:15"), 2);
        auto trip107 = std::make_shared<Trip>(107, route4, trolleybus2, driver11, Time("09:15"), 2);
        auto trip108 = std::make_shared<Trip>(108, route4, trolleybus3, driver12, Time("11:15"), 2);
        auto trip109 = std::make_shared<Trip>(109, route4, trolleybus1, driver10, Time("13:15"), 2);
        auto trip110 = std::make_shared<Trip>(110, route4, trolleybus2, driver11, Time("15:15"), 2);
        auto trip111 = std::make_shared<Trip>(111, route4, trolleybus3, driver12, Time("17:15"), 2);
        
        auto trip112 = std::make_shared<Trip>(112, route5, bus3, driver13, Time("08:15"), 2);
        auto trip113 = std::make_shared<Trip>(113, route5, bus6, driver14, Time("10:15"), 2);
        auto trip114 = std::make_shared<Trip>(114, route5, bus3, driver13, Time("12:15"), 2);
        auto trip115 = std::make_shared<Trip>(115, route5, bus6, driver14, Time("14:15"), 2);
        auto trip116 = std::make_shared<Trip>(116, route5, bus3, driver13, Time("16:15"), 2);
        auto trip117 = std::make_shared<Trip>(117, route5, bus6, driver14, Time("18:15"), 2);
        
        auto trip118 = std::make_shared<Trip>(118, route6, tram2, driver15, Time("08:45"), 2);
        auto trip119 = std::make_shared<Trip>(119, route6, tram4, driver7, Time("10:45"), 2);
        auto trip120 = std::make_shared<Trip>(120, route6, tram2, driver15, Time("12:45"), 2);
        auto trip121 = std::make_shared<Trip>(121, route6, tram4, driver7, Time("14:45"), 2);
        auto trip122 = std::make_shared<Trip>(122, route6, tram2, driver15, Time("16:45"), 2);
        
        // Рейсы для среды (день 3)
        auto trip123 = std::make_shared<Trip>(123, route1, bus1, driver1, Time("06:30"), 3);
        auto trip124 = std::make_shared<Trip>(124, route1, bus2, driver2, Time("08:00"), 3);
        auto trip125 = std::make_shared<Trip>(125, route1, bus3, driver3, Time("09:30"), 3);
        auto trip126 = std::make_shared<Trip>(126, route1, bus1, driver1, Time("11:00"), 3);
        auto trip127 = std::make_shared<Trip>(127, route1, bus2, driver2, Time("13:00"), 3);
        auto trip128 = std::make_shared<Trip>(128, route1, bus3, driver3, Time("15:00"), 3);
        auto trip129 = std::make_shared<Trip>(129, route1, bus1, driver1, Time("17:00"), 3);
        auto trip130 = std::make_shared<Trip>(130, route1, bus2, driver2, Time("19:00"), 3);
        
        auto trip131 = std::make_shared<Trip>(131, route2, bus2, driver2, Time("07:00"), 3);
        auto trip132 = std::make_shared<Trip>(132, route2, bus4, driver4, Time("08:30"), 3);
        auto trip133 = std::make_shared<Trip>(133, route2, bus5, driver5, Time("10:00"), 3);
        auto trip134 = std::make_shared<Trip>(134, route2, bus2, driver2, Time("12:00"), 3);
        auto trip135 = std::make_shared<Trip>(135, route2, bus4, driver4, Time("14:00"), 3);
        auto trip136 = std::make_shared<Trip>(136, route2, bus5, driver5, Time("16:00"), 3);
        auto trip137 = std::make_shared<Trip>(137, route2, bus2, driver2, Time("18:00"), 3);
        
        auto trip138 = std::make_shared<Trip>(138, route3, tram1, driver6, Time("06:00"), 3);
        auto trip139 = std::make_shared<Trip>(139, route3, tram2, driver7, Time("07:30"), 3);
        auto trip140 = std::make_shared<Trip>(140, route3, tram3, driver8, Time("09:00"), 3);
        auto trip141 = std::make_shared<Trip>(141, route3, tram1, driver6, Time("11:30"), 3);
        auto trip142 = std::make_shared<Trip>(142, route3, tram2, driver7, Time("13:30"), 3);
        auto trip143 = std::make_shared<Trip>(143, route3, tram3, driver8, Time("15:30"), 3);
        auto trip144 = std::make_shared<Trip>(144, route3, tram1, driver6, Time("17:30"), 3);
        auto trip145 = std::make_shared<Trip>(145, route3, tram4, driver9, Time("19:30"), 3);
        
        auto trip146 = std::make_shared<Trip>(146, route4, trolleybus1, driver10, Time("07:15"), 3);
        auto trip147 = std::make_shared<Trip>(147, route4, trolleybus2, driver11, Time("09:15"), 3);
        auto trip148 = std::make_shared<Trip>(148, route4, trolleybus3, driver12, Time("11:15"), 3);
        auto trip149 = std::make_shared<Trip>(149, route4, trolleybus1, driver10, Time("13:15"), 3);
        auto trip150 = std::make_shared<Trip>(150, route4, trolleybus2, driver11, Time("15:15"), 3);
        auto trip151 = std::make_shared<Trip>(151, route4, trolleybus3, driver12, Time("17:15"), 3);
        
        auto trip152 = std::make_shared<Trip>(152, route5, bus3, driver13, Time("08:15"), 3);
        auto trip153 = std::make_shared<Trip>(153, route5, bus6, driver14, Time("10:15"), 3);
        auto trip154 = std::make_shared<Trip>(154, route5, bus3, driver13, Time("12:15"), 3);
        auto trip155 = std::make_shared<Trip>(155, route5, bus6, driver14, Time("14:15"), 3);
        auto trip156 = std::make_shared<Trip>(156, route5, bus3, driver13, Time("16:15"), 3);
        auto trip157 = std::make_shared<Trip>(157, route5, bus6, driver14, Time("18:15"), 3);
        
        auto trip158 = std::make_shared<Trip>(158, route6, tram2, driver15, Time("08:45"), 3);
        auto trip159 = std::make_shared<Trip>(159, route6, tram4, driver7, Time("10:45"), 3);
        auto trip160 = std::make_shared<Trip>(160, route6, tram2, driver15, Time("12:45"), 3);
        auto trip161 = std::make_shared<Trip>(161, route6, tram4, driver7, Time("14:45"), 3);
        auto trip162 = std::make_shared<Trip>(162, route6, tram2, driver15, Time("16:45"), 3);
        
        // Рейсы для четверга (день 4)
        auto trip163 = std::make_shared<Trip>(163, route1, bus1, driver1, Time("06:30"), 4);
        auto trip164 = std::make_shared<Trip>(164, route1, bus2, driver2, Time("08:00"), 4);
        auto trip165 = std::make_shared<Trip>(165, route1, bus3, driver3, Time("09:30"), 4);
        auto trip166 = std::make_shared<Trip>(166, route1, bus1, driver1, Time("11:00"), 4);
        auto trip167 = std::make_shared<Trip>(167, route1, bus2, driver2, Time("13:00"), 4);
        auto trip168 = std::make_shared<Trip>(168, route1, bus3, driver3, Time("15:00"), 4);
        auto trip169 = std::make_shared<Trip>(169, route1, bus1, driver1, Time("17:00"), 4);
        auto trip170 = std::make_shared<Trip>(170, route1, bus2, driver2, Time("19:00"), 4);
        
        auto trip171 = std::make_shared<Trip>(171, route2, bus2, driver2, Time("07:00"), 4);
        auto trip172 = std::make_shared<Trip>(172, route2, bus4, driver4, Time("08:30"), 4);
        auto trip173 = std::make_shared<Trip>(173, route2, bus5, driver5, Time("10:00"), 4);
        auto trip174 = std::make_shared<Trip>(174, route2, bus2, driver2, Time("12:00"), 4);
        auto trip175 = std::make_shared<Trip>(175, route2, bus4, driver4, Time("14:00"), 4);
        auto trip176 = std::make_shared<Trip>(176, route2, bus5, driver5, Time("16:00"), 4);
        auto trip177 = std::make_shared<Trip>(177, route2, bus2, driver2, Time("18:00"), 4);
        
        auto trip178 = std::make_shared<Trip>(178, route3, tram1, driver6, Time("06:00"), 4);
        auto trip179 = std::make_shared<Trip>(179, route3, tram2, driver7, Time("07:30"), 4);
        auto trip180 = std::make_shared<Trip>(180, route3, tram3, driver8, Time("09:00"), 4);
        auto trip181 = std::make_shared<Trip>(181, route3, tram1, driver6, Time("11:30"), 4);
        auto trip182 = std::make_shared<Trip>(182, route3, tram2, driver7, Time("13:30"), 4);
        auto trip183 = std::make_shared<Trip>(183, route3, tram3, driver8, Time("15:30"), 4);
        auto trip184 = std::make_shared<Trip>(184, route3, tram1, driver6, Time("17:30"), 4);
        auto trip185 = std::make_shared<Trip>(185, route3, tram4, driver9, Time("19:30"), 4);
        
        auto trip186 = std::make_shared<Trip>(186, route4, trolleybus1, driver10, Time("07:15"), 4);
        auto trip187 = std::make_shared<Trip>(187, route4, trolleybus2, driver11, Time("09:15"), 4);
        auto trip188 = std::make_shared<Trip>(188, route4, trolleybus3, driver12, Time("11:15"), 4);
        auto trip189 = std::make_shared<Trip>(189, route4, trolleybus1, driver10, Time("13:15"), 4);
        auto trip190 = std::make_shared<Trip>(190, route4, trolleybus2, driver11, Time("15:15"), 4);
        auto trip191 = std::make_shared<Trip>(191, route4, trolleybus3, driver12, Time("17:15"), 4);
        
        auto trip192 = std::make_shared<Trip>(192, route5, bus3, driver13, Time("08:15"), 4);
        auto trip193 = std::make_shared<Trip>(193, route5, bus6, driver14, Time("10:15"), 4);
        auto trip194 = std::make_shared<Trip>(194, route5, bus3, driver13, Time("12:15"), 4);
        auto trip195 = std::make_shared<Trip>(195, route5, bus6, driver14, Time("14:15"), 4);
        auto trip196 = std::make_shared<Trip>(196, route5, bus3, driver13, Time("16:15"), 4);
        auto trip197 = std::make_shared<Trip>(197, route5, bus6, driver14, Time("18:15"), 4);
        
        auto trip198 = std::make_shared<Trip>(198, route6, tram2, driver15, Time("08:45"), 4);
        auto trip199 = std::make_shared<Trip>(199, route6, tram4, driver7, Time("10:45"), 4);
        auto trip200 = std::make_shared<Trip>(200, route6, tram2, driver15, Time("12:45"), 4);
        auto trip201 = std::make_shared<Trip>(201, route6, tram4, driver7, Time("14:45"), 4);
        auto trip202 = std::make_shared<Trip>(202, route6, tram2, driver15, Time("16:45"), 4);
        
        // Рейсы для пятницы (день 5)
        auto trip203 = std::make_shared<Trip>(203, route1, bus1, driver1, Time("06:30"), 5);
        auto trip204 = std::make_shared<Trip>(204, route1, bus2, driver2, Time("08:00"), 5);
        auto trip205 = std::make_shared<Trip>(205, route1, bus3, driver3, Time("09:30"), 5);
        auto trip206 = std::make_shared<Trip>(206, route1, bus1, driver1, Time("11:00"), 5);
        auto trip207 = std::make_shared<Trip>(207, route1, bus2, driver2, Time("13:00"), 5);
        auto trip208 = std::make_shared<Trip>(208, route1, bus3, driver3, Time("15:00"), 5);
        auto trip209 = std::make_shared<Trip>(209, route1, bus1, driver1, Time("17:00"), 5);
        auto trip210 = std::make_shared<Trip>(210, route1, bus2, driver2, Time("19:00"), 5);
        
        auto trip211 = std::make_shared<Trip>(211, route2, bus2, driver2, Time("07:00"), 5);
        auto trip212 = std::make_shared<Trip>(212, route2, bus4, driver4, Time("08:30"), 5);
        auto trip213 = std::make_shared<Trip>(213, route2, bus5, driver5, Time("10:00"), 5);
        auto trip214 = std::make_shared<Trip>(214, route2, bus2, driver2, Time("12:00"), 5);
        auto trip215 = std::make_shared<Trip>(215, route2, bus4, driver4, Time("14:00"), 5);
        auto trip216 = std::make_shared<Trip>(216, route2, bus5, driver5, Time("16:00"), 5);
        auto trip217 = std::make_shared<Trip>(217, route2, bus2, driver2, Time("18:00"), 5);
        
        auto trip218 = std::make_shared<Trip>(218, route3, tram1, driver6, Time("06:00"), 5);
        auto trip219 = std::make_shared<Trip>(219, route3, tram2, driver7, Time("07:30"), 5);
        auto trip220 = std::make_shared<Trip>(220, route3, tram3, driver8, Time("09:00"), 5);
        auto trip221 = std::make_shared<Trip>(221, route3, tram1, driver6, Time("11:30"), 5);
        auto trip222 = std::make_shared<Trip>(222, route3, tram2, driver7, Time("13:30"), 5);
        auto trip223 = std::make_shared<Trip>(223, route3, tram3, driver8, Time("15:30"), 5);
        auto trip224 = std::make_shared<Trip>(224, route3, tram1, driver6, Time("17:30"), 5);
        auto trip225 = std::make_shared<Trip>(225, route3, tram4, driver9, Time("19:30"), 5);
        
        auto trip226 = std::make_shared<Trip>(226, route4, trolleybus1, driver10, Time("07:15"), 5);
        auto trip227 = std::make_shared<Trip>(227, route4, trolleybus2, driver11, Time("09:15"), 5);
        auto trip228 = std::make_shared<Trip>(228, route4, trolleybus3, driver12, Time("11:15"), 5);
        auto trip229 = std::make_shared<Trip>(229, route4, trolleybus1, driver10, Time("13:15"), 5);
        auto trip230 = std::make_shared<Trip>(230, route4, trolleybus2, driver11, Time("15:15"), 5);
        auto trip231 = std::make_shared<Trip>(231, route4, trolleybus3, driver12, Time("17:15"), 5);
        
        auto trip232 = std::make_shared<Trip>(232, route5, bus3, driver13, Time("08:15"), 5);
        auto trip233 = std::make_shared<Trip>(233, route5, bus6, driver14, Time("10:15"), 5);
        auto trip234 = std::make_shared<Trip>(234, route5, bus3, driver13, Time("12:15"), 5);
        auto trip235 = std::make_shared<Trip>(235, route5, bus6, driver14, Time("14:15"), 5);
        auto trip236 = std::make_shared<Trip>(236, route5, bus3, driver13, Time("16:15"), 5);
        auto trip237 = std::make_shared<Trip>(237, route5, bus6, driver14, Time("18:15"), 5);
        
        auto trip238 = std::make_shared<Trip>(238, route6, tram2, driver15, Time("08:45"), 5);
        auto trip239 = std::make_shared<Trip>(239, route6, tram4, driver7, Time("10:45"), 5);
        auto trip240 = std::make_shared<Trip>(240, route6, tram2, driver15, Time("12:45"), 5);
        auto trip241 = std::make_shared<Trip>(241, route6, tram4, driver7, Time("14:45"), 5);
        auto trip242 = std::make_shared<Trip>(242, route6, tram2, driver15, Time("16:45"), 5);
        
        // Рейсы для субботы (день 6)
        auto trip243 = std::make_shared<Trip>(243, route1, bus1, driver1, Time("08:00"), 6);
        auto trip244 = std::make_shared<Trip>(244, route1, bus2, driver2, Time("10:00"), 6);
        auto trip245 = std::make_shared<Trip>(245, route1, bus3, driver3, Time("12:00"), 6);
        auto trip246 = std::make_shared<Trip>(246, route1, bus1, driver1, Time("14:00"), 6);
        auto trip247 = std::make_shared<Trip>(247, route1, bus2, driver2, Time("16:00"), 6);
        
        auto trip248 = std::make_shared<Trip>(248, route2, bus2, driver2, Time("09:00"), 6);
        auto trip249 = std::make_shared<Trip>(249, route2, bus4, driver4, Time("11:00"), 6);
        auto trip250 = std::make_shared<Trip>(250, route2, bus5, driver5, Time("13:00"), 6);
        auto trip251 = std::make_shared<Trip>(251, route2, bus2, driver2, Time("15:00"), 6);
        
        auto trip252 = std::make_shared<Trip>(252, route3, tram1, driver6, Time("08:30"), 6);
        auto trip253 = std::make_shared<Trip>(253, route3, tram2, driver7, Time("10:30"), 6);
        auto trip254 = std::make_shared<Trip>(254, route3, tram3, driver8, Time("12:30"), 6);
        auto trip255 = std::make_shared<Trip>(255, route3, tram1, driver6, Time("14:30"), 6);
        
        auto trip256 = std::make_shared<Trip>(256, route4, trolleybus1, driver10, Time("09:15"), 6);
        auto trip257 = std::make_shared<Trip>(257, route4, trolleybus2, driver11, Time("11:15"), 6);
        auto trip258 = std::make_shared<Trip>(258, route4, trolleybus3, driver12, Time("13:15"), 6);
        auto trip259 = std::make_shared<Trip>(259, route4, trolleybus1, driver10, Time("15:15"), 6);
        
        // Рейсы для воскресенья (день 7)
        auto trip260 = std::make_shared<Trip>(260, route1, bus1, driver1, Time("09:00"), 7);
        auto trip261 = std::make_shared<Trip>(261, route1, bus2, driver2, Time("11:00"), 7);
        auto trip262 = std::make_shared<Trip>(262, route1, bus3, driver3, Time("13:00"), 7);
        auto trip263 = std::make_shared<Trip>(263, route1, bus1, driver1, Time("15:00"), 7);
        
        auto trip264 = std::make_shared<Trip>(264, route2, bus2, driver2, Time("10:00"), 7);
        auto trip265 = std::make_shared<Trip>(265, route2, bus4, driver4, Time("12:00"), 7);
        auto trip266 = std::make_shared<Trip>(266, route2, bus5, driver5, Time("14:00"), 7);
        
        auto trip267 = std::make_shared<Trip>(267, route3, tram1, driver6, Time("09:30"), 7);
        auto trip268 = std::make_shared<Trip>(268, route3, tram2, driver7, Time("11:30"), 7);
        auto trip269 = std::make_shared<Trip>(269, route3, tram3, driver8, Time("13:30"), 7);
        
        auto trip270 = std::make_shared<Trip>(270, route4, trolleybus1, driver10, Time("10:15"), 7);
        auto trip271 = std::make_shared<Trip>(271, route4, trolleybus2, driver11, Time("12:15"), 7);
        auto trip272 = std::make_shared<Trip>(272, route4, trolleybus3, driver12, Time("14:15"), 7);

        // Добавляем все рейсы в систему
        system.addTripDirect(trip1);
        system.addTripDirect(trip2);
        system.addTripDirect(trip3);
        system.addTripDirect(trip4);
        system.addTripDirect(trip5);
        system.addTripDirect(trip6);
        system.addTripDirect(trip7);
        system.addTripDirect(trip8);
        system.addTripDirect(trip9);
        system.addTripDirect(trip10);
        system.addTripDirect(trip11);
        system.addTripDirect(trip12);
        system.addTripDirect(trip13);
        system.addTripDirect(trip14);
        system.addTripDirect(trip15);
        system.addTripDirect(trip16);
        system.addTripDirect(trip17);
        system.addTripDirect(trip18);
        system.addTripDirect(trip19);
        system.addTripDirect(trip20);
        system.addTripDirect(trip21);
        system.addTripDirect(trip22);
        system.addTripDirect(trip23);
        system.addTripDirect(trip24);
        system.addTripDirect(trip25);
        system.addTripDirect(trip26);
        system.addTripDirect(trip27);
        system.addTripDirect(trip28);
        system.addTripDirect(trip29);
        system.addTripDirect(trip30);
        system.addTripDirect(trip31);
        system.addTripDirect(trip32);
        system.addTripDirect(trip33);
        system.addTripDirect(trip34);
        system.addTripDirect(trip35);
        system.addTripDirect(trip36);
        system.addTripDirect(trip37);
        system.addTripDirect(trip38);
        system.addTripDirect(trip39);
        system.addTripDirect(trip40);
        system.addTripDirect(trip41);
        system.addTripDirect(trip42);
        system.addTripDirect(trip43);
        system.addTripDirect(trip44);
        system.addTripDirect(trip45);
        system.addTripDirect(trip46);
        system.addTripDirect(trip47);
        system.addTripDirect(trip48);
        system.addTripDirect(trip49);
        system.addTripDirect(trip50);
        system.addTripDirect(trip51);
        system.addTripDirect(trip52);
        system.addTripDirect(trip53);
        system.addTripDirect(trip54);
        system.addTripDirect(trip55);
        system.addTripDirect(trip56);
        system.addTripDirect(trip57);
        system.addTripDirect(trip58);
        system.addTripDirect(trip59);
        system.addTripDirect(trip60);
        system.addTripDirect(trip61);
        system.addTripDirect(trip62);
        system.addTripDirect(trip63);
        system.addTripDirect(trip64);
        system.addTripDirect(trip65);
        system.addTripDirect(trip66);
        system.addTripDirect(trip67);
        system.addTripDirect(trip68);
        system.addTripDirect(trip69);
        system.addTripDirect(trip70);
        system.addTripDirect(trip71);
        system.addTripDirect(trip72);
        system.addTripDirect(trip73);
        system.addTripDirect(trip74);
        system.addTripDirect(trip75);
        system.addTripDirect(trip76);
        system.addTripDirect(trip77);
        system.addTripDirect(trip78);
        system.addTripDirect(trip79);
        system.addTripDirect(trip80);
        system.addTripDirect(trip81);
        system.addTripDirect(trip82);
        system.addTripDirect(trip83);
        system.addTripDirect(trip84);
        system.addTripDirect(trip85);
        system.addTripDirect(trip86);
        system.addTripDirect(trip87);
        system.addTripDirect(trip88);
        system.addTripDirect(trip89);
        system.addTripDirect(trip90);
        system.addTripDirect(trip91);
        system.addTripDirect(trip92);
        system.addTripDirect(trip93);
        system.addTripDirect(trip94);
        system.addTripDirect(trip95);
        system.addTripDirect(trip96);
        system.addTripDirect(trip97);
        system.addTripDirect(trip98);
        system.addTripDirect(trip99);
        system.addTripDirect(trip100);
        system.addTripDirect(trip101);
        system.addTripDirect(trip102);
        system.addTripDirect(trip103);
        system.addTripDirect(trip104);
        system.addTripDirect(trip105);
        system.addTripDirect(trip106);
        system.addTripDirect(trip107);
        system.addTripDirect(trip108);
        system.addTripDirect(trip109);
        system.addTripDirect(trip110);
        system.addTripDirect(trip111);
        system.addTripDirect(trip112);
        system.addTripDirect(trip113);
        system.addTripDirect(trip114);
        system.addTripDirect(trip115);
        system.addTripDirect(trip116);
        system.addTripDirect(trip117);
        system.addTripDirect(trip118);
        system.addTripDirect(trip119);
        system.addTripDirect(trip120);
        system.addTripDirect(trip121);
        system.addTripDirect(trip122);
        system.addTripDirect(trip123);
        system.addTripDirect(trip124);
        system.addTripDirect(trip125);
        system.addTripDirect(trip126);
        system.addTripDirect(trip127);
        system.addTripDirect(trip128);
        system.addTripDirect(trip129);
        system.addTripDirect(trip130);
        system.addTripDirect(trip131);
        system.addTripDirect(trip132);
        system.addTripDirect(trip133);
        system.addTripDirect(trip134);
        system.addTripDirect(trip135);
        system.addTripDirect(trip136);
        system.addTripDirect(trip137);
        system.addTripDirect(trip138);
        system.addTripDirect(trip139);
        system.addTripDirect(trip140);
        system.addTripDirect(trip141);
        system.addTripDirect(trip142);
        system.addTripDirect(trip143);
        system.addTripDirect(trip144);
        system.addTripDirect(trip145);
        system.addTripDirect(trip146);
        system.addTripDirect(trip147);
        system.addTripDirect(trip148);
        system.addTripDirect(trip149);
        system.addTripDirect(trip150);
        system.addTripDirect(trip151);
        system.addTripDirect(trip152);
        system.addTripDirect(trip153);
        system.addTripDirect(trip154);
        system.addTripDirect(trip155);
        system.addTripDirect(trip156);
        system.addTripDirect(trip157);
        system.addTripDirect(trip158);
        system.addTripDirect(trip159);
        system.addTripDirect(trip160);
        system.addTripDirect(trip161);
        system.addTripDirect(trip162);
        system.addTripDirect(trip163);
        system.addTripDirect(trip164);
        system.addTripDirect(trip165);
        system.addTripDirect(trip166);
        system.addTripDirect(trip167);
        system.addTripDirect(trip168);
        system.addTripDirect(trip169);
        system.addTripDirect(trip170);
        system.addTripDirect(trip171);
        system.addTripDirect(trip172);
        system.addTripDirect(trip173);
        system.addTripDirect(trip174);
        system.addTripDirect(trip175);
        system.addTripDirect(trip176);
        system.addTripDirect(trip177);
        system.addTripDirect(trip178);
        system.addTripDirect(trip179);
        system.addTripDirect(trip180);
        system.addTripDirect(trip181);
        system.addTripDirect(trip182);
        system.addTripDirect(trip183);
        system.addTripDirect(trip184);
        system.addTripDirect(trip185);
        system.addTripDirect(trip186);
        system.addTripDirect(trip187);
        system.addTripDirect(trip188);
        system.addTripDirect(trip189);
        system.addTripDirect(trip190);
        system.addTripDirect(trip191);
        system.addTripDirect(trip192);
        system.addTripDirect(trip193);
        system.addTripDirect(trip194);
        system.addTripDirect(trip195);
        system.addTripDirect(trip196);
        system.addTripDirect(trip197);
        system.addTripDirect(trip198);
        system.addTripDirect(trip199);
        system.addTripDirect(trip200);
        system.addTripDirect(trip201);
        system.addTripDirect(trip202);
        system.addTripDirect(trip203);
        system.addTripDirect(trip204);
        system.addTripDirect(trip205);
        system.addTripDirect(trip206);
        system.addTripDirect(trip207);
        system.addTripDirect(trip208);
        system.addTripDirect(trip209);
        system.addTripDirect(trip210);
        system.addTripDirect(trip211);
        system.addTripDirect(trip212);
        system.addTripDirect(trip213);
        system.addTripDirect(trip214);
        system.addTripDirect(trip215);
        system.addTripDirect(trip216);
        system.addTripDirect(trip217);
        system.addTripDirect(trip218);
        system.addTripDirect(trip219);
        system.addTripDirect(trip220);
        system.addTripDirect(trip221);
        system.addTripDirect(trip222);
        system.addTripDirect(trip223);
        system.addTripDirect(trip224);
        system.addTripDirect(trip225);
        system.addTripDirect(trip226);
        system.addTripDirect(trip227);
        system.addTripDirect(trip228);
        system.addTripDirect(trip229);
        system.addTripDirect(trip230);
        system.addTripDirect(trip231);
        system.addTripDirect(trip232);
        system.addTripDirect(trip233);
        system.addTripDirect(trip234);
        system.addTripDirect(trip235);
        system.addTripDirect(trip236);
        system.addTripDirect(trip237);
        system.addTripDirect(trip238);
        system.addTripDirect(trip239);
        system.addTripDirect(trip240);
        system.addTripDirect(trip241);
        system.addTripDirect(trip242);
        system.addTripDirect(trip243);
        system.addTripDirect(trip244);
        system.addTripDirect(trip245);
        system.addTripDirect(trip246);
        system.addTripDirect(trip247);
        system.addTripDirect(trip248);
        system.addTripDirect(trip249);
        system.addTripDirect(trip250);
        system.addTripDirect(trip251);
        system.addTripDirect(trip252);
        system.addTripDirect(trip253);
        system.addTripDirect(trip254);
        system.addTripDirect(trip255);
        system.addTripDirect(trip256);
        system.addTripDirect(trip257);
        system.addTripDirect(trip258);
        system.addTripDirect(trip259);
        system.addTripDirect(trip260);
        system.addTripDirect(trip261);
        system.addTripDirect(trip262);
        system.addTripDirect(trip263);
        system.addTripDirect(trip264);
        system.addTripDirect(trip265);
        system.addTripDirect(trip266);
        system.addTripDirect(trip267);
        system.addTripDirect(trip268);
        system.addTripDirect(trip269);
        system.addTripDirect(trip270);
        system.addTripDirect(trip271);
        system.addTripDirect(trip272);

        // Рассчитываем время прибытия для всех рейсов
        // Скорости: автобусы ~30 км/ч, трамваи ~25 км/ч, троллейбусы ~28 км/ч
        for (int i = 1; i <= 272; i++) {
            auto trip = system.getTripById(i);
            if (trip) {
                std::string vehicleType = trip->getRoute()->getVehicleType();
                double speed = 30.0; // по умолчанию
                if (vehicleType == "Трамвай") speed = 25.0;
                else if (vehicleType == "Троллейбус") speed = 28.0;
                system.calculateArrivalTimes(i, speed);
            }
        }

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

