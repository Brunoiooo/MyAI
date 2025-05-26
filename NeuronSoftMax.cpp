#include "pch.h"
#include "NeuronSoftMax.h"

double NeuronSoftMax::Activator()
{
    double maxSum = -INFINITY;
    for (Neuron* neuron : Neurons)
        maxSum = std::max(maxSum, neuron->GetSum());

    double denominator = 0.0;
    for (Neuron* neuron : Neurons)
        denominator += exp(neuron->GetSum() - maxSum);

    return exp(Sum - maxSum) / denominator;
}

void NeuronSoftMax::SetError(double expectedY)
{
    Error = Activator() - expectedY;
}
