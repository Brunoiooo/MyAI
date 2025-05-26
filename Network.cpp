#include "pch.h"
#include "Network.h"

Network::~Network()
{
    for (Layer* layer : Layers)
        delete layer;
}

std::vector<double> Network::Outputs(std::vector<double> inputs)
{
    if (inputs.size() != Layers[0]->GetNeurons().size())
        throw std::invalid_argument("Error");

    for (int i = 0; i < inputs.size(); i++)
        Layers[0]->GetNeurons()[i]->SetSum(inputs[i]);

    for (int i = 1; i < Layers.size(); i++)
        Layers[i]->SetSums();

    std::vector<double> outputs = std::vector<double>();
    for (Neuron* neuron : Layers.back()->GetNeurons())
        outputs.push_back(neuron->Activator());

    return outputs;
}

std::vector<double> Network::Outputs(std::vector<double> inputs, std::vector<double> expectedYs, double rate)
{
    std::vector<double> outputs = Outputs(inputs);

    for (int i = Layers.size(); i > 1; i--)
        i == Layers.size() ? Layers[i - 1]->SetErrors(expectedYs) : Layers[i - 1]->SetErrors();

    for (int i = 1; i < Layers.size(); i++)
        Layers[i]->UpdateWeights(rate) ;

    return outputs;
}

std::vector<Layer*> Network::GetLayers()
{
    return Layers;
}
