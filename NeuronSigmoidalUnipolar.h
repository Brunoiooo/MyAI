#pragma once

#include "Neuron.h"

class NeuronSigmoidalUnipolar : public Neuron {
	using Neuron::Neuron;

public:
	double Activator();

	void SetError();
	void SetError(double expectedY);
};