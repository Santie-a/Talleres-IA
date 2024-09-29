#include <iostream>

template <typename Object>
class List
{
    private:
        // Estructura interna que representa un nodo en la lista
        struct Node
        {
            Object data;  // Dato que almacena el nodo
            Node *prev;   // Puntero al nodo anterior
            Node *next;   // Puntero al siguiente nodo

            // Constructor para un nodo con un dato constante
            Node(const Object &d = Object{}, Node *p = nullptr, Node *n = nullptr):
                data{d}, prev{p}, next{n} {}
            
            // Constructor para un nodo con un dato movido
            Node(Object &&d, Node *p = nullptr, Node *n = nullptr):
                data{std::move(d)}, prev{p}, next{n} {}
        };
    
    public:
        // Clase iterador constante para recorrer la lista
        class const_iterator
        {
            public:
                // Constructor por defecto
                const_iterator() : current{ nullptr} { }

                // Operador de desreferencia para obtener el valor del nodo actual
                const Object &operator*() const
                {
                    return retrieve();
                }

                // Operador de preincremento, avanza al siguiente nodo
                const_iterator &operator++()
                {
                    current = current->next;
                    return *this;
                }

                // Operador de postincremento, avanza al siguiente nodo y devuelve el iterador anterior
                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++(*this);
                    return old;
                }

                // Operador de predecremento, retrocede al nodo anterior
                const_iterator &operator--()
                {
                    current = current->prev;
                    return *this;
                }

                // Operador de postdecremento, retrocede al nodo anterior y devuelve el iterador anterior
                const_iterator operator--(int)
                {
                    const_iterator old = *this;
                    --(*this);
                    return old;
                }

                // Operador de igualdad, compara si dos iteradores son iguales
                bool operator==(const const_iterator &rhs) const
                {
                    return current == rhs.current;
                }

                // Operador de desigualdad, compara si dos iteradores son diferentes
                bool operator!=(const const_iterator &rhs) const
                {
                    return !(*this == rhs);
                }

            protected:
                const List<Object> *theList; // Puntero a la lista asociada
                Node *current;               // Puntero al nodo actual

                // Constructor protegido para crear un iterador en un nodo específico
                const_iterator(const List<Object> &lst, Node *p):
                    theList{ &lst}, current{p} {}
                
                // Método para validar el iterador, lanza una excepción si es inválido
                void assertIsValid() const
                {
                    if (theList == nullptr || current == nullptr || current == theList->head){
                        throw IteratorOutOfBoundsException{};
                    }
                }

                // Método para obtener el dato del nodo actual
                Object &retrieve() const
                {
                    return current->data;
                }

                // Constructor protegido para inicializar el iterador con un nodo
                const_iterator(Node *p) : current{p} {}

                // Amiga de la clase List para permitir el acceso a los miembros privados
                friend class List<Object>;

            // Clase de excepción para iteradores fuera de límites
            class IteratorOutOfBoundsException : public std::exception {
                public:
                    // Mensaje de error para la excepción
                    const char* what() const noexcept override 
                    {
                        return "Iterator is out of bounds";
                    }
                };
        };

        // Clase iterador no constante para recorrer y modificar la lista
        class iterator : public const_iterator
        {
            public:
                // Constructor por defecto
                iterator() {}

                // Operador de desreferencia para obtener y modificar el valor del nodo actual
                Object &operator*()
                {
                    return const_iterator::retrieve();
                }

                // Operador de desreferencia para obtener el valor del nodo actual (versión constante)
                const Object &operator*( ) const
                {
                    return const_iterator::operator*();
                }

                // Operador de preincremento, avanza al siguiente nodo
                iterator & operator++()
                {
                    this->current = this->current->next;
                    return *this;
                }

                // Operador de postincremento, avanza al siguiente nodo y devuelve el iterador anterior
                iterator operator++(int)
                {
                    iterator old = *this;
                    ++(*this);
                    return old;
                }

                // Operador de predecremento, retrocede al nodo anterior
                iterator &operator--()
                {
                    this->current = this->current->prev;
                    return *this;
                }

                // Operador de postdecremento, retrocede al nodo anterior y devuelve el iterador anterior
                iterator operator--(int)
                {
                    iterator old = *this;
                    --(*this);
                    return old;
                }

            protected:
                // Constructor protegido para inicializar el iterador con un nodo
                iterator(Node *p) : const_iterator{p} {}

                // Amiga de la clase List para permitir el acceso a los miembros privados
                friend class List<Object>;
        };

    public:
        // Constructor por defecto, inicializa la lista vacía
        List()
        {  
            init();
        }

        // Constructor por copia, crea una nueva lista copiando otra
        List(const List &rhs)
        {
            init();
            for (auto &x:rhs)
            {
                push_back (x);
            }
        }

        // Destructor, libera la memoria ocupada por los nodos de la lista
        ~List()
        {
            clear();
            delete head;
            delete tail;
        }

        // Operador de asignación por copia
        List &operator=(const List &rhs)
        {
            List copy = rhs;
            std::swap( *this, copy);
            return *this;
        }

        // Constructor por movimiento, transfiere los recursos de otra lista
        List(List &&rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
        {
            rhs.theSize = 0;
            rhs.head = nullptr;
            rhs.tail = nullptr;
        }

        // Operador de asignación por movimiento
        List &operator=(List &&rhs)
        {
            std::swap(theSize, rhs.theSize);
            std::swap(head, rhs.head);
            std::swap(tail, rhs.tail);

            return *this;
        }

        // Devuelve un iterador al primer elemento de la lista
        iterator begin()
        {
            return {head->next};
        }

        // Devuelve un iterador constante al primer elemento de la lista
        const_iterator begin() const
        {
            return {head->next};
        }

        // Devuelve un iterador al final de la lista (nodo ficticio tail)
        iterator end()
        {
            return {tail};
        }

        // Devuelve un iterador constante al final de la lista (nodo ficticio tail)
        const_iterator end() const
        {
            return {tail};
        }

        // Devuelve el número de elementos en la lista
        int size() const
        {
            return theSize;
        }

        // Comprueba si la lista está vacía
        bool empty() const
        {
            return size() == 0;
        }

        // Borra todos los elementos de la lista
        void clear()
        {
            while (!empty())
            {
                pop_front();
            }
        }

        // Devuelve una referencia al primer elemento de la lista
        Object &front()
        {
            return *begin();
        }

        // Devuelve una referencia constante al primer elemento de la lista
        const Object &front() const
        {
            return *begin();
        }

        // Devuelve una referencia al último elemento de la lista
        Object &back()
        {
            return *--end();
        }

        // Devuelve una referencia constante al último elemento de la lista
        const Object &back() const
        {
            return *--end();
        }

        // Inserta un elemento al inicio de la lista
        void push_front(const Object &x)
        {
            insert(begin(), x);
        }

        // Inserta un elemento movido al inicio de la lista
        void push_front(Object &&x)
        {
            insert(begin(), std::move(x));
        }

        // Inserta un elemento al final de la lista
        void push_back(const Object &x)
        {
            insert(end(), x);
        }

        // Inserta un elemento movido al final de la lista
        void push_back(Object &&x)
        {
            insert(end(), std::move(x));
        }

        // Elimina el primer elemento de la lista
        void pop_front()
        {
            erase(begin());
        }

        // Elimina el último elemento de la lista
        void pop_back()
        {
            erase(--end());
        }

        // Inserta un elemento antes del iterador dado
        iterator insert(iterator itr, const Object &x)
        {
            Node *p = itr.current;  // Nodo actual
            theSize++;
            return {p->prev = p->prev->next = new Node{x, p->prev, p}};
        }

        // Inserta un elemento movido antes del iterador dado
        iterator insert(iterator itr, Object &&x)
        {
            Node *p = itr.current;  // Nodo actual
            theSize++;
            return {p->prev = p->prev->next = new Node{std::move(x), p->prev, p}};
        }

        // Elimina el nodo apuntado por el iterador dado y devuelve el siguiente
        iterator erase(iterator itr)
        {
            Node *p = itr.current;  // Nodo actual
            iterator retVal{p->next};  // Iterador al siguiente nodo
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            theSize--;

            return retVal;
        }

        // Elimina los nodos en el rango [from, to)
        iterator erase(iterator from, iterator to)
        {
            for (iterator itr = from; itr != to; )
            {
                itr = erase(itr);
            }
            return to;
        }
        
    private:
        int theSize;  // Número de elementos en la lista
        Node *head;   // Nodo ficticio de inicio
        Node* tail;   // Nodo ficticio de final

        // Inicializa la lista con nodos ficticios de inicio y final
        void init()
        {
            theSize = 0;
            head = new Node;
            tail = new Node;
            head->next = tail;
            tail->prev = head;
        }
};
