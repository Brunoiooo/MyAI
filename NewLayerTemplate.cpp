#include "pch.h"
#include "NewLayerTemplate.h"

void NewLayerTemplate::Validate()
{
	if (NeuronCount < 1)
		throw gcnew System::Exception("Error");
}
