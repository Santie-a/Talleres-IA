#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Para std::find
#include "queue_list.h"


// Función helper para imprimir la Queue (copia)
void print_queue(Queue<char> q) {
    while (!q.empty()) {
        std::cout << q.dequeue() << " ";
    }
    std::cout << std::endl;
}

// Función helper para convertir un string a una Queue
Queue<char> make_queue_from_binary(const std::string &bin_str) {
    Queue<char> q;
    for (char c : bin_str) {
        if (c == '0' || c == '1') {
            q.enqueue(c);
        } else {
            throw std::invalid_argument("Invalid binary number");
        }
    }
    return q;
}

// Función helper para convertir una Queue de enteros a un string
std::string make_string_from_queue(Queue<char> q) {
	std::string bin;
	while (!q.empty()) {
		bin += q.dequeue();
	}
	return bin;
}

// Función de clasificación. Clasifica 00 a aquellos que con las reglas no entran en un bucle, y con 1101 a aquellos que resultan en un bucle.
std::string binary_classifier(std::string &bin) {
    Queue<char> q = make_queue_from_binary(bin);
	std::vector<std::string> seen; // Vector para almacenar estados de la cola. Si se vuelve a un estado después de haber aplicado las reglas, se considera un bucle.

    // Imprimir la cola inicial
    std::cout << "Initial queue: ";
    print_queue(q);

    // Mientras haya al menos 3 bits en la cola
    while (q.size() >= 3) {
		// Comprobar si el estado actual ya ha sido visitado
		if (std::find(seen.begin(), seen.end(), make_string_from_queue(q)) != seen.end()) {
			std::cout << "Loop found at: ";
            print_queue(q);
            std::cout << std::endl;
			return "1101";
		}
		seen.push_back(make_string_from_queue(q));


        char front = q.front();  // Obtener el primer bit

        // Eliminar los primeros 3 bits
        q.dequeue();
        q.dequeue();
        q.dequeue();

        // Añadir bits según la regla
        if (front == '0') {
            q.enqueue('0');
            q.enqueue('0');
        } else if (front == '1') {
            q.enqueue('1');
            q.enqueue('1');
            q.enqueue('0');
            q.enqueue('1');
        }
    }

	// En este punto la cadena no entró en un bucle, por lo tanto, su última iteración fue de la forma 0xx y su clasificación es 00
	return "00";
}

int main() {
    std::string bin;
    std::cout << "Enter a binary number: ";
    std::cin >> bin;

    // Llamar al clasificador binario
    std::string classification = binary_classifier(bin);

    // Imprimir el resultado
    std::cout << "Classification: " << classification << std::endl;

    return 0;
}
