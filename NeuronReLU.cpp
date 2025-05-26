#include "pch.h"
#include "NeuronReLU.h"

double NeuronReLU::Activator()
{
    return std::max(0.0, Sum);
}

void NeuronReLU::SetError()
{
    Error = 0.0;

    for (Neuron* neuron : NeuronsAfter)
        Error += neuron->GetWeight(this) * neuron->GetError();

    Error *= ActivatorDerivative();
}

void NeuronReLU::SetError(double expectedY)
{
    Error = (Activator() - expectedY) * ActivatorDerivative();
}

double NeuronReLU::ActivatorDerivative()
{
    return Sum > 0 ? 1.0 : 0.0;
}
