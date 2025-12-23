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
#include <QMessageBox>
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
    List<std::string> route1Stops;
    route1Stops.push_back("Центральный вокзал");
    route1Stops.push_back("Площадь Ленина");
    route1Stops.push_back("Улица Гагарина");
    route1Stops.push_back("Стадион");
    auto route1 = std::make_shared<Route>(101, "Автобус", route1Stops);

    List<std::string> route2Stops;
    route2Stops.push_back("Центральный вокзал");
    route2Stops.push_back("Площадь Ленина");
    route2Stops.push_back("Больница");
    route2Stops.push_back("Университет");
    auto route2 = std::make_shared<Route>(202, "Автобус", route2Stops);

    List<std::string> route3Stops;
    route3Stops.push_back("Парк Победы");
    route3Stops.push_back("Улица Гагарина");
    route3Stops.push_back("Больница");
    route3Stops.push_back("Университет");
    auto route3 = std::make_shared<Route>(5, "Трамвай", route3Stops);

    List<std::string> route4Stops;
    route4Stops.push_back("Стадион");
    route4Stops.push_back("Улица Гагарина");
    route4Stops.push_back("Парк Победы");
    auto route4 = std::make_shared<Route>(303, "Троллейбус", route4Stops);

    List<std::string> route5Stops;
    route5Stops.push_back("Университет");
    route5Stops.push_back("Больница");
    route5Stops.push_back("Площадь Ленина");
    route5Stops.push_back("Центральный вокзал");
    auto route5 = std::make_shared<Route>(404, "Автобус", route5Stops);

    List<std::string> route6Stops;
    route6Stops.push_back("Парк Победы");
    route6Stops.push_back("Улица Гагарина");
    route6Stops.push_back("Площадь Ленина");
    auto route6 = std::make_shared<Route>(7, "Трамвай", route6Stops);

    List<std::string> route7Stops;
    route7Stops.push_back("Железнодорожный вокзал");
    route7Stops.push_back("Центральный вокзал");
    route7Stops.push_back("Торговый центр");
    route7Stops.push_back("Поликлиника");
    auto route7 = std::make_shared<Route>(105, "Автобус", route7Stops);

    List<std::string> route8Stops;
    route8Stops.push_back("Аэропорт");
    route8Stops.push_back("Железнодорожный вокзал");
    route8Stops.push_back("Центральный вокзал");
    route8Stops.push_back("Площадь Ленина");
    auto route8 = std::make_shared<Route>(206, "Автобус", route8Stops);

    List<std::string> route9Stops;
    route9Stops.push_back("Университет");
    route9Stops.push_back("Школа №1");
    route9Stops.push_back("Поликлиника");
    route9Stops.push_back("Больница");
    auto route9 = std::make_shared<Route>(8, "Трамвай", route9Stops);

    List<std::string> route10Stops;
    route10Stops.push_back("Центральный рынок");
    route10Stops.push_back("Площадь Ленина");
    route10Stops.push_back("Театр");
    route10Stops.push_back("Библиотека");
    auto route10 = std::make_shared<Route>(304, "Троллейбус", route10Stops);

    List<std::string> route11Stops;
    route11Stops.push_back("Спортивный комплекс");
    route11Stops.push_back("Стадион");
    route11Stops.push_back("Парк культуры");
    route11Stops.push_back("Парк Победы");
    auto route11 = std::make_shared<Route>(9, "Трамвай", route11Stops);

    List<std::string> route12Stops;
    route12Stops.push_back("Автовокзал");
    route12Stops.push_back("Центральный вокзал");
    route12Stops.push_back("Площадь Ленина");
    route12Stops.push_back("Торговый центр");
    auto route12 = std::make_shared<Route>(107, "Автобус", route12Stops);

    List<std::string> route13Stops;
    route13Stops.push_back("Завод");
    route13Stops.push_back("Микрорайон Северный");
    route13Stops.push_back("Университет");
    route13Stops.push_back("Больница");
    auto route13 = std::make_shared<Route>(305, "Троллейбус", route13Stops);

    List<std::string> route14Stops;
    route14Stops.push_back("Парк культуры");
    route14Stops.push_back("Библиотека");
    route14Stops.push_back("Театр");
    route14Stops.push_back("Центральный рынок");
    auto route14 = std::make_shared<Route>(306, "Троллейбус", route14Stops);

    List<std::string> route15Stops;
    route15Stops.push_back("Аэропорт");
    route15Stops.push_back("Железнодорожный вокзал");
    route15Stops.push_back("Автовокзал");
    route15Stops.push_back("Центральный вокзал");
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
        // Рейсы для понедельника (день 1) - все рейсы только в понедельник
        auto trip1 = std::make_shared<Trip>(1, route1, bus1, driver1, Time("06:30"), 1);
        auto trip2 = std::make_shared<Trip>(2, route1, bus2, driver2, Time("08:00"), 1);
        auto trip3 = std::make_shared<Trip>(3, route1, bus3, driver3, Time("09:30"), 1);
        auto trip4 = std::make_shared<Trip>(4, route1, bus1, driver1, Time("11:00"), 1);
        auto trip5 = std::make_shared<Trip>(5, route1, bus2, driver2, Time("13:00"), 1);
        auto trip6 = std::make_shared<Trip>(6, route1, bus3, driver3, Time("15:00"), 1);
        auto trip7 = std::make_shared<Trip>(7, route1, bus1, driver1, Time("17:00"), 1);
        auto trip8 = std::make_shared<Trip>(8, route1, bus2, driver2, Time("19:00"), 1);

        auto trip9 = std::make_shared<Trip>(9, route2, bus2, driver2, Time("07:00"), 1);
        auto trip10 = std::make_shared<Trip>(10, route2, bus4, driver4, Time("08:30"), 1);
        auto trip11 = std::make_shared<Trip>(11, route2, bus5, driver5, Time("10:00"), 1);
        auto trip12 = std::make_shared<Trip>(12, route2, bus2, driver2, Time("12:00"), 1);
        auto trip13 = std::make_shared<Trip>(13, route2, bus4, driver4, Time("14:00"), 1);
        auto trip14 = std::make_shared<Trip>(14, route2, bus5, driver5, Time("16:00"), 1);
        auto trip15 = std::make_shared<Trip>(15, route2, bus2, driver2, Time("18:00"), 1);

        auto trip16 = std::make_shared<Trip>(16, route3, tram1, driver6, Time("06:00"), 1);
        auto trip17 = std::make_shared<Trip>(17, route3, tram2, driver7, Time("07:30"), 1);
        auto trip18 = std::make_shared<Trip>(18, route3, tram3, driver8, Time("09:00"), 1);
        auto trip19 = std::make_shared<Trip>(19, route3, tram1, driver6, Time("11:30"), 1);
        auto trip20 = std::make_shared<Trip>(20, route3, tram2, driver7, Time("13:30"), 1);
        auto trip21 = std::make_shared<Trip>(21, route3, tram3, driver8, Time("15:30"), 1);
        auto trip22 = std::make_shared<Trip>(22, route3, tram1, driver6, Time("17:30"), 1);
        auto trip23 = std::make_shared<Trip>(23, route3, tram4, driver9, Time("19:30"), 1);

        auto trip24 = std::make_shared<Trip>(24, route4, trolleybus1, driver10, Time("07:15"), 1);
        auto trip25 = std::make_shared<Trip>(25, route4, trolleybus2, driver11, Time("09:15"), 1);
        auto trip26 = std::make_shared<Trip>(26, route4, trolleybus3, driver12, Time("11:15"), 1);
        auto trip27 = std::make_shared<Trip>(27, route4, trolleybus1, driver10, Time("13:15"), 1);
        auto trip28 = std::make_shared<Trip>(28, route4, trolleybus2, driver11, Time("15:15"), 1);
        auto trip29 = std::make_shared<Trip>(29, route4, trolleybus3, driver12, Time("17:15"), 1);

        auto trip30 = std::make_shared<Trip>(30, route5, bus3, driver13, Time("08:15"), 1);
        auto trip31 = std::make_shared<Trip>(31, route5, bus6, driver14, Time("10:15"), 1);
        auto trip32 = std::make_shared<Trip>(32, route5, bus3, driver13, Time("12:15"), 1);
        auto trip33 = std::make_shared<Trip>(33, route5, bus6, driver14, Time("14:15"), 1);
        auto trip34 = std::make_shared<Trip>(34, route5, bus3, driver13, Time("16:15"), 1);
        auto trip35 = std::make_shared<Trip>(35, route5, bus6, driver14, Time("18:15"), 1);

        auto trip36 = std::make_shared<Trip>(36, route6, tram2, driver15, Time("08:45"), 1);
        auto trip37 = std::make_shared<Trip>(37, route6, tram4, driver7, Time("10:45"), 1);
        auto trip38 = std::make_shared<Trip>(38, route6, tram2, driver15, Time("12:45"), 1);
        auto trip39 = std::make_shared<Trip>(39, route6, tram4, driver7, Time("14:45"), 1);
        auto trip40 = std::make_shared<Trip>(40, route6, tram2, driver15, Time("16:45"), 1);

        auto trip41 = std::make_shared<Trip>(41, route7, bus4, driver1, Time("09:00"), 1);
        auto trip42 = std::make_shared<Trip>(42, route7, bus5, driver2, Time("11:00"), 1);
        auto trip43 = std::make_shared<Trip>(43, route7, bus4, driver1, Time("13:00"), 1);
        auto trip44 = std::make_shared<Trip>(44, route7, bus5, driver2, Time("15:00"), 1);
        auto trip45 = std::make_shared<Trip>(45, route7, bus4, driver1, Time("17:00"), 1);

        auto trip46 = std::make_shared<Trip>(46, route8, bus6, driver3, Time("08:20"), 1);
        auto trip47 = std::make_shared<Trip>(47, route8, bus1, driver4, Time("10:20"), 1);
        auto trip48 = std::make_shared<Trip>(48, route8, bus6, driver3, Time("12:20"), 1);
        auto trip49 = std::make_shared<Trip>(49, route8, bus1, driver4, Time("14:20"), 1);
        auto trip50 = std::make_shared<Trip>(50, route8, bus6, driver3, Time("16:20"), 1);

        auto trip51 = std::make_shared<Trip>(51, route9, tram1, driver5, Time("07:00"), 1);
        auto trip52 = std::make_shared<Trip>(52, route9, tram3, driver8, Time("09:00"), 1);
        auto trip53 = std::make_shared<Trip>(53, route9, tram1, driver5, Time("11:00"), 1);
        auto trip54 = std::make_shared<Trip>(54, route9, tram3, driver8, Time("13:00"), 1);
        auto trip55 = std::make_shared<Trip>(55, route9, tram1, driver5, Time("15:00"), 1);

        auto trip56 = std::make_shared<Trip>(56, route10, trolleybus2, driver7, Time("08:30"), 1);
        auto trip57 = std::make_shared<Trip>(57, route10, trolleybus4, driver8, Time("10:30"), 1);
        auto trip58 = std::make_shared<Trip>(58, route10, trolleybus2, driver7, Time("12:30"), 1);
        auto trip59 = std::make_shared<Trip>(59, route10, trolleybus4, driver8, Time("14:30"), 1);
        auto trip60 = std::make_shared<Trip>(60, route10, trolleybus2, driver7, Time("16:30"), 1);

        auto trip61 = std::make_shared<Trip>(61, route11, tram2, driver9, Time("08:00"), 1);
        auto trip62 = std::make_shared<Trip>(62, route11, tram4, driver10, Time("10:00"), 1);
        auto trip63 = std::make_shared<Trip>(63, route11, tram2, driver9, Time("12:00"), 1);
        auto trip64 = std::make_shared<Trip>(64, route11, tram4, driver10, Time("14:00"), 1);
        auto trip65 = std::make_shared<Trip>(65, route11, tram2, driver9, Time("16:00"), 1);

        auto trip66 = std::make_shared<Trip>(66, route12, bus2, driver11, Time("09:30"), 1);
        auto trip67 = std::make_shared<Trip>(67, route12, bus3, driver12, Time("11:30"), 1);
        auto trip68 = std::make_shared<Trip>(68, route12, bus2, driver11, Time("13:30"), 1);
        auto trip69 = std::make_shared<Trip>(69, route12, bus3, driver12, Time("15:30"), 1);
        auto trip70 = std::make_shared<Trip>(70, route12, bus2, driver11, Time("17:30"), 1);

        auto trip71 = std::make_shared<Trip>(71, route13, trolleybus3, driver13, Time("08:00"), 1);
        auto trip72 = std::make_shared<Trip>(72, route13, trolleybus1, driver14, Time("10:00"), 1);
        auto trip73 = std::make_shared<Trip>(73, route13, trolleybus3, driver13, Time("12:00"), 1);
        auto trip74 = std::make_shared<Trip>(74, route13, trolleybus1, driver14, Time("14:00"), 1);
        auto trip75 = std::make_shared<Trip>(75, route13, trolleybus3, driver13, Time("16:00"), 1);

        auto trip76 = std::make_shared<Trip>(76, route14, trolleybus1, driver15, Time("09:00"), 1);
        auto trip77 = std::make_shared<Trip>(77, route14, trolleybus2, driver7, Time("11:00"), 1);
        auto trip78 = std::make_shared<Trip>(78, route14, trolleybus1, driver15, Time("13:00"), 1);
        auto trip79 = std::make_shared<Trip>(79, route14, trolleybus2, driver7, Time("15:00"), 1);
        auto trip80 = std::make_shared<Trip>(80, route14, trolleybus1, driver15, Time("17:00"), 1);

        auto trip81 = std::make_shared<Trip>(81, route15, bus5, driver2, Time("07:30"), 1);
        auto trip82 = std::make_shared<Trip>(82, route15, bus6, driver3, Time("09:30"), 1);
        auto trip83 = std::make_shared<Trip>(83, route15, bus5, driver2, Time("11:30"), 1);
        auto trip84 = std::make_shared<Trip>(84, route15, bus6, driver3, Time("13:30"), 1);
        auto trip85 = std::make_shared<Trip>(85, route15, bus5, driver2, Time("15:30"), 1);
        auto trip86 = std::make_shared<Trip>(86, route15, bus6, driver3, Time("17:30"), 1);

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

        // Рассчитываем время прибытия для всех рейсов
        // Скорости: автобусы ~30 км/ч, трамваи ~25 км/ч, троллейбусы ~28 км/ч
        for (int i = 1; i <= 86; i++) {
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
        // Ошибка при создании тестовых рейсов - не критично, просто пропускаем
        // Можно показать предупреждение, но не критическую ошибку
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
        QMessageBox::critical(nullptr, "Критическая ошибка ввода", QString("Критическая ошибка ввода: %1").arg(e.what()));
        try {
            TransportSystem tempSystem;
            tempSystem.saveData();
        } catch (...) {
        }
        return 1;
    } catch (const FileException& e) {
        QMessageBox::critical(nullptr, "Критическая ошибка файла", QString("Критическая ошибка файла: %1").arg(e.what()));
        return 1;
    } catch (const ContainerException& e) {
        QMessageBox::critical(nullptr, "Критическая ошибка данных", QString("Критическая ошибка данных: %1").arg(e.what()));
        return 1;
    } catch (const TransportException& e) {
        QMessageBox::critical(nullptr, "Критическая ошибка системы", QString("Критическая ошибка системы: %1").arg(e.what()));
        return 1;
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Критическая ошибка", QString("Критическая ошибка: %1").arg(e.what()));
        return 1;
    }
}