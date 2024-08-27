#include "population.h"
#include "individual.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
int main(int argc, char* argv[])
{

    // Determine parameter file name
    std::string paramFileName = "parameters.txt";  // Default parameter file
    if (argc > 1) {
        paramFileName = argv[1];  // Use the provided parameter file
    }
    else {
        std::cout << "No parameter file specified. Using default 'parameters.txt'." << std::endl;
    }

    // Load parameters from file
    std::unordered_map<std::string, double> parameters;
    std::ifstream paramFile(paramFileName);
    if (!paramFile.is_open()) {
        std::cerr << "Error opening parameter file: " << paramFileName << std::endl;
        return 1;  // Exit the program with an error code
    }

    std::string line;
    while (std::getline(paramFile, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        if (std::getline(iss, key, '=') && (iss >> value)) {
            parameters[key] = value;
        }
    }
    paramFile.close();


    size_t populationSize = static_cast<int>(parameters["populationSize"]);
    const double infectionRate = parameters["infectionRate"];
    const double initialInfectionRate = parameters["initialInfectionRate"];
    const double recoveryRate = parameters["recoveryRate"];
    const double vaccinationRate = parameters["vaccinationRate"];
    const double dieRate = parameters["dieRate"];

    Population myPopulation(populationSize, infectionRate, recoveryRate, vaccinationRate, initialInfectionRate, dieRate);

    myPopulation.start_stimulate();

    return 0;
}
