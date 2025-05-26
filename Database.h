#pragma once

#include "Network.h"
#include "Layer.h"
#include "Neuron.h"
#include "NeuronSigmoidalUnipolar.h"
#include "NeuronReLU.h"
#include "NeuronSoftMax.h"
#include <vector>
#include <random>
#include "NewLayerTemplate.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace MySql::Data::MySqlClient;

public ref class Database {
private:
	MySqlConnection^ Connection;
	Random^ Rand = gcnew Random();

public:
	Database(String^ server, String^ port, String^ user, String^ password, String^ schema) {
		Connection = gcnew MySqlConnection("server=" + server + ";port=" + port + ";user=" + user + ";password=" + password + ";database=" + schema + ";");
	};

	Network* GetNetwork(int networkId);
	int CreateNetwork(String^ name, List<NewLayerTemplate>^ layersTemplates, int inputs);
	void UpdateNetwork(Network* network, int networkId);
	void DeleteNetwork(int networkId);

private:
	//void CreateSchema(String^ schema);

	int GetLastId();

	int InsertNetwork(String^ name);
	int InsertLayer(int networkId);
	int InsertNeuron(int layerId, String^ typeId);
	int InsertWeight(int neuronId);

	std::vector<std::vector<Neuron*>> GetLayersNeurons(int networkId);
	std::vector<std::vector<std::vector<double>>> GetLayersNeuronsWeights(int networkId);

	std::vector<int> GetLayersIds(int networkId);
	std::vector<Neuron*> GetLayerNeurons(int layerId);
	std::vector<int> GetLayerNeuronsIds(int layerId);
	std::vector<std::vector<double>> GetLayerNeuronsWeights(int layerId);
	std::vector<double> GetLayerNeuronWeights(int neuronId);
	std::vector<int> GetLayerNeuronWeightsIds(int neuronId);

	int CreateLayer(int networkId, NewLayerTemplate layerTemplate, int weightsCount);
	int CreateNeuron(int layerId, String^ typeId, int weightsCount);

	void UpdateLayer(Layer* layer, int layerId);
	void UpdateNeuron(Neuron* neuron, int neuronId);
	void UpdateBias(Neuron* neuron, int neuronId);
	void UpdateWeight(double weight, int weightId);

	double GetRand();
};