#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <random>

class Individual {
public:
    enum State { Susceptible, Infected, Recovered, Vaccinated, Dead };

    Individual();
    void updateState(double infectionRate, double recoveryRate, double mutationRate, double deathThreshold);
    void vaccinate();
    bool isHighRisk() const;
    int getSocialActivityLevel() const;
    State getState() const;
    bool isVaccinated() const;

    // Newly added public methods for newlyInfected
    void setNewlyInfected(bool status);
    bool isNewlyInfected() const;

private:
    State state;
    int daysInfected;
    int socialActivityLevel;
    bool isHighRiskFlag;
    bool isInfectedWithMutatedStrain;

    // Make newlyInfected private
    bool newlyInfected;
};

#endif // INDIVIDUAL_H
