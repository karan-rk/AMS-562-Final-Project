#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include <vector>
#include <random>

class Population {
private:
    std::vector<Individual> individuals;
    std::mt19937 gen;  // Random number generator
    double interactionRate;
    double recoveryRate;
    double mutationRate;
    double deathThreshold;
    int dailyNewInfections; // Add this to track daily new infections

public:
    Population(int size, double interactionRate, double initialInfectionRate, 
               double recoveryRate, double mutationRate, double deathThreshold);
    int getSusceptibleCount() const;
    int getInfectedCount() const;
    int getRecoveredCount() const;
    int getVaccinatedCount() const;
    int getDeathCount() const;
    int getDailyNewInfections() const; // Add this to get the count of daily new infections
    void vaccinateHighRisk(double vaccinationRate);
    void vaccinateSociallyActive(double vaccinationRate, int threshold);
    void updatePopulationState(double infectionRate); // This will need modification to track new infections
    bool noInfections() const;
    void printPopulationStates() const;
};

#endif // POPULATION_H
