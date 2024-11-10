#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <iostream>
#include <stdexcept>
#include <algorithm> // Para std::max

/**
 * @brief Clase AvlTree que representa un árbol AVL.
 * 
 * Un árbol AVL es un árbol binario de búsqueda balanceado. La diferencia
 * de altura entre los subárboles izquierdo y derecho de cualquier nodo no
 * debe ser mayor que 1. Esta estructura permite realizar operaciones de 
 * inserción, eliminación y búsqueda en tiempo O(log N).
 */
template <typename T>
class AvlTree {
public:
    AvlTree();
    AvlTree(const AvlTree &rhs);
    AvlTree(AvlTree &&rhs);
    ~AvlTree();

    const T &findMin() const;
    const T &findMax() const;
    bool contains(const T &x) const;
    bool isEmpty() const;
    void printTree(std::ostream &out = std::cout) const;

    void makeEmpty();
    void insert(const T &x);
    void insert(T &&x);
    void remove(const T &x);

    AvlTree &operator=(const AvlTree &rhs);
    AvlTree &operator=(AvlTree &&rhs);

protected:
    /**
     * @brief Estructura AvlNode que representa un nodo en el árbol AVL.
     */
    struct AvlNode {
        T element;         ///< Elemento almacenado en el nodo
        AvlNode *left;     ///< Puntero al hijo izquierdo
        AvlNode *right;    ///< Puntero al hijo derecho
        int height;        ///< Altura del nodo para facilitar el balance

        AvlNode(const T &theElement, AvlNode *lt, AvlNode *rt, int h = 0)
            : element{theElement}, left{lt}, right{rt}, height{h} {}

        AvlNode(T &&theElement, AvlNode *lt, AvlNode *rt, int h = 0)
            : element{std::move(theElement)}, left{lt}, right{rt}, height{h} {}
    };

    AvlNode *root; ///< Raíz del árbol AVL

private:
    // Métodos auxiliares privados para la funcionalidad del árbol
    void insert(const T &x, AvlNode *&t);
    void insert(T &&x, AvlNode *&t);
    void remove(const T &x, AvlNode *&t);
    AvlNode *findMinNode(AvlNode *t) const;
    AvlNode *findMaxNode(AvlNode *t) const;
    bool contains(const T &x, AvlNode *t) const;
    void makeEmpty(AvlNode *&t);
    AvlNode *clone(AvlNode *t) const;

    int height(AvlNode *t) const;
    void balance(AvlNode *&t);

    void rotateWithLeftChild(AvlNode *&k2);
    void rotateWithRightChild(AvlNode *&k2);
    void doubleWithLeftChild(AvlNode *&k3);
    void doubleWithRightChild(AvlNode *&k3);

    static const int ALLOWED_IMBALANCE = 1; ///< Desbalance permitido en los subárboles

    AvlNode *getRoot() const;
};

// Implementación de métodos públicos

/**
 * @brief Constructor por defecto. Inicializa el árbol vacío.
 */
template <typename T>
AvlTree<T>::AvlTree() : root(nullptr) {}

/**
 * @brief Constructor de copia.
 * @param rhs Árbol AVL a copiar.
 */
template <typename T>
AvlTree<T>::AvlTree(const AvlTree &rhs) : root(nullptr) {
    root = clone(rhs.root);
}

/**
 * @brief Constructor de movimiento.
 * @param rhs Árbol AVL a mover.
 */
template <typename T>
AvlTree<T>::AvlTree(AvlTree &&rhs) : root(rhs.root) {
    rhs.root = nullptr;
}

/**
 * @brief Destructor. Libera toda la memoria de los nodos del árbol.
 */
template <typename T>
AvlTree<T>::~AvlTree() {
    makeEmpty();
}

/**
 * @brief Encuentra el nodo con el elemento máximo en el subárbol de `t`.
 * @param t Puntero al nodo raíz del subárbol.
 * @return Puntero al nodo con el elemento máximo.
 */
template <typename T>
typename AvlTree<T>::AvlNode *AvlTree<T>::getRoot() const {
    return root;
}

/**
 * @brief Retorna la altura de un nodo o -1 si es nulo.
 * @param t Puntero al nodo cuya altura se desea obtener.
 * @return Altura del nodo.
 */
template <typename T>
int AvlTree<T>::height(AvlNode *t) const {
    return t == nullptr ? -1 : t->height;
}

/**
 * @brief Balancea el subárbol apuntado por el nodo `t`.
 * 
 * Verifica si el nodo está desbalanceado y aplica rotaciones si es necesario
 * para restaurar el balance AVL.
 * @param t Puntero de referencia al nodo a balancear.
 */
template <typename T>
void AvlTree<T>::balance(AvlNode *&t) {
    if (t == nullptr) return;

    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    } else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
        if (height(t->right->right) >= height(t->right->left))
            rotateWithRightChild(t);
        else
            doubleWithRightChild(t);
    }

    t->height = std::max(height(t->left), height(t->right)) + 1;
}

/**
 * @brief Rotación simple con hijo izquierdo.
 * Corrige un desbalance en el caso izquierdo-izquierdo.
 * @param k2 Puntero de referencia al nodo desbalanceado.
 */
template <typename T>
void AvlTree<T>::rotateWithLeftChild(AvlNode *&k2) {
    AvlNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = std::max(height(k2->left), height(k2->right)) + 1;
    k1->height = std::max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

/**
 * @brief Rotación simple con hijo derecho.
 * Corrige un desbalance en el caso derecho-derecho.
 * @param k2 Puntero de referencia al nodo desbalanceado.
 */
template <typename T>
void AvlTree<T>::rotateWithRightChild(AvlNode *&k2) {
    AvlNode *k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = std::max(height(k2->left), height(k2->right)) + 1;
    k1->height = std::max(height(k1->right), k2->height) + 1;
    k2 = k1;
}

/**
 * @brief Rotación doble con hijo izquierdo.
 * Corrige un desbalance en el caso izquierdo-derecho.
 * @param k3 Puntero de referencia al nodo desbalanceado.
 */
template <typename T>
void AvlTree<T>::doubleWithLeftChild(AvlNode *&k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

/**
 * @brief Rotación doble con hijo derecho.
 * Corrige un desbalance en el caso derecho-izquierdo.
 * @param k3 Puntero de referencia al nodo desbalanceado.
 */
template <typename T>
void AvlTree<T>::doubleWithRightChild(AvlNode *&k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
}

/**
 * @brief Inserta un elemento en el árbol AVL.
 * @param x Elemento a insertar.
 */
template <typename T>
void AvlTree<T>::insert(const T &x) {
    insert(x, root);
}

/**
 * @brief Inserta un elemento en el árbol AVL usando movimiento.
 * @param x Elemento a insertar.
 */
template <typename T>
void AvlTree<T>::insert(T &&x) {
    insert(std::move(x), root);
}

/**
 * @brief Inserta un elemento en el subárbol apuntado por `t`.
 * Aplica el balanceo si es necesario después de la inserción.
 * @param x Elemento a insertar.
 * @param t Puntero de referencia al nodo raíz del subárbol.
 */
template <typename T>
void AvlTree<T>::insert(const T &x, AvlNode *&t) {
    if (t == nullptr)
        t = new AvlNode{x, nullptr, nullptr};
    else if (x < t->element)
        insert(x, t->left);
    else if (t->element < x)
        insert(x, t->right);

    balance(t);
}

/**
 * @brief Inserta un elemento en el subárbol usando movimiento.
 * Aplica el balanceo si es necesario después de la inserción.
 * @param x Elemento a insertar.
 * @param t Puntero de referencia al nodo raíz del subárbol.
 */
template <typename T>
void AvlTree<T>::insert(T &&x, AvlNode *&t) {
    if (t == nullptr)
        t = new AvlNode{std::move(x), nullptr, nullptr};
    else if (x < t->element)
        insert(std::move(x), t->left);
    else if (t->element < x)
        insert(std::move(x), t->right);

    balance(t);
}

/**
 * @brief Elimina un elemento del árbol AVL.
 * @param x Elemento a eliminar.
 */
template <typename T>
void AvlTree<T>::remove(const T &x) {
    remove(x, root);
}

/**
 * @brief Elimina un elemento del subárbol apuntado por `t`.
 * Aplica el balanceo si es necesario después de la eliminación.
 * @param x Elemento a eliminar.
 * @param t Puntero de referencia al nodo raíz del subárbol.
 */
template <typename T>
void AvlTree<T>::remove(const T &x, AvlNode *&t) {
    if (t == nullptr) return;

    if (x < t->element)
        remove(x, t->left);
    else if (t->element < x)
        remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) {
        t->element = findMinNode(t->right)->element;
        remove(t->element, t->right);
    } else {
        AvlNode *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }

    balance(t);
}

/**
 * @brief Encuentra el mínimo elemento en el árbol AVL.
 * @return Referencia constante al elemento mínimo.
 * @throws std::runtime_error si el árbol está vacío.
 */
template <typename T>
const T &AvlTree<T>::findMin() const {
    if (isEmpty())
        throw std::runtime_error("El árbol está vacío");
    return findMinNode(root)->element;
}

/**
 * @brief Encuentra el máximo elemento en el árbol AVL.
 * @return Referencia constante al elemento máximo.
 * @throws std::runtime_error si el árbol está vacío.
 */
template <typename T>
const T &AvlTree<T>::findMax() const {
    if (isEmpty())
        throw std::runtime_error("El árbol está vacío");
    return findMaxNode(root)->element;
}

/**
 * @brief Encuentra el nodo con el elemento mínimo en el subárbol de `t`.
 * @param t Puntero al nodo raíz del subárbol.
 * @return Puntero al nodo con el elemento mínimo.
 */
template <typename T>
typename AvlTree<T>::AvlNode *AvlTree<T>::findMinNode(AvlNode *t) const {
    if (t == nullptr) return nullptr;
    while (t->left != nullptr) t = t->left;
    return t;
}

/**
 * @brief Encuentra el nodo con el elemento máximo en el subárbol de `t`.
 * @param t Puntero al nodo raíz del subárbol.
 * @return Puntero al nodo con el elemento máximo.
 */
template <typename T>
typename AvlTree<T>::AvlNode *AvlTree<T>::findMaxNode(AvlNode *t) const {
    if (t == nullptr) return nullptr;
    while (t->right != nullptr) t = t->right;
    return t;
}

/**
 * @brief Verifica si el árbol AVL contiene un elemento.
 * @param x Elemento a buscar.
 * @return `true` si el elemento está en el árbol, `false` en caso contrario.
 */
template <typename T>
bool AvlTree<T>::contains(const T &x) const {
    return contains(x, root);
}

/**
 * @brief Verifica si el subárbol contiene un elemento.
 * @param x Elemento a buscar.
 * @param t Puntero al nodo raíz del subárbol.
 * @return `true` si el elemento está en el subárbol, `false` en caso contrario.
 */
template <typename T>
bool AvlTree<T>::contains(const T &x, AvlNode *t) const {
    if (t == nullptr) return false;
    else if (x < t->element) return contains(x, t->left);
    else if (t->element < x) return contains(x, t->right);
    else return true;
}

/**
 * @brief Verifica si el árbol AVL está vacío.
 * @return `true` si el árbol está vacío, `false` en caso contrario.
 */
template <typename T>
bool AvlTree<T>::isEmpty() const {
    return root == nullptr;
}

/**
 * @brief Vacía el árbol AVL, eliminando todos sus elementos.
 */
template <typename T>
void AvlTree<T>::makeEmpty() {
    makeEmpty(root);
}

/**
 * @brief Vacía el subárbol apuntado por `t`, eliminando todos sus nodos.
 * @param t Puntero de referencia al nodo raíz del subárbol.
 */
template <typename T>
void AvlTree<T>::makeEmpty(AvlNode *&t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

/**
 * @brief Crea una copia del subárbol apuntado por `t`.
 * @param t Puntero al nodo raíz del subárbol a clonar.
 * @return Puntero al nodo raíz del nuevo subárbol clonado.
 */
template <typename T>
typename AvlTree<T>::AvlNode *AvlTree<T>::clone(AvlNode *t) const {
    if (t == nullptr) return nullptr;
    return new AvlNode{t->element, clone(t->left), clone(t->right), t->height};
}

/**
 * @brief Operador de asignación de copia.
 * Realiza una copia profunda del árbol AVL `rhs` en el árbol actual.
 * @param rhs El árbol AVL a copiar.
 * @return Referencia al árbol actual.
 */
template <typename T>
AvlTree<T> &AvlTree<T>::operator=(const AvlTree &rhs) {
    if (this != &rhs) {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

/**
 * @brief Operador de asignación de movimiento.
 * Transfiere la propiedad de los recursos del árbol `rhs` al árbol actual.
 * @param rhs El árbol AVL a mover.
 * @return Referencia al árbol actual.
 */
template <typename T>
AvlTree<T> &AvlTree<T>::operator=(AvlTree &&rhs) {
    if (this != &rhs) {
        makeEmpty();
        root = rhs.root;
        rhs.root = nullptr;
    }
    return *this;
}

#endif // _AVL_TREE_H
