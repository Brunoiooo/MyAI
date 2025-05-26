#include "pch.h"
#include "Database.h"

int Database::CreateNetwork(String^ name, List<NewLayerTemplate>^ layersTemplates, int inputs)
{
    if (layersTemplates->Count <= 0)
        throw gcnew Exception("Error");

    Connection->Open();

    MySqlTransaction^ mySqlTransaction = Connection->BeginTransaction();

    try {
        int networkId = InsertNetwork(name);

        for (int i = 0; i < layersTemplates->Count; i++)
            CreateLayer(networkId, layersTemplates[i], i == 0 ? inputs : layersTemplates[i - 1].NeuronCount);

        mySqlTransaction->Commit();

        return networkId;
    }
    catch (...) {
        mySqlTransaction->Rollback();

        throw;
    }
    finally {
        Connection->Close();
    }
}

void Database::UpdateNetwork(Network* network, int networkId)
{
    Connection->Open();

    std::vector<int> layersIds = GetLayersIds(networkId);

    if (layersIds.empty())
        throw gcnew Exception("Error");

    if (layersIds.size() != network->GetLayers().size() - 1)
        throw gcnew Exception("Error");

    MySqlTransaction^ mySqlTransaction = Connection->BeginTransaction();

    try {
        for (int i = 0; i < layersIds.size(); i++)
            UpdateLayer(network->GetLayers()[i + 1], layersIds[i]);

        mySqlTransaction->Commit();
    }
    catch (...) {
        mySqlTransaction->Rollback();

        throw;
    }
    finally {
        Connection->Close();
    }
}


void Database::DeleteNetwork(int networkId)
{
    Connection->Open();

    MySqlTransaction^ mySqlTransaction = Connection->BeginTransaction();

    try {
        MySqlCommand^ mySqlCommand = gcnew MySqlCommand("DELETE FROM networks WHERE id = @networkId", Connection);

        mySqlCommand->Parameters->AddWithValue("@networkId", networkId);

        mySqlCommand->ExecuteNonQuery();

        mySqlTransaction->Commit();
    }
    catch (...) {
        mySqlTransaction->Rollback();

        throw;
    }
    finally {
        Connection->Close();
    }
}

Network* Database::GetNetwork(int networkId)
{
    Connection->Open();

    Network* network = nullptr;

    try {
        network = new Network(GetLayersNeurons(networkId), GetLayersNeuronsWeights(networkId));

        return network;
    }
    catch (...) {
        if(network != nullptr)
            delete network;

        throw;
    }
    finally {
        Connection->Close();
    }
}

int Database::GetLastId()
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT LAST_INSERT_ID()", Connection);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    int lastId;

    try {
        if (!mySqlDataReader->Read())
            throw gcnew Exception("Error");

        lastId = mySqlDataReader->GetInt32(0);
    }
    catch (...) {
        throw;
    }
    finally {
        mySqlDataReader->Close();
    }

    return lastId;
}

int Database::InsertNetwork(String^ name)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("INSERT INTO networks (name) VALUES (@name)", Connection);

    mySqlCommand->Parameters->AddWithValue("@name", name);

    mySqlCommand->ExecuteNonQuery();

    return GetLastId();
}

int Database::InsertLayer(int networkId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("INSERT INTO layers (network_id) VALUES (@networkId)", Connection);

    mySqlCommand->Parameters->AddWithValue("@networkId", networkId);

    mySqlCommand->ExecuteNonQuery();

    return GetLastId();
}

int Database::InsertNeuron(int layerId, String^ typeId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("INSERT INTO neurons (type_id, bias, layer_id) VALUES (@typeId, @bias, @layerId)", Connection);

    mySqlCommand->Parameters->AddWithValue("@typeId", typeId);
    mySqlCommand->Parameters->AddWithValue("@bias", GetRand());
    mySqlCommand->Parameters->AddWithValue("@layerId", layerId);

    mySqlCommand->ExecuteNonQuery();

    return GetLastId();
}

std::vector<std::vector<Neuron*>> Database::GetLayersNeurons(int networkId)
{
    std::vector<std::vector<Neuron*>> layersNeurons = std::vector<std::vector<Neuron*>>();

    try {
        std::vector<int> layersIds = GetLayersIds(networkId);
        for (int layerId : layersIds)
            layersNeurons.push_back(GetLayerNeurons(layerId));
    }
    catch (...) {
        for (std::vector<Neuron*> layerNeurons : layersNeurons)
            for (Neuron* neuron : layerNeurons)
                delete neuron;

        throw;
    }
 
    return layersNeurons;
}

std::vector<std::vector<std::vector<double>>> Database::GetLayersNeuronsWeights(int networkId)
{
    std::vector<std::vector<std::vector<double>>> layersNeuronsWeights = std::vector<std::vector<std::vector<double>>>();

    for (int layerId : GetLayersIds(networkId))
        layersNeuronsWeights.push_back(GetLayerNeuronsWeights(layerId));

    return layersNeuronsWeights;
}

std::vector<int> Database::GetLayersIds(int networkId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT id FROM layers WHERE network_id = @networkId", Connection);

    mySqlCommand->Parameters->AddWithValue("@networkId", networkId);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    std::vector<int> layersIds = std::vector<int>();

    try {
        while (mySqlDataReader->Read())
            layersIds.push_back(mySqlDataReader->GetInt32(0));
    }
    catch (...) {
        throw;
    }
    finally {
        mySqlDataReader->Close();
    }

    return layersIds;
}

std::vector<Neuron*> Database::GetLayerNeurons(int layerId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT type_id, bias FROM neurons WHERE layer_id = @layerId", Connection);

    mySqlCommand->Parameters->AddWithValue("@layerId", layerId);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    std::vector<Neuron*> neurons = std::vector<Neuron*>();
    try {
        while (mySqlDataReader->Read())
            if (mySqlDataReader->GetString(0) == "NeuronSigmoidalUnipolar")
                neurons.push_back(new NeuronSigmoidalUnipolar(mySqlDataReader->GetDouble(1)));
            else if (mySqlDataReader->GetString(0) == "NeuronReLU")
                neurons.push_back(new NeuronReLU(mySqlDataReader->GetDouble(1)));
            else if (mySqlDataReader->GetString(0) == "NeuronSoftMax")
                neurons.push_back(new NeuronSoftMax(mySqlDataReader->GetDouble(1)));
            else
                throw gcnew Exception("Error");

        return neurons;
    }
    catch (...) {
        for (Neuron* neuron : neurons)
            delete neuron;

        throw;
    }
    finally {
        mySqlDataReader->Close();
    }
}

std::vector<int> Database::GetLayerNeuronsIds(int layerId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT id FROM neurons WHERE layer_id = @layerId", Connection);

    mySqlCommand->Parameters->AddWithValue("@layerId", layerId);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    std::vector<int> neuronsIds = std::vector<int>();

    try{
        while (mySqlDataReader->Read())
            neuronsIds.push_back(mySqlDataReader->GetInt32(0));
    }
    catch (...) {
        throw;
    }
    finally {
        mySqlDataReader->Close();
    }

    return neuronsIds;
}

std::vector<std::vector<double>> Database::GetLayerNeuronsWeights(int layerId)
{
    std::vector<std::vector<double>> layerNeuronsWeights = std::vector<std::vector<double>>();
    for (int layerNeuronId : GetLayerNeuronsIds(layerId))
        layerNeuronsWeights.push_back(GetLayerNeuronWeights(layerNeuronId));

    return layerNeuronsWeights;
}

std::vector<double> Database::GetLayerNeuronWeights(int neuronId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT weight FROM weights WHERE neuron_id = @neuronId", Connection);

    mySqlCommand->Parameters->AddWithValue("@neuronId", neuronId);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    std::vector<double> weights = std::vector<double>();

    try {
        while (mySqlDataReader->Read())
            weights.push_back(mySqlDataReader->GetDouble(0));
    }
    catch (...) {
        throw;
    }
    finally {
        mySqlDataReader->Close();
    }

    return weights;
}

std::vector<int> Database::GetLayerNeuronWeightsIds(int neuronId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("SELECT id FROM weights WHERE neuron_id = @neuronId", Connection);

    mySqlCommand->Parameters->AddWithValue("@neuronId", neuronId);

    MySqlDataReader^ mySqlDataReader = mySqlCommand->ExecuteReader();

    std::vector<int> weightsIds = std::vector<int>();

    try {
        while (mySqlDataReader->Read())
            weightsIds.push_back(mySqlDataReader->GetInt32(0));
    }
    catch (...) {
        throw;
    }
    finally {
        mySqlDataReader->Close();
    }

    return weightsIds;
}

int Database::InsertWeight(int neuronId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("INSERT INTO weights (weight, neuron_id) VALUES (@weight, @neuronId)", Connection);

    mySqlCommand->Parameters->AddWithValue("@weight", GetRand());
    mySqlCommand->Parameters->AddWithValue("@neuronId", neuronId);

    mySqlCommand->ExecuteNonQuery();

    return GetLastId();
}

int Database::CreateLayer(int networkId, NewLayerTemplate layerTemplate, int weightsCount)
{
    layerTemplate.Validate();

    int layerId = InsertLayer(networkId);

    for (int i = 0; i < layerTemplate.NeuronCount; i++)
        CreateNeuron(layerId, layerTemplate.NeuronTypeId, weightsCount);

    return layerId;
}

int Database::CreateNeuron(int layerId, String^ typeId, int weightsCount)
{
    int neuronId = InsertNeuron(layerId, typeId);

    for (int i = 0; i < weightsCount; i++)
        InsertWeight(neuronId);

    return neuronId;
}

void Database::UpdateLayer(Layer* layer, int layerId)
{
    std::vector<int> layerNeuronsIds = GetLayerNeuronsIds(layerId);

    if (layerNeuronsIds.empty())
        throw gcnew Exception("Error");

    if (layerNeuronsIds.size() != layer->GetNeurons().size())
        throw gcnew Exception("Error");

    for (int i = 0; i < layerNeuronsIds.size(); i++)
        UpdateNeuron(layer->GetNeurons()[i], layerNeuronsIds[i]);
}

void Database::UpdateNeuron(Neuron* neuron, int neuronId)
{
    std::vector<int> layerNeuronWeightsIds = GetLayerNeuronWeightsIds(neuronId);

    if (layerNeuronWeightsIds.empty())
        throw gcnew Exception("Error");

    if (layerNeuronWeightsIds.size() != neuron->GetWeights().size())
        throw gcnew Exception("Error");

    UpdateBias(neuron, neuronId);

    for (int i = 0; i < layerNeuronWeightsIds.size(); i++)
        UpdateWeight(neuron->GetWeights()[i], layerNeuronWeightsIds[i]);
}

void Database::UpdateBias(Neuron* neuron, int neuronId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("UPDATE neurons SET bias = @bias WHERE id = @id", Connection);

    mySqlCommand->Parameters->AddWithValue("@bias", neuron->GetBias());
    mySqlCommand->Parameters->AddWithValue("@id", neuronId);

    mySqlCommand->ExecuteNonQuery();
}

void Database::UpdateWeight(double weight, int weightId)
{
    MySqlCommand^ mySqlCommand = gcnew MySqlCommand("UPDATE weights SET weight = @weight WHERE id = @id", Connection);

    mySqlCommand->Parameters->AddWithValue("@weight", weight);
    mySqlCommand->Parameters->AddWithValue("@id", weightId);

    mySqlCommand->ExecuteNonQuery();
}

double Database::GetRand()
{
    return Rand->NextDouble() * 2.0 - 1.0;
}
