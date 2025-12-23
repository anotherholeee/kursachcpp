#include "exceptions.h"

// Конструктор базового исключения транспортной системы
TransportException::TransportException(const std::string& msg) : message(msg) {}

// Возвращает сообщение об ошибке (стандартный метод std::exception)
const char* TransportException::what() const noexcept {
    return message.c_str();
}

// Конструктор исключения для ошибок ввода данных
InputException::InputException(const std::string& msg) : TransportException(msg) {}

// Конструктор исключения для ошибок работы с файлами (с именем файла и операцией)
FileException::FileException(const std::string& filename, const std::string& operation)
    : TransportException("Ошибка работы с файлом '" + filename + "' (" + operation + ")") {}

// Конструктор исключения для ошибок работы с файлами (с сообщением)
FileException::FileException(const std::string& msg) : TransportException("Ошибка файла: " + msg) {}

// Конструктор исключения для ошибок работы с контейнерами
ContainerException::ContainerException(const std::string& msg) : TransportException("Ошибка контейнера: " + msg) {}

