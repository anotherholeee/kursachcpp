cmake_minimum_required(VERSION 4.0)
project(kursovayacpp)

set(CMAKE_CXX_STANDARD 20)

# Исходные файлы
set(SOURCES
    main.cpp
    exceptions.cpp
    stop.cpp
    time.cpp
    vehicle.cpp
    electric_transport.cpp
    fuel_transport.cpp
    tram.cpp
    trolleybus.cpp
    bus.cpp
    driver.cpp
    route.cpp
    trip.cpp
    journey.cpp
    algorithm.cpp
    journey_planner.cpp
    driver_schedule.cpp
    data_manager.cpp
    command.cpp
    commands.cpp
    transport_system.cpp
    ui.cpp
)

add_executable(kursovayacpp ${SOURCES})
