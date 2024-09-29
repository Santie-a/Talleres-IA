#include "list.h"
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

// Función para cargar datos desde un string CSV simulado
List<float> load_data_from_csv(const std::string &csv_content) {
    List<float> dataList;
    std::istringstream sstream(csv_content);
    std::string line;
    
    while (std::getline(sstream, line)) {
        std::stringstream ss(line);
        float value;
        ss >> value;
        dataList.push_back(value);
    }
    
    return dataList;
}

// Prototipo de función para insertar datos agregados en la lista
void insert_aggregated_data(List<float> &list, int step) {
    auto it = list.begin();
    int counter = 0;
    float sum = 0.0;

    while (it != list.end()) {
        // Advance the counter and add the current value to the sum
        ++counter;
        sum += *it;
        ++it;

        // If the counter reaches step, calculate the average and insert it
        if (counter == step) {
            float average = sum / (step);
            list.insert(it, average);
            // Reset counter and sum after insertion
            counter = 0;
            sum = 0.0;
        }
    }
}

int main() {
    // Simulación de datos del archivo CSV como un string
    std::string csv_content =
        "55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n55.0\n52.7\n53.9\n55.6\n56.1\n"
        "57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n55.7\n57.1\n54.6\n56.9\n52.9\n"
        "54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n55.9\n56.4\n53.3\n52.5\n55.8\n"
        "56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n52.3\n54.7\n55.2\n56.5\n55.7\n"
        "54.9\n55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n55.0\n52.7\n53.9\n55.6\n"
        "56.1\n57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n55.7\n57.1\n54.6\n56.9\n"
        "52.9\n54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n55.9\n56.4\n53.3\n52.5\n"
        "55.8\n56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n52.3\n54.7\n55.2\n56.5\n"
        "55.7\n54.9\n55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n55.0\n52.7\n53.9\n"
        "55.6\n56.1\n57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n55.7\n57.1\n54.6\n"
        "56.9\n52.9\n54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n55.9\n56.4\n53.3\n"
        "52.5\n55.8\n56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n52.3\n54.7\n55.2\n"
        "56.5\n55.7\n54.9\n55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n55.0\n52.7\n"
        "53.9\n55.6\n56.1\n57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n55.7\n57.1\n"
        "54.6\n56.9\n52.9\n54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n55.9\n56.4\n"
        "53.3\n52.5\n55.8\n56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n52.3\n54.7\n"
        "55.2\n56.5\n55.7\n54.9\n55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n55.0\n"
        "52.7\n53.9\n55.6\n56.1\n57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n55.7\n"
        "57.1\n54.6\n56.9\n52.9\n54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n55.9\n"
        "56.4\n53.3\n52.5\n55.8\n56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n52.3\n"
        "54.7\n55.2\n56.5\n55.7\n54.9\n55.1\n54.3\n56.7\n55.5\n53.2\n52.8\n57.4\n58.9\n54.3\n56.2\n"
        "55.0\n52.7\n53.9\n55.6\n56.1\n57.2\n55.4\n54.8\n53.1\n56.5\n57.9\n55.3\n54.2\n56.6\n53.8\n"
        "55.7\n57.1\n54.6\n56.9\n52.9\n54.5\n55.4\n56.8\n57.3\n53.7\n54.1\n56.0\n52.6\n53.4\n54.0\n"
        "55.9\n56.4\n53.3\n52.5\n55.8\n56.2\n57.6\n54.9\n52.4\n53.5\n56.3\n57.8\n54.4\n53.0\n56.1\n";

    // Cargar datos desde el string CSV simulado
    List<float> humidityData = load_data_from_csv(csv_content);
    
    // Calcular el promedio (para simplificar, aquí se da un valor fijo)
    float averageHumidity = 55.5; // Este valor sería calculado a partir de los datos
    
    // Insertar los valores promedio al final de cada día (cada 24 lecturas)
    insert_aggregated_data(humidityData, 24);
    
    // Imprimir la lista resultante
    for (auto itr = humidityData.begin(); itr != humidityData.end(); ++itr) {
        std::cout << *itr << std::endl;
    }

    std::cout << humidityData.size() << std::endl;
    
    return 0;
}