#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

#include <list>
#include <stdexcept>
#include <initializer_list>

// Clase Queue que utiliza std::list como contenedor subyacente
template<class T>
class Queue {
private:
    std::list<T> queueContainer;   // Contenedor std::list para almacenar los elementos de la cola

public:
    /**
     * @brief Constructor por defecto. Crea una cola vacía.
     */
    Queue() = default;

    /**
     * @brief Constructor que inicializa la cola con una lista de inicialización.
     * @param init Lista de inicialización de elementos.
     */
    Queue(std::initializer_list<T> init) : queueContainer(init) {}

    /**
     * @brief Constructor por copia.
     * @param other Otra cola de la cual copiar los elementos.
     */
    Queue(const Queue& other) : queueContainer(other.queueContainer) {}

    /**
     * @brief Constructor por movimiento.
     * @param other Otra cola desde la cual mover los elementos.
     */
    Queue(Queue&& other) noexcept : queueContainer(std::move(other.queueContainer)) {}

    /**
     * @brief Operador de asignación por copia.
     * @param other Otra cola de la cual copiar los elementos.
     * @return Referencia a la cola actual después de la copia.
     */
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            queueContainer = other.queueContainer;
        }
        return *this;
    }

    /**
     * @brief Operador de asignación por movimiento.
     * @param other Otra cola desde la cual mover los elementos.
     * @return Referencia a la cola actual después del movimiento.
     */
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            queueContainer = std::move(other.queueContainer);
        }
        return *this;
    }

    /**
     * @brief Verifica si la cola está vacía.
     * @return true si la cola está vacía, false en caso contrario.
     */
    bool empty() const {
        return queueContainer.empty();
    }

    /**
     * @brief Obtiene el número de elementos en la cola.
     * @return El número de elementos en la cola.
     */
    int size() const {
        return queueContainer.size();
    }

    /**
     * @brief Devuelve una referencia al elemento en el frente de la cola.
     * @return Referencia al primer elemento de la cola.
     * @throws std::out_of_range si la cola está vacía.
     */
    T& front() {
        if (empty()) {
            throw std::out_of_range("La cola está vacía. No se puede acceder al elemento en front.");
        }
        return queueContainer.front();
    }

    /**
     * @brief Devuelve una referencia al elemento en la parte trasera de la cola.
     * @return Referencia al último elemento de la cola.
     * @throws std::out_of_range si la cola está vacía.
     */
    T& back() {
        if (empty()) {
            throw std::out_of_range("La cola está vacía. No se puede acceder al elemento en back.");
        }
        return queueContainer.back();
    }

    /**
     * @brief Inserta un nuevo elemento en la parte trasera de la cola.
     * @param value El valor que se va a insertar en la cola.
     */
    void enqueue(const T& value) {
        queueContainer.push_back(value);
    }

    /**
     * @brief Inserta un nuevo elemento movido en la parte trasera de la cola.
     * @param value El valor que se va a mover a la cola.
     */
    void enqueue(T&& value) {
        queueContainer.push_back(std::move(value));
    }

    /**
     * @brief Elimina y devuelve el elemento en la parte frontal de la cola.
     * @return T El valor del elemento eliminado de la cola.
     * @throws std::out_of_range si la cola está vacía.
     */
    T dequeue() {
        if (empty()) {
            throw std::out_of_range("La cola está vacía. No se puede eliminar ningún elemento.");
        }
        T value = queueContainer.front();
        queueContainer.pop_front();
        return value;
    }
};

#endif // QUEUE_LIST_H
