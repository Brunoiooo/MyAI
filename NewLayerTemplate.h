#pragma once

using namespace System;

public value struct NewLayerTemplate {
	int NeuronCount;
	String^ NeuronTypeId;

	void Validate();
};