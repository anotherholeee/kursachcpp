#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include "list.h"
#include "exceptions.h"

class TransportSystem;

// Базовый класс для команд (реализует паттерн Command)
// Позволяет инкапсулировать запросы как объекты, что дает возможность
// поддерживать операции отмены (undo) и повтора (redo)
class Command {
public:
    virtual ~Command() = default;

    // Выполнить команду
    virtual void execute() = 0;

    // Отменить выполнение команды
    virtual void undo() = 0;

    // Получить описание команды
    virtual std::string getDescription() const = 0;
};

// Класс для управления историей команд
// Хранит историю выполненных команд и позволяет отменять (undo)
// и повторять (redo) операции. Поддерживает ограничение размера истории
class CommandHistory {
private:
    List<std::unique_ptr<Command>> history;  // История команд
    size_t currentIndex = 0;                  // Текущая позиция в истории
    static const size_t MAX_HISTORY_SIZE = 100;  // Максимальный размер истории

public:
    // Выполнить команду и добавить в историю
    void executeCommand(std::unique_ptr<Command> cmd);

    // Проверить, можно ли отменить последнее действие
    bool canUndo() const;

    // Отменить последнее выполненное действие
    void undo();

    // Проверить, можно ли повторить отмененное действие
    bool canRedo() const;

    // Повторить последнее отмененное действие
    void redo();

    // Очистить историю команд
    void clear();

    // Получить описание последней выполненной команды
    std::string getLastCommandDescription() const;

    // Получить описание следующей команды, которую можно повторить
    std::string getNextCommandDescription() const;
};

#endif // COMMAND_H