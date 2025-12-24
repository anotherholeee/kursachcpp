#include "bus.h"

// Конструктор автобуса
Bus::Bus(const std::string& m, const std::string& lp, int cap, const std::string& fuel)
    : FuelTransport("Автобус", m, lp, cap, fuel) {}

