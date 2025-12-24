#include "command.h"

// Выполняет команду и добавляет её в историю
// При выполнении новой команды удаляются все команды после текущей позиции
// (отменяет возможность redo). Если история превышает максимальный размер,
// удаляется самая старая команда
void CommandHistory::executeCommand(std::unique_ptr<Command> cmd) {
    // Удаляем элементы от currentIndex до конца (отменяем возможность redo)
    auto it = history.begin();
    for (size_t i = 0; i < currentIndex && it != history.end(); ++i, ++it) {}
    history.erase(it, history.end());
    
    // Выполняем команду и добавляем в историю
    cmd->execute();
    history.push_back(std::move(cmd));
    currentIndex = history.size();

    // Ограничиваем размер истории
    if (history.size() > MAX_HISTORY_SIZE) {
        history.erase(history.begin());
        currentIndex--;
    }
}

// Проверяет, можно ли отменить последнее действие
bool CommandHistory::canUndo() const {
    return currentIndex > 0;
}

// Отменяет последнее выполненное действие
void CommandHistory::undo() {
    if (!canUndo()) {
        throw ContainerException("Нет действий для отмены");
    }
    currentIndex--;
    history[currentIndex]->undo();  // Вызываем метод undo команды
}

// Проверяет, можно ли повторить отмененное действие
bool CommandHistory::canRedo() const {
    return currentIndex < history.size();
}

// Повторяет последнее отмененное действие
void CommandHistory::redo() {
    if (!canRedo()) {
        throw ContainerException("Нет действий для повтора");
    }
    history[currentIndex]->execute();  // Выполняем команду снова
    currentIndex++;
}

// Очищает историю команд
void CommandHistory::clear() {
    history.clear();
    currentIndex = 0;
}

// Возвращает описание последней выполненной команды
std::string CommandHistory::getLastCommandDescription() const {
    if (currentIndex > 0) {
        return history[currentIndex - 1]->getDescription();
    }
    return "Нет выполненных команд";
}

// Возвращает описание следующей команды, которую можно повторить
std::string CommandHistory::getNextCommandDescription() const {
    if (currentIndex < history.size()) {
        return history[currentIndex]->getDescription();
    }
    return "Нет действий для повтора";
}