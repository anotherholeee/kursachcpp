#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <memory>
#include <sstream>

/**
 * @class Driver
 * @brief Класс, представляющий водителя общественного транспорта
 * 
 * Хранит информацию о водителе: ФИО и категорию водительских прав.
 * Поддерживает сериализацию для сохранения и загрузки данных.
 */
class Driver {
private:
    std::string firstName;   // Имя водителя
    std::string lastName;     // Фамилия водителя
    std::string middleName;   // Отчество водителя
    std::string category;     // Категория водительских прав (D, T, и т.д.)

public:
    /**
     * @brief Конструктор водителя
     * @param fname Имя
     * @param lname Фамилия
     * @param mname Отчество (по умолчанию пустая строка)
     * @param cat Категория водительских прав (по умолчанию пустая строка)
     */
    Driver(const std::string& fname, const std::string& lname, const std::string& mname = "", const std::string& cat = "");

    /**
     * @brief Получить полное имя в формате "Фамилия Имя Отчество"
     */
    std::string getFullName() const;

    // Геттеры
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getMiddleName() const;
    std::string getCategory() const;

    /**
     * @brief Установить категорию водительских прав
     * @param cat Категория водительских прав
     */
    void setCategory(const std::string& cat);

    /**
     * @brief Оператор сравнения водителей по ФИО
     * @param other Другой водитель для сравнения
     * @return true, если водители имеют одинаковое ФИО
     */
    bool operator==(const Driver& other) const;

    // Сериализация
    /**
     * @brief Сериализация водителя в строку формата "firstName|lastName|middleName|category"
     */
    std::string serialize() const;

    /**
     * @brief Десериализация водителя из строки формата "firstName|lastName|middleName|category"
     * @param data Строка с данными водителя
     * @return Указатель на созданного водителя
     */
    static std::shared_ptr<Driver> deserialize(const std::string& data);
};

#endif // DRIVER_H

