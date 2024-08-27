#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Population.h"

int main(int argc, char* argv[]) {
    // Determine parameter file name
    std::string paramFileName = "parameters.txt";  // Default parameter file
    if (argc > 1) {
        paramFileName = argv[1];  // Use the provided parameter file
    } else {
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

    // Assign parameters to variables
    int populationSize = static_cast<int>(parameters["populationSize"]);
    double interactionRate = parameters["interactionRate"];
    double infectionRate = parameters["infectionRate"];
    double initialInfectionRate = parameters["initialInfectionRate"];
    double recoveryRate = parameters["recoveryRate"];
    double vaccinationRate = parameters["vaccinationRate"];
    double mutationRate = parameters["mutationRate"];
    double deathThreshold = parameters["deathThreshold"];
    int totalSteps = static_cast<int>(parameters["totalSteps"]);

    // Validate parameters
    if (populationSize <= 0 || totalSteps <= 0 || interactionRate < 0 || interactionRate > 1) {
        std::cerr << "Invalid parameters in file: " << paramFileName << std::endl;
        return 1;  // Exit with an error code
    }

    // Initialize and run the simulation
    Population population(populationSize, interactionRate, initialInfectionRate, recoveryRate, mutationRate, deathThreshold);
    std::ofstream outFile("simulation_output.csv");
    outFile << "TimeStep,Susceptible,Infected,Recovered,Vaccinated,Dead,NewInfections\n";

    for (int step = 0; step < totalSteps; ++step) {
        population.updatePopulationState(infectionRate);
        int numSusceptible = population.getSusceptibleCount();
        int numInfected = population.getInfectedCount();
        int numRecovered = population.getRecoveredCount();
        int numVaccinated = population.getVaccinatedCount();
        int numDead = population.getDeathCount();
        int newInfections = population.getDailyNewInfections();

        outFile << step << "," << numSusceptible << "," << numInfected << "," << numRecovered << "," << numVaccinated << "," << numDead << "," << newInfections << "\n";
        std::cout << "Time Step " << step << " - Susceptible: " << numSusceptible << ", Infected: " << numInfected << ", Recovered: " << numRecovered << ", Vaccinated: " << numVaccinated << ", Dead: " << numDead << ", New Infections: " << newInfections << std::endl;

        if (population.noInfections()) {
            std::cout << "No more infections, ending simulation at step " << step << std::endl;
            break;
        }
    }

    outFile.close();
    return 0;
}
