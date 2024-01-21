#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <random>
using namespace std;

int capacity;
int numberOfItems;
int populationSize;
int FROM_PREV_POPULATION;
vector<vector<bool>> population;
unordered_set<vector<bool>> exists;

struct Item {
    int weight;
    int value;
};

vector<Item> items;

int fitness(vector<bool> &chromosome) {
    int weight=0;
    int value=0;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> index(0, chromosome.size() - 1);

    for (int i = 0; i < numberOfItems; i++) {
        if( chromosome[i]) {
            weight += items[i].weight;
            value += items[i].value;
            while (weight > capacity) {
                int idx = index(rng);
                while (chromosome[idx] == 0)  {
                    idx = index(rng);
                }
                chromosome[idx] = false;
                weight -= items[idx].weight;
                value -= items[idx].value;
            }
        }
    }
    return value;
}

vector<vector<bool>> startingPopulation () {
    vector<vector<bool>> population;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> distribution(0, 1);

    for (int i = 0; i < populationSize; i++) {
        vector<bool> temp;
        for (int j = 0; j < numberOfItems; j++) {
            temp.push_back(distribution(rng));
        }
        if ( exists.find(temp) == exists.end() ) {
            fitness(temp);
            population.push_back(temp);
            exists.insert(temp);
        } else {
            i--;
            continue;
        }
    }

    return population;

}

struct Compare {
    bool operator()(vector<bool> &one, vector<bool> &two) {
        int fittnesOne = fitness(one);
        int fittnesTwo = fitness(two);

        return fittnesOne < fittnesTwo;
    }
};

pair<vector<bool>, vector<bool>> crossover(vector<bool> &chromosome1, vector<bool> &chromosome2) {
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> distribution(1, chromosome1.size()-2);

    int cutPoint =  distribution(rng);
    vector<bool> newChromosome , newChromosome2;
    for (int i = 0; i < chromosome1.size(); i++) {
        if(i < cutPoint) {
            newChromosome.push_back(chromosome1[i]);
            newChromosome2.push_back(chromosome2[i]);
        } else {
            newChromosome.push_back(chromosome2[i]);
            newChromosome2.push_back(chromosome1[i]);
        }
    }

    pair<vector<bool>, vector<bool>> chromosomes;
    chromosomes.first = newChromosome;
    chromosomes.second = newChromosome2;
    return chromosomes;
}

vector<bool> mutate(vector<bool> &chromosome) {
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> mutate(1, 100);
    if( mutate(rng) <= 5) {
        random_device dev1;
        mt19937 rng1(dev1());
        uniform_int_distribution<std::mt19937::result_type> bit(0, chromosome.size()-1);
        int index = bit(rng1);
        chromosome[index] = !chromosome[index];
    }

    fitness(chromosome);
    return  chromosome;
}

void generatePopulation() {

    vector<vector<bool>> newPopulation;
    priority_queue<vector<bool>, vector<vector<bool>>, Compare> queue1;
    for (auto i = population.begin(); i!=population.end(); i++) {
        queue1.push(*i);
    }

    for (int i = 0; i < FROM_PREV_POPULATION; i++) {
        auto currBest = queue1.top();
        newPopulation.push_back(currBest);
        queue1.pop();
    }

    while ( newPopulation.size() < population.size()) {
        random_device dev;
        mt19937 rng(dev());
        uniform_int_distribution<std::mt19937::result_type> positions(0, population.size() - 1);

        int index1 = positions(rng);
        int index2 = positions(rng);
        while (index1 == index2) {
            index1 = positions(rng);
            index2 = positions(rng);
        }

        pair<vector<bool>, vector<bool>> child = crossover(population[index1], population[index2]);
        newPopulation.push_back(mutate(child.first));
        newPopulation.push_back(mutate(child.second));
    }

    population = newPopulation;

}

int maxFitnes(){
    int max = -2;
    for (int i = 0; i < population.size(); i++) {
        int f = fitness(population[i]);
        if(max < f) {
            max = f;
        }
    }
    return max;

}


int main() {

    cin>> capacity >> numberOfItems;
    populationSize = 2*numberOfItems;
    FROM_PREV_POPULATION = populationSize / 10;
    if ( FROM_PREV_POPULATION == 0) {
        FROM_PREV_POPULATION = 1;
    }
    for (int i = 0; i < numberOfItems; i++) {
        Item item;
        cin>>item.weight;
        cin>>item.value;
        items.push_back(item);
    }

    population = startingPopulation();
    int i=0;
    while(i < 1000) {
        generatePopulation();

        if ( i % 50 == 0) {
            cout << maxFitnes() << endl;
        }

        i++;
    }
    return 0;
}

// beam search prilicha na tova  hill pazim purviq nai dobur pri beam pazim fiksiran razmer v populaciqta