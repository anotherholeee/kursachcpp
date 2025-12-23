#ifndef UI_H
#define UI_H

#include "transport_system.h"
#include <string>

// Функции валидации ввода
bool isValidNumber(const std::string& str);
bool isValidText(const std::string& str);
bool containsOnlyDigits(const std::string& str);
bool containsOnlyLetters(const std::string& str);
bool hasCapitalFirstLetter(const std::string& str);
bool isValidNameFormat(const std::string& str);
bool isValidLength(const std::string& str, size_t minLen, size_t maxLen);
bool isValidLicensePlate(const std::string& str);
bool isValidDriverCategory(const std::string& str);

void displayGuestMenu();
void displayAdminMenu();
void displayLoginMenu();
void displayAllStopsForSelection(const TransportSystem& system);
std::string getStopNameByInput(const TransportSystem& system, const std::string& input);

void adminAddRoute(TransportSystem& system);
void adminAddTrip(TransportSystem& system);
void adminAddVehicle(TransportSystem& system);
void adminAddStop(TransportSystem& system);
void adminAddDriver(TransportSystem& system);

void viewTransportSchedule(TransportSystem& system);
void viewTransportScheduleGuest(TransportSystem& system);
void viewStopTimetable(TransportSystem& system);
void searchRoutes(TransportSystem& system);
void calculateArrivalTime(TransportSystem& system);
void showAllTrips(const TransportSystem& system);

void runGuestMode(TransportSystem& system);
void runAdminMode(TransportSystem& system);

void initializeTestData(TransportSystem& system);

#endif // UI_H

