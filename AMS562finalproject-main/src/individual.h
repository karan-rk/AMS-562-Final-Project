#pragma once
#include <vector>
#include <random>
#include <set>
#include "state.h"
class Individual : state
{
public:

    //initilize the social level for an individual
    Individual(size_t index, int mutation_time = 6) : state(Susceptible), index(index), mutation_time(mutation_time)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);
        std::uniform_real_distribution<> dis_1(0.0, 1.0);

        socialActivityLevel = dis(gen);

        source = dis_1(gen) < 0.3 ? true : false;
    }

    void ini_infection();

    //sick person will infect others with probability lamada(i) at given time i
    //also the number of person it infect depend on its socialActivityLevel
    void infection(double infectionRate, Individual* sick_individual);

    //sick person will recover with probability gamma(i) at gievn time i
    //recovered people will be susceptible after infected once
    void recover(double recoveryRate);

    //Susceptible and Recovered person may vaccine
    void vaccinate();

    State getState() const;

    int getSocialActivityLevel() const;

    size_t getIndex() const;

    void changeIndex(size_t idx);

    //stimulation the person that contact with each other
    const std::set<size_t> & getContacts() const;

    void clearContacts();

    void insert_idx_contacts(size_t idx);

    int getMutation_count() const;

    int getMutation_time() const;

    void die();

    Individual& operator=(const Individual& other);

    bool getSource() const;

    void travel();


private:

    bool source;

    State state;

    size_t index;

    int socialActivityLevel;

    std::set<size_t> uniqueContacts;

    //for mutation
    //virus will mutate after "mutation_time" many transmissions
    //new virus will infect the recovered people
    const int mutation_time;

    //count how many time that virus has been infect
    //mutation_count / mutation_time gives how many times the virus mutate
    int mutation_count = 0;

    //the level of recover to prevent a mutate virus infection.
    int recover_level = 0;

};

Individual& Individual::operator=(const Individual& other)
{
    if (this != &other) {

        state::operator=(other); 
        this->source = other.source;
        this->state = other.state;
        this->index = other.index;
        this->socialActivityLevel = other.socialActivityLevel;
        this->uniqueContacts = other.uniqueContacts;
        this->mutation_count = other.mutation_count;
        this->recover_level = other.recover_level;
    }
    return *this;
}

void Individual::infection(double infectionRate, Individual *sick_individual) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (state == Susceptible && dis(gen) < infectionRate)
    {
        state = Infected;
        mutation_count += (sick_individual->getMutation_count() + 1);
        state::Susceptible_count--;
        
        if (source)
        {
            state::Infected_count_0++;
        }
        else
        {
            state::Infected_count_1++;
        }
        

    }
    else if (state == Recovered && dis(gen) < infectionRate && (mutation_count/mutation_time) <= recover_level )
    {
        state = Susceptible;
        state::Recovered_count--;
        state::Susceptible_count++;
    }
    else if (state == Recovered && dis(gen) < infectionRate && (mutation_count / mutation_time) > recover_level)
    {
        state = Infected;
        mutation_count += (sick_individual->getMutation_count() + 1);
        state::Recovered_count--;
        if (source)
        {
            state::Infected_count_0++;
        }
        else
        {
            state::Infected_count_1++;
        }
    }
}

void Individual::recover(double recoveryRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (state == Infected && dis(gen) < recoveryRate)
    {
        state = Recovered;

        if (source)
        {
            state::Infected_count_0--;
        }
        else
        {
            state::Infected_count_1--;
        }
        state::Recovered_count++;

        recover_level = mutation_count / mutation_time;
        mutation_count = 0;
    }
}

void Individual::vaccinate()
{
    if (state == Susceptible)
    {
        state = Vaccinated;
        state::Susceptible_count--;
        state::Vaccinated_count++;
    }
    else if(state == Recovered)
    {
        state = Vaccinated;
        state::Recovered_count--;
        state::Vaccinated_count++;
    }
}

int Individual::getSocialActivityLevel() const 
{
    return socialActivityLevel;
}

Individual::State Individual:: getState() const
{
    return Individual::state;
}

size_t Individual::getIndex() const
{
    return index;
}

const std::set<size_t>& Individual::getContacts() const
{
    return Individual::uniqueContacts;
}

void Individual::clearContacts()
{
    Individual::uniqueContacts.clear();
}

void Individual:: insert_idx_contacts(size_t idx)
{
    Individual::uniqueContacts.insert(idx);
}

int Individual::getMutation_count() const
{
    return mutation_count;
}

void Individual::ini_infection()
{
    state = Infected;
    state::Susceptible_count--;
    state::Infected_count_0++;
   

}

int Individual::getMutation_time() const
{
    return mutation_time;
}
void Individual::die()
{
    state = Died;
    if (source)
    {
        state::Infected_count_0--;
    }
    else
    {
        state::Infected_count_1--;
    }
    state::Died_count++;
}

void Individual::changeIndex(size_t idx)
{
    index = idx;
}

bool Individual::getSource() const
{
    return source;
}

void Individual::travel()
{
    source = !source;
}