#include "Individual.h"

Individual::Individual() : state(Susceptible), daysInfected(0),
                           socialActivityLevel(rand() % 10), isHighRiskFlag(rand() % 2 == 0),
                           isInfectedWithMutatedStrain(false), newlyInfected(false) {}  // Initialize newlyInfected to false

void Individual::updateState(double infectionRate, double recoveryRate, double mutationRate, double deathThreshold) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    // Reset newlyInfected flag at the start of each update
    newlyInfected = false;

    // If already infected, check for recovery or death
    if (state == Infected) {
        daysInfected++;
        
        // Check for recovery
        if (dis(gen) < recoveryRate) {
            state = Recovered;
            isInfectedWithMutatedStrain = false;  // Reset mutation flag upon recovery
        }

        // Check for death
        if (daysInfected > deathThreshold) {
            state = Dead;
        }
    } 
    // If susceptible, check if the individual becomes infected
    else if (state == Susceptible) {
        if (dis(gen) < infectionRate) {
            state = Infected;
            newlyInfected = true;  // Set the flag when the individual becomes infected
            isInfectedWithMutatedStrain = dis(gen) < mutationRate;  // Check for mutation
        }
    }

    // Other states (Recovered, Vaccinated, Dead) do not require updates in this method
}

void Individual::vaccinate() {
    if (state == Susceptible) {
        state = Vaccinated;
    }
}

bool Individual::isHighRisk() const {
    return isHighRiskFlag;
}

int Individual::getSocialActivityLevel() const {
    return socialActivityLevel;
}

Individual::State Individual::getState() const {
    return state;
}

bool Individual::isVaccinated() const {
    return state == Vaccinated;
}

void Individual::setNewlyInfected(bool status) {
    newlyInfected = status;
}

bool Individual::isNewlyInfected() const {
    return newlyInfected;
}
