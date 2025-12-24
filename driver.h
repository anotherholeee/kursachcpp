#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <memory>
#include <sstream>

// Класс, представляющий водителя общественного транспорта
// Хранит информацию о водителе: ФИО и категорию водительских прав.
// Поддерживает сериализацию для сохранения и загрузки данных
class Driver {
private:
    std::string firstName;   // Имя водителя
    std::string lastName;     // Фамилия водителя
    std::string middleName;   // Отчество водителя
    std::string category;     // Категория водительских прав (D, T, и т.д.)

public:
    // Конструктор водителя
    Driver(const std::string& fname, const std::string& lname, const std::string& mname = "", const std::string& cat = "");

    // Получить полное имя в формате "Фамилия Имя Отчество"
    std::string getFullName() const;

    // Геттеры
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getMiddleName() const;
    std::string getCategory() const;

    // Установить категорию водительских прав
    void setCategory(const std::string& cat);

    // Оператор сравнения водителей по ФИО
    bool operator==(const Driver& other) const;

    // Сериализация водителя в строку формата "firstName|lastName|middleName|category"
    std::string serialize() const;

    // Десериализация водителя из строки формата "firstName|lastName|middleName|category"
    static std::shared_ptr<Driver> deserialize(const std::string& data);
};

#endif // DRIVER_H

