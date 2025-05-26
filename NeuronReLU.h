#pragma once

#include "Neuron.h"
#include <algorithm>

class NeuronReLU : public Neuron {
	using Neuron::Neuron;

public:
	double Activator();

	void SetError();
	void SetError(double expectedY);

private:
	double ActivatorDerivative();
};