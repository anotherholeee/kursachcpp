#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <filesystem>
#include <fstream>

class TransportSystem;

/**
 * @class DataManager
 * @brief Класс для управления сохранением и загрузкой данных
 * 
 * Обеспечивает персистентность данных транспортной системы:
 * сохранение и загрузку остановок, транспортных средств, водителей,
 * маршрутов, рейсов и учетных данных администраторов.
 */
class DataManager {
private:
    std::string dataDirectory;  // Путь к директории с данными

public:
    /**
     * @brief Конструктор менеджера данных
     * @param dir Путь к директории с данными (по умолчанию "data/")
     * 
     * Создает директорию, если она не существует, и нормализует путь.
     */
    DataManager(const std::string& dir = "data/");

    /**
     * @brief Сохранить все данные транспортной системы в файлы
     * @param system Транспортная система для сохранения
     */
    void saveAllData(TransportSystem& system);

    /**
     * @brief Загрузить все данные транспортной системы из файлов
     * @param system Транспортная система для загрузки данных
     */
    void loadAllData(TransportSystem& system);

private:
    // Методы сохранения отдельных типов данных
    void saveStops(TransportSystem& system);
    void saveVehicles(TransportSystem& system);
    void saveDrivers(TransportSystem& system);
    void saveRoutes(TransportSystem& system);
    void saveTrips(TransportSystem& system);
    void saveAdminCredentials(TransportSystem& system);

    // Методы загрузки отдельных типов данных
    void loadStops(TransportSystem& system);
    void loadVehicles(TransportSystem& system);
    void loadDrivers(TransportSystem& system);
    void loadRoutes(TransportSystem& system);
    void loadTrips(TransportSystem& system);
    void loadAdminCredentials(TransportSystem& system);
    
    // Вспомогательные методы для загрузки данных из файлов
    int loadVehiclesFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName);
    int loadTripsFromFile(std::ifstream& file, TransportSystem& system, const std::string& fileName);
};

#endif // DATA_MANAGER_H

