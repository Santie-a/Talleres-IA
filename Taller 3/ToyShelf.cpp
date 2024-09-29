#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

// Struct para mantener relación entre ID y nombre
struct Toy {
    int id;
    std::string name;
    
    Toy(int id, const std::string &name) : id(id), name(name) { }
};

// Clase para manejar la base de datos
class ToyShelf {
	private:
		std::vector<Toy> toys; // Vector para la base de datos (incluye los placeholder)
		std::vector<int> ids; // Mantener información de los IDs para no tener que iterar para limpiar los placeholders
		int capacity_per_stand; // Capacidad de cada estantería
		int stands = 0;
		
		// Función helper para conocer el último elemento de la base de datos que no es un placeholder
		int get_last_item_index() {
		    for (int i = toys.size() - 1; i >= 0; i--) {
		        if (toys[i].id != -1) { return i; }
		    }
		    return -1;
		}
		
	public:
		// Constructor ("create_shelf()") que se inicializa teniendo en cuenta la capacidad de cada estantería 
		ToyShelf(int capacity_per_stand) : capacity_per_stand(capacity_per_stand) { }

		// Inserta un nuevo juguete en la base de datos
		void add_toy(int id, const std::string &name) {
		    std::cout << "Adding " << name << "(ID: " << id << ") to the shelf..." << std::endl;
		    
		    int last = get_last_item_index();
		    
			// Se tiene en cuenta el último elemento válido para saber si es necesario crear placeholders o removerlos
			if ((last + 1) % capacity_per_stand == 0) {
				toys.insert(toys.begin(), Toy(id, name));
				stands++;
				
		        for (int i = 0; i < capacity_per_stand -1; i++) { toys.push_back(Toy(-1, "**")); } // Creacion de placeholders
			
			} else {
			    toys.pop_back();
				toys.insert(toys.begin(), Toy(id, name));
			}
			
			// Se almacenan los IDs para no tener que iterar para limpiar los placeholders
			ids.push_back(id);
			
		}
		
		// Se ordena el vector de IDs y se devuelve
		std::vector<int> view_shelf(bool rev=false) {
		    auto comp = (!rev) ? [] (int a, int b) { return a < b; } : [] (int a, int b) { return a > b; };
		    
		    std::sort(ids.begin(), ids.end(), comp);
		    
		    return ids;
		}
		
		// Mueve un elemento de la estantería a la ubicación inferior de otro elemento
		void move_below(int x, int y) {
		    int x_idx = -1;
            int y_idx = -1;
        
            for (int i = 0; i < toys.size(); i++) {
                if (toys[i].id == x) { x_idx = i; }
                if (toys[i].id == y) { y_idx = i; }
            }
            
            if ((x_idx != -1) && (y_idx != -1)) {
                
				// Crea una estantería si es necesario (en el caso de que el elemento de referencia se encuentre en la última estantería)
                if ((y_idx + capacity_per_stand) >= (capacity_per_stand * stands)) {
                    stands++;
                    for (int i = 0; i < capacity_per_stand; i++) { toys.push_back(Toy(-1, "**")); }
                }
                
                std::cout << "Moving " << toys[x_idx].name << " (ID: " << toys[x_idx].id << ") below " << toys[y_idx].name << " (ID: " << toys[y_idx].id << ")" << std::endl;
                std::swap(toys[x_idx], toys[y_idx + capacity_per_stand]);
            }
		}

		// Imprime la estantería para mantener visibilidad
		void print() {
			for (int i = 0; i < toys.size(); i++) {
			    
				if (i % capacity_per_stand == 0) {
				    std::cout << std::endl;
				    std::cout << std::string(12*capacity_per_stand, '-') << std::endl;
				}
				
				std::cout << std::setfill(' ') << std::setw(12);
				std::cout << toys[i].name;
			}
			std::cout << std::endl;
			std::cout << std::string(12*capacity_per_stand, '-') << std::endl;
			std::cout << "Stands: " << stands <<std::endl;
			std::cout << std::endl;
		}
};

void print_vector(std::vector<int> &vec) {
    for (auto el: vec) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

int main() {

	// Caso de prueba para una estantería con 5 elementos en cada estante
	int capacity_per_stand = 5;

	ToyShelf toy_shelf(capacity_per_stand);

	toy_shelf.add_toy(1, "Car");
	toy_shelf.add_toy(21, "Robot");
	toy_shelf.add_toy(14, "Doll");
	toy_shelf.add_toy(77, "Train");
	toy_shelf.add_toy(22, "Plane");
	toy_shelf.add_toy(42, "Helicopter");
	toy_shelf.add_toy(4, "Ball");
	toy_shelf.print();
	
	toy_shelf.move_below(14, 1);
	toy_shelf.move_below(22, 1);
	toy_shelf.move_below(4, 22);
	toy_shelf.move_below(77, 21);
	toy_shelf.print();
	
	toy_shelf.add_toy(55, "Bicycle");
	toy_shelf.add_toy(98, "Boat");
	toy_shelf.add_toy(45, "Jet");
	toy_shelf.add_toy(32, "Submarine");
	toy_shelf.print();
	
	toy_shelf.move_below(32, 98);
	toy_shelf.move_below(22, 32);
	toy_shelf.move_below(14, 4);
	toy_shelf.print();
	
	std::cout << "Printing ordered ids (asc): " <<std::endl;
	std::vector<int> ordered_ids_asc = toy_shelf.view_shelf();
	print_vector(ordered_ids_asc);
	
	std::cout << std::endl;
	
	std::cout << "Printing ordered ids (desc): " <<std::endl;
	std::vector<int> ordered_ids_desc = toy_shelf.view_shelf(true);
	print_vector(ordered_ids_desc);
	
	return 0;
}