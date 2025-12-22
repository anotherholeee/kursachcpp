#ifndef LIST_TPP
#define LIST_TPP

// Этот файл должен включаться только из list.h
#ifndef LIST_H
#error "list.tpp should only be included from list.h"
#endif

template<typename T>
List<T>::List() : head(nullptr), tail(nullptr), size_(0) {}

template<typename T>
List<T>::List(const List& other) : head(nullptr), tail(nullptr), size_(0) {
    Node* current = other.head;
    while (current != nullptr) {
        push_back(current->data);
        current = current->next;
    }
}

template<typename T>
template<typename InputIt>
List<T>::List(InputIt first, InputIt last) : head(nullptr), tail(nullptr), size_(0) {
    for (InputIt it = first; it != last; ++it) {
        push_back(*it);
    }
}

template<typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        clearNodes();
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }
    return *this;
}

template<typename T>
List<T>::~List() {
    clearNodes();
}

template<typename T>
void List<T>::push_back(const T& value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    ++size_;
}

template<typename T>
void List<T>::push_back(T&& value) {
    Node* newNode = new Node(std::move(value));
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    ++size_;
}

template<typename T>
template<typename... Args>
void List<T>::emplace_back(Args&&... args) {
    Node* newNode = new Node(T(std::forward<Args>(args)...));
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    ++size_;
}

template<typename T>
typename List<T>::Iterator List<T>::erase(const Iterator& it) {
    Node* nodeToDelete = it.getNode();
    if (nodeToDelete == nullptr) {
        return Iterator(nullptr);
    }

    // Сохраняем следующий узел перед удалением
    Node* nextNode = nodeToDelete->next;

    // Обновляем связи
    if (nodeToDelete->prev != nullptr) {
        nodeToDelete->prev->next = nodeToDelete->next;
    } else {
        // Удаляем голову
        head = nodeToDelete->next;
    }

    if (nodeToDelete->next != nullptr) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    } else {
        // Удаляем хвост
        tail = nodeToDelete->prev;
    }

    delete nodeToDelete;
    --size_;

    return Iterator(nextNode);
}

template<typename T>
void List<T>::erase(const Iterator& first, const Iterator& last) {
    Iterator it = first;
    while (it != last && it != end()) {
        Iterator next = it;
        ++next;
        erase(it);
        it = next;
    }
}

template<typename T>
T& List<T>::operator[](size_t index) {
    Node* node = getNodeAt(index);
    if (node == nullptr) {
        throw std::out_of_range("List index out of range");
    }
    return node->data;
}

template<typename T>
const T& List<T>::operator[](size_t index) const {
    Node* node = getNodeAt(index);
    if (node == nullptr) {
        throw std::out_of_range("List index out of range");
    }
    return node->data;
}

template<typename T>
T& List<T>::front() {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return head->data;
}

template<typename T>
const T& List<T>::front() const {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return head->data;
}

template<typename T>
T& List<T>::back() {
    if (tail == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return tail->data;
}

template<typename T>
const T& List<T>::back() const {
    if (tail == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return tail->data;
}

template<typename T>
size_t List<T>::size() const {
    return size_;
}

template<typename T>
bool List<T>::empty() const {
    return size_ == 0;
}

template<typename T>
void List<T>::clear() {
    clearNodes();
}

template<typename T>
template<typename Compare>
void List<T>::sort(Compare comp) {
    if (size_ <= 1) return;
    
    // Копируем элементы в std::list для сортировки
    std::list<T> tempList;
    Node* current = head;
    while (current != nullptr) {
        tempList.push_back(current->data);
        current = current->next;
    }
    
    // Сортируем
    tempList.sort(comp);
    
    // Очищаем текущий список
    clearNodes();
    
    // Копируем отсортированные элементы обратно
    for (const auto& item : tempList) {
        push_back(item);
    }
}

template<typename T>
void List<T>::reverse() {
    if (size_ <= 1) return;
    
    // Меняем местами указатели prev и next для всех узлов
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
    }
    
    // Меняем местами head и tail
    Node* temp = head;
    head = tail;
    tail = temp;
}

template<typename T>
typename List<T>::Iterator List<T>::begin() {
    return Iterator(head);
}

template<typename T>
typename List<T>::Iterator List<T>::end() {
    return Iterator(nullptr);
}

template<typename T>
typename List<T>::Iterator List<T>::begin() const {
    return Iterator(head);
}

template<typename T>
typename List<T>::Iterator List<T>::end() const {
    return Iterator(nullptr);
}

#endif // LIST_TPP

