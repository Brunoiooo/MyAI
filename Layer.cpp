#include "pch.h"
#include "Layer.h"

Layer::~Layer()
{
	for (Neuron* neuron : Neurons)
		delete neuron;
}

void Layer::SetSums()
{
	for (Neuron* neuron : Neurons)
		neuron->SetSum();
}

void Layer::SetSums(std::vector<double> sums)
{
	if (sums.size() != Neurons.size())
		throw std::invalid_argument("Error");

	for (int i = 0; i < sums.size(); i++)
		Neurons[i]->SetSum(sums[i]);
}

void Layer::SetErrors()
{
	for (Neuron* neuron : Neurons)
		neuron->SetError();
}

void Layer::SetErrors(std::vector<double> expectedYs)
{
	if (expectedYs.size() != Neurons.size())
		throw std::invalid_argument("Error");

	for (int i = 0; i < expectedYs.size(); i++)
		Neurons[i]->SetError(expectedYs[i]);
}

void Layer::UpdateWeights(double rate)
{
	for (Neuron* neuron : Neurons)
		neuron->UpdateWeights(rate);
}

std::vector<Neuron*> Layer::GetNeurons()
{
	return Neurons;
}

