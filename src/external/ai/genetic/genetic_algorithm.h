#pragma once

#include "../neural_network.h"

struct MutationRate
{
    float chanceOfMutation;
    float chanceOfBitMutation;
};
/**
 * You can play with these values in order to change the behaviour of mutation.
 */
struct MutationTendency
{
    MutationRate biasMutationRate   {  0.2f, 0.2f };
    MutationRate weightMutationRate { 0.04f, 0.2f };
};

/**
 * I decided to make it a static class, because I wanted to make it a friend of the classes
 * NeuralNetwork, Layer and Neuron in order to simplify interactions between them.
 */
class GA 
{
public:
    static NeuralNetwork Crossover(const NeuralNetwork& parent0, const NeuralNetwork& parent1, MutationTendency mutationTendency = MutationTendency());
};