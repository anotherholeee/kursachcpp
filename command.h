#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include "list.h"
#include "exceptions.h"

class TransportSystem;

/**
 * @class Command
 * @brief Базовый класс для команд (реализует паттерн Command)
 * 
 * Позволяет инкапсулировать запросы как объекты, что дает возможность
 * поддерживать операции отмены (undo) и повтора (redo).
 */
class Command {
public:
    virtual ~Command() = default;

    /**
     * @brief Выполнить команду
     */
    virtual void execute() = 0;

    /**
     * @brief Отменить выполнение команды
     */
    virtual void undo() = 0;

    /**
     * @brief Получить описание команды
     * @return Текстовое описание команды
     */
    virtual std::string getDescription() const = 0;
};

/**
 * @class CommandHistory
 * @brief Класс для управления историей команд
 * 
 * Хранит историю выполненных команд и позволяет отменять (undo)
 * и повторять (redo) операции. Поддерживает ограничение размера истории.
 */
class CommandHistory {
private:
    List<std::unique_ptr<Command>> history;  // История команд
    size_t currentIndex = 0;                  // Текущая позиция в истории
    static const size_t MAX_HISTORY_SIZE = 100;  // Максимальный размер истории

public:
    /**
     * @brief Выполнить команду и добавить в историю
     * @param cmd Уникальный указатель на команду
     */
    void executeCommand(std::unique_ptr<Command> cmd);

    /**
     * @brief Проверить, можно ли отменить последнее действие
     * @return true, если есть команды для отмены
     */
    bool canUndo() const;

    /**
     * @brief Отменить последнее выполненное действие
     */
    void undo();

    /**
     * @brief Проверить, можно ли повторить отмененное действие
     * @return true, если есть команды для повтора
     */
    bool canRedo() const;

    /**
     * @brief Повторить последнее отмененное действие
     */
    void redo();

    /**
     * @brief Очистить историю команд
     */
    void clear();

    /**
     * @brief Получить описание последней выполненной команды
     * @return Описание команды или сообщение об отсутствии команд
     */
    std::string getLastCommandDescription() const;

    /**
     * @brief Получить описание следующей команды, которую можно повторить
     * @return Описание команды или сообщение об отсутствии команд для повтора
     */
    std::string getNextCommandDescription() const;
};

#endif // COMMAND_H