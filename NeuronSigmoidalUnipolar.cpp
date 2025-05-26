#include "pch.h"
#include "NeuronSigmoidalUnipolar.h"

double NeuronSigmoidalUnipolar::Activator()
{
    return 1.0 / (1.0 + std::exp(-Sum));
}

void NeuronSigmoidalUnipolar::SetError()
{
    Error = 0.0;

    for (Neuron* neuron : NeuronsAfter)
        Error += neuron->GetWeight(this) * neuron->GetError();

    Error *= Activator() * (1 - Activator());
}

void NeuronSigmoidalUnipolar::SetError(double expectedY)
{
    Error = (Activator() - expectedY) * Activator() * (1 - Activator());
}