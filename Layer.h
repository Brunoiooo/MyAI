#pragma once

#include "Neuron.h"
#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <unordered_map>

class Layer {
private:
	std::vector<Neuron*> Neurons;

public:
	Layer(std::vector<Neuron*> neurons) : Neurons(neurons) {
		if (neurons.empty())
			throw std::invalid_argument("Error");

		for (Neuron* neuron : neurons)
			neuron->SetNeurons(neurons);
	};
	Layer(std::vector<Neuron*> neuronsBefore, std::vector<Neuron*> neurons, std::vector<std::vector<double>> neuronsWeights, std::vector<Neuron*> neuronsAfter) : Layer(neurons) {
		if (neuronsWeights.size() != neurons.size())
			throw std::invalid_argument("Error");

		for (int i = 0; i < Neurons.size(); i++)
			neurons[i]->SetNeuronsBefore(neuronsBefore, neuronsWeights[i]);

		for (Neuron* neuron : neurons)
			neuron->SetNeuronsAfter(neuronsAfter);
	};
	~Layer();

	void SetSums();
	void SetSums(std::vector<double> sums);

	void SetErrors();
	void SetErrors(std::vector<double> expectedYs);

	void UpdateWeights(double rate);

	std::vector<Neuron*> GetNeurons();
};