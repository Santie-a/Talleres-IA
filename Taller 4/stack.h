#ifndef STACK_H
#define STACK_H

#include <vector>
#include <list>
#include <stdexcept>
#include <initializer_list>

// Clase Stack que permite elegir el contenedor subyacente
template<class T, template<typename, typename...> class Container = std::vector>
class Stack {
private:
    Container<T> stackContainer;   // Contenedor genérico, por defecto std::vector
public:
    /**
     * @brief Constructor por defecto. Crea una pila vacía.
     */
    Stack() = default;

    /**
     * @brief Constructor que inicializa la pila con una lista de inicialización.
     * @param init Lista de inicialización de elementos.
     */
    Stack(std::initializer_list<T> init) : stackContainer(init) {}

    /**
     * @brief Constructor por copia.
     * @param other Otra pila de la cual copiar los elementos.
     */
    Stack(const Stack& other) : stackContainer(other.stackContainer) {}

    /**
     * @brief Constructor por movimiento.
     * @param other Otra pila desde la cual mover los elementos.
     */
    Stack(Stack&& other) noexcept : stackContainer(std::move(other.stackContainer)) {}

    /**
     * @brief Operador de asignación por copia.
     * @param other Otra pila de la cual copiar los elementos.
     * @return Referencia a la pila actual después de la copia.
     */
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            stackContainer = other.stackContainer;
        }
        return *this;
    }

    /**
     * @brief Operador de asignación por movimiento.
     * @param other Otra pila desde la cual mover los elementos.
     * @return Referencia a la pila actual después del movimiento.
     */
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            stackContainer = std::move(other.stackContainer);
        }
        return *this;
    }

    /**
     * @brief Verifica si la pila está vacía.
     * @return true si la pila está vacía, false en caso contrario.
     */
    bool empty() const {
        return stackContainer.empty();
    }

    /**
     * @brief Obtiene el número de elementos en la pila.
     * @return El número de elementos en la pila.
     */
    int size() const {
        return stackContainer.size();
    }

    /**
     * @brief Devuelve una referencia al elemento en la cima de la pila.
     * @return Referencia al último elemento agregado a la pila.
     * @throws std::out_of_range si la pila está vacía.
     */
    T& top() {
        if (empty()) {
            throw std::out_of_range("La pila está vacía. No se puede acceder al elemento en top.");
        }
        return stackContainer.back();
    }

    /**
     * @brief Inserta un nuevo elemento en la cima de la pila.
     * @param value El valor que se va a insertar en la pila.
     */
    void push(const T& value) {
        stackContainer.push_back(value);
    }

    /**
     * @brief Inserta un nuevo elemento movido en la cima de la pila.
     * @param value El valor que se va a mover a la pila.
     */
    void push(T&& value) {
        stackContainer.push_back(std::move(value));
    }

    /**
     * @brief Elimina y devuelve el elemento en la cima de la pila.
     * @return T El valor del elemento eliminado de la cima de la pila.
     * @throws std::out_of_range si la pila está vacía.
     */
    T pop() {
        if (empty()) {
            throw std::out_of_range("La pila está vacía. No se puede eliminar ningún elemento.");
        }
        T value = stackContainer.back();
        stackContainer.pop_back();
        return value;
    }
};

#endif // STACK_H
