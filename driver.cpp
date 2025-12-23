#include "driver.h"
#include <sstream>

// Конструктор водителя: инициализирует ФИО и категорию водительских прав
Driver::Driver(const std::string& fname, const std::string& lname, const std::string& mname, const std::string& cat)
    : firstName(fname), lastName(lname), middleName(mname), category(cat) {}

// Возвращает полное имя в формате "Фамилия Имя Отчество"
std::string Driver::getFullName() const {
    return lastName + " " + firstName + (middleName.empty() ? "" : " " + middleName);
}

// Возвращает имя водителя
std::string Driver::getFirstName() const {
    return firstName;
}

// Возвращает фамилию водителя
std::string Driver::getLastName() const {
    return lastName;
}

// Возвращает отчество водителя
std::string Driver::getMiddleName() const {
    return middleName;
}

// Возвращает категорию водительских прав (D, T и т.д.)
std::string Driver::getCategory() const {
    return category;
}

// Устанавливает категорию водительских прав
void Driver::setCategory(const std::string& cat) {
    category = cat;
}

// Сравнение водителей по ФИО
bool Driver::operator==(const Driver& other) const {
    return firstName == other.firstName &&
           lastName == other.lastName &&
           middleName == other.middleName;
}

// Сериализация водителя в строку формата "firstName|lastName|middleName|category"
std::string Driver::serialize() const {
    return firstName + "|" + lastName + "|" + middleName + "|" + category;
}

// Десериализация водителя из строки формата "firstName|lastName|middleName|category"
std::shared_ptr<Driver> Driver::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string firstName, lastName, middleName, category;
    std::getline(ss, firstName, '|');   // Читаем имя
    std::getline(ss, lastName, '|');    // Читаем фамилию
    std::getline(ss, middleName, '|');  // Читаем отчество
    std::getline(ss, category);         // Читаем категорию
    // Если категория не указана в старых данных, оставляем пустой строкой
    return std::make_shared<Driver>(firstName, lastName, middleName, category);
}

