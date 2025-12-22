#ifndef LIST_H
#define LIST_H

#include <iterator>
#include <stdexcept>
#include <list>

// Самописный контейнер - двусвязный список (аналог std::list)
template<typename T>
class List {
private:
    // Узел двусвязного списка
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size_;

    void clearNodes() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size_ = 0;
    }

    Node* getNodeAt(size_t index) const {
        if (index >= size_) {
            return nullptr;
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

public:
    // Итератор для List (bidirectional iterator)
    class Iterator {
    private:
        Node* node;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* n) : node(n) {}

        T& operator*() { return node->data; }
        const T& operator*() const { return node->data; }
        T* operator->() { return &(node->data); }
        const T* operator->() const { return &(node->data); }

        Iterator& operator++() {
            if (node != nullptr) {
                node = node->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            if (node != nullptr) {
                node = node->next;
            }
            return tmp;
        }

        Iterator& operator--() {
            if (node != nullptr) {
                node = node->prev;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            if (node != nullptr) {
                node = node->prev;
            }
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return node == other.node;
        }

        bool operator!=(const Iterator& other) const {
            return node != other.node;
        }

        // Метод для доступа к узлу (используется в erase)
        Node* getNode() const { return node; }

        friend class List;
    };

    List();
    List(const List& other);
    template<typename InputIt>
    List(InputIt first, InputIt last);
    List& operator=(const List& other);
    ~List();

    void push_back(const T& value);
    void push_back(T&& value);
    template<typename... Args>
    void emplace_back(Args&&... args);
    Iterator erase(const Iterator& it);
    void erase(const Iterator& first, const Iterator& last);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_t size() const;
    bool empty() const;
    void clear();

    template<typename Compare>
    void sort(Compare comp);
    
    void reverse();

    Iterator begin();
    Iterator end();
    Iterator begin() const;
    Iterator end() const;
};

// Реализация шаблонного класса
#include "list.tpp"

#endif // LIST_H

