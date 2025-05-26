#pragma once

#include "Layer.h"
#include <vector>
#include <stdexcept>

class Network {
private:
	std::vector<Layer*> Layers;

public:
	Network(std::vector<std::vector<Neuron*>> neurons, std::vector<std::vector<std::vector<double>>> weights) {
		if (neurons.empty())
			throw std::invalid_argument("Error");

		if (neurons.size() != weights.size())
			throw std::invalid_argument("Errro");

		std::vector<Neuron*> inputs = std::vector<Neuron*>();
		for (int i = 0; i < weights[0][0].size(); i++)
			inputs.push_back(new Neuron());

		Layers = std::vector<Layer*>();
		Layers.push_back(new Layer(inputs));

		for (int i = 0; i < neurons.size(); i++)
			Layers.push_back(new Layer(i == 0 ? inputs : neurons[i - 1], neurons[i], weights[i], i == neurons.size() - 1 ? std::vector<Neuron*>() : neurons[i + 1]));
	}
	~Network();

	std::vector<double> Outputs(std::vector<double> inputs);
	std::vector<double> Outputs(std::vector<double> inputs, std::vector<double> expectedYs, double rate);

	std::vector<Layer*> GetLayers();
};