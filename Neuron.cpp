#include "pch.h"
#include "Neuron.h"

double Neuron::GetSum()
{
    return Sum;
}

void Neuron::SetSum()
{
    Sum = Bias;

    for (const auto& pair : NeuronsBefore)
        Sum += pair.first->Activator() * pair.second;
}

void Neuron::SetSum(double sum)
{
    Sum = sum;
}

double Neuron::Activator()
{
    return Sum;
}

void Neuron::UpdateWeights(double rate)
{
    for (auto& pair : NeuronsBefore)
        pair.second = pair.second - rate * Gradient(pair.first);

    Bias -= rate * Gradient();
}

void Neuron::SetError()
{
    std::invalid_argument("Error");
}

void Neuron::SetError(double expectedY)
{
    std::invalid_argument("Error");
}

double Neuron::GetError()
{
    return Error;
}

double Neuron::GetWeight(Neuron* neuron)
{
    auto it = NeuronsBefore.find(neuron);

    if(NeuronsBefore.end() == it)
        throw std::invalid_argument("Error");

    return it->second;
}

double Neuron::Gradient()
{
    return Error;
}

double Neuron::Gradient(Neuron* neuronBefore)
{
    return neuronBefore->Activator() * Error;
}

std::vector<double> Neuron::GetWeights()
{
    std::vector<double> weights = std::vector<double>();

    for (auto& pair : NeuronsBefore)
        weights.push_back(pair.second);

    return weights;
}

double Neuron::GetBias()
{
    return Bias;
}

void Neuron::SetNeuronsBefore(std::vector<Neuron*> neuronsBefore, std::vector<double> weights)
{
    if (neuronsBefore.size() != weights.size())
        throw std::invalid_argument("Error");

    NeuronsBefore = std::unordered_map<Neuron*, double>();

    for (int i = 0; i < neuronsBefore.size(); i++)
        NeuronsBefore.insert({ neuronsBefore[i], weights[i] });
}

void Neuron::SetNeurons(std::vector<Neuron*> neurons)
{
    Neurons = neurons;
}

void Neuron::SetNeuronsAfter(std::vector<Neuron*> neuronsAfter)
{
    NeuronsAfter = neuronsAfter;
}
