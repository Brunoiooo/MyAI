#pragma once

#include <vector>
#include <unordered_map>
#include <stdexcept>

class Neuron {
protected:
	double Error = 0.0;
	double Sum = 0.0;
	std::unordered_map<Neuron*, double> NeuronsBefore = std::unordered_map<Neuron*, double>();
	std::vector<Neuron*> NeuronsAfter = std::vector<Neuron*>();
	double Bias = 0.0;
	std::vector<Neuron*> Neurons = std::vector<Neuron*>();

public:
	Neuron() {
	};
	Neuron(double bias) : Bias(bias) {
	};

	double GetSum();
	void SetSum();
	void SetSum(double sum);

	void SetNeuronsBefore(std::vector<Neuron*> neuronsBefore, std::vector<double> weights);
	void SetNeurons(std::vector<Neuron*> neurons);
	void SetNeuronsAfter(std::vector<Neuron*> neuronsAfter);

	virtual double Activator();

	void UpdateWeights(double rate);

	virtual void SetError();
	virtual void SetError(double expectedY);
	double GetError();

	double GetWeight(Neuron* neuron);
	std::vector<double> GetWeights();

	double GetBias();
private:
	double Gradient();
	double Gradient(Neuron* neuronBefore);
};