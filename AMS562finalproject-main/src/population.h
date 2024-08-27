#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include "individual.h"

class Population : state
{
public:

    //initiallize the virus for all individuals
    Population(const size_t size, const double infectionRate, const double recoveryRate, const double vaccinationRate, const double initialInfectionRate, const double dieRate)
        : recoveryRate(recoveryRate), infectionRate(infectionRate), vaccinationRate(vaccinationRate),dieRate(dieRate), size(size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        state::Susceptible_count = size;
        state::Infected_count_0 = 0;
        state::Infected_count_1 = 0;
        state::Recovered_count = 0;
        state::Vaccinated_count = 0;

        individuals.resize(2);

        for (size_t i = 0; i < size; i++)
        {
            Individual newIndividual(0);

            if (newIndividual.getSource())
            {
                if (dis(gen) < initialInfectionRate)
                {
                    newIndividual.ini_infection(); // Initial infection
                }
                individuals[0].push_back(&newIndividual);

                individuals[0].back()->changeIndex(individuals[0].size() - 1);
            }
            else
            {
                individuals[1].push_back(&newIndividual);
                individuals[1].back()->changeIndex(individuals[1].size() - 1);
            }

        }

    }

    void start_stimulate();


private:

    //update from today to next day
    void updatePopulationState();

    //print the result
    //currently too slow
    void OutPutPopulationStates();

    //check if the virus clear
    bool is_clear() const;

    bool disease_travel = false;

    void die();

    //loop through all individuals to vaccinate
    void vaccination();


    //infect depend on social level for all individuals
    void contagion();
    void _contagion(Individual* individual, size_t idx, short int i);


    void ofstream_to_m(size_t susceptible, size_t infected, size_t recovered, size_t vaccinated, size_t died, size_t mutation) const;

    void ofstream_to_m(size_t infected_source, size_t infected_other) const;

    void clean_state();

    void population_travel();

    std::vector<std::vector<Individual*>> individuals;


    const double infectionRate;

    const double recoveryRate;

    const double vaccinationRate;

    const double dieRate;

    const double travelRate = 0.0001;

    bool disease_clear = false;

    int max_mutation = 0;

    const size_t size = 0;

    size_t current_step = 0;
};

void Population::start_stimulate()
{

    size_t timeSteps = 1000;
    for ( ; current_step < timeSteps; current_step++)
    {
        std::cout << "Time Step " << current_step << std::endl;
        OutPutPopulationStates();
        if (is_clear() == true) { break; }
        updatePopulationState();
        std::cout << std::endl;
    }

}
void Population::updatePopulationState()
{
    die();
    contagion();

    if(current_step >= 30) vaccination();

    clean_state();
    population_travel();
}

void Population::population_travel()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (short int i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < individuals[i].size(); j++)
        {

            if (dis(gen) < travelRate)
            {
                if (individuals[i][j]->getState() == state::Infected)
                {
                    disease_travel = true;
                    if (individuals[i][j]->getSource())
                    {
                        Infected_count_0--;
                        Infected_count_1++;
                    }
                    else
                    {
                        Infected_count_0++;
                        Infected_count_1--;
                    }
                }

                individuals[i][j]->travel();

                //push back to the other country and update the index
                individuals[(i + 1) % 2].push_back(individuals[i][j]);
                individuals[(i + 1) % 2].back()->changeIndex(individuals[(i + 1) % 2].size() - 1);

                //erase in current country and update the index
                individuals[i].back()->changeIndex(j);
                individuals[i][j] = individuals[i].back();
                individuals[i].pop_back();
            }
        }

    }
}
void Population::die()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (short int i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < individuals[i].size(); j++)
        {
            if (dis(gen) < dieRate && individuals[i][j]->getState() == state::Infected)
            {
                individuals[i][j]->die();
                individuals[i].back()->changeIndex(j);
                individuals[i][j] = individuals[i].back();
                individuals[i].pop_back();
            }
        }

        if (!disease_travel) break;
    }
}

void Population::vaccination()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (short int i = 0; i < 2; i++)
    {
        for (Individual* individual : individuals[i])
        {
            if (dis(gen) < vaccinationRate)
            {
                individual->vaccinate();
            }
        }

        if (!disease_travel) break;
    }

}

void  Population::clean_state()
{
    for (short int i = 0; i < 2; i++)
    {
        for (Individual* individual : individuals[i])
        {
            individual->clearContacts();
        }
        if (!disease_travel) break;
    }
}

void  Population::OutPutPopulationStates()
 {
    assert(state::Susceptible_count + state::Infected_count_0 + state::Infected_count_1  + state::Recovered_count + state::Vaccinated_count + state::Died_count == size);

    if (Infected_count_0 == 0 && Infected_count_1 == 0) { disease_clear = true; }
    std::cout << "Susceptible: " << state::Susceptible_count
        << ", Infected_source: " << state::Infected_count_0
        << ", Infected_other: " << state::Infected_count_1
        << ", Recovered: " << state::Recovered_count
        << ", Vaccinated: " << state::Vaccinated_count
        << ", Died: " << state::Died_count
        << ", Virus Mutate Time: " << max_mutation 
        << ", Disease Tranvel: " << disease_travel << std::endl;

    ofstream_to_m(state::Susceptible_count, state::Infected_count_0 + state::Infected_count_1, state::Recovered_count, state::Vaccinated_count, state::Died_count, max_mutation);
    ofstream_to_m(state::Infected_count_0, state::Infected_count_1);
}

void Population::contagion()
{

    for (short int i = 0; i < 2; i++)
    {
        std::vector<int> infect_individuals_idx;
        for (size_t j = 0; j < individuals[i].size(); j++)
        {
            individuals[i][j]->recover(recoveryRate);
            if (individuals[i][j]->getState() == state::Infected)
            {
                assert(individuals[i][j]->getIndex() == j);
                infect_individuals_idx.push_back(j);
            }
        }
        for (size_t j = 0; j < infect_individuals_idx.size(); j++)
        {
            _contagion(individuals[i][infect_individuals_idx[j]], infect_individuals_idx[j], i);
        }

        if (!disease_travel) break;
    }
}

void  Population::_contagion(Individual* individual, size_t idx, short int i)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> individualDist(0, individuals[i].size() - 1);


    //get unique set of person's index in vector that contact with infected person.
    while (individual->getContacts().size() < (size_t)individual->getSocialActivityLevel())
    {
        size_t randomIndex = individualDist(gen);
        individual->insert_idx_contacts(randomIndex);
        individuals[i][randomIndex]->insert_idx_contacts(idx);
    }

    for (const auto &idx : individual->getContacts())
    {
        individuals[i][idx]->infection(infectionRate, individual);
        max_mutation = std::max(max_mutation, individuals[i][idx]->getMutation_count() / individuals[i][idx]->getMutation_time());
    }
}

bool Population::is_clear() const
{
    return disease_clear;
}


void Population::ofstream_to_m(size_t susceptible, size_t infected, size_t recovered, size_t vaccinated, size_t died, size_t mutation) const
{
    std::ofstream outputFile("result.m", std::ios::app);
    if (outputFile.is_open()) 
    {
        outputFile << susceptible << " " << infected << " " << recovered << " " << vaccinated << " " << died << " " << mutation << std::endl;
        outputFile.close();
    }
    else 
    {
        std::cerr << "Unable to open file for output!" << std::endl;
    }
}


void Population::ofstream_to_m(size_t infected_source, size_t infected_other) const
{
    std::ofstream outputFile("infect_map.m", std::ios::app);
    if (outputFile.is_open())
    {
        outputFile << infected_source << " " << infected_other << std::endl;
        outputFile.close();
    }
    else
    {
        std::cerr << "Unable to open file for output!" << std::endl;
    }
}