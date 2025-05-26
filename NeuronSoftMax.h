#pragma once

#include "Neuron.h"

class NeuronSoftMax : public Neuron {
	using Neuron::Neuron;

public:
	double Activator();

	void SetError(double expectedY);

};