#include "NeuralNetwork.h"
#include "ofApp.h"
#include <functional>

NeuralNetwork::NeuralNetwork(unsigned int inputCount, unsigned int hiddenCount, unsigned int outputCount)
{
	this->InputCount = inputCount;
	this->HiddenLayerCounts = vector<unsigned int>();
	this->HiddenLayerCounts.push_back(hiddenCount);
	this->OutputCount = outputCount;
	InitializeMatrices();
	RandomizeMatrices();
}

NeuralNetwork::NeuralNetwork(unsigned int inputCount, vector<unsigned int> hiddenLayers, unsigned int outputCount)
{
	this->InputCount = inputCount;
	this->HiddenLayerCounts = hiddenLayers;
	this->OutputCount = outputCount;
	InitializeMatrices();
	RandomizeMatrices();
}

NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::InitializeMatrices()
{
	// ------------- Weight Matrices -------------
	// There are weight connections between each layer, then between input and between output.
	Weights = vector<nmMatrix>();
	Weights.resize(HiddenLayerCounts.size() + 1);

	// First weight matrix is always the input weights
	Weights[0] = nmMatrix(HiddenLayerCounts[0], InputCount);

	// Last weight matrix is always the output weights
	Weights[Weights.size() - 1] = nmMatrix(OutputCount, HiddenLayerCounts[HiddenLayerCounts.size() - 1]);

	// If there is more than one hidden layer:
	// Loop through all but the first and last matrices to create weights for hidden layers
	for (unsigned int i = 1; i < HiddenLayerCounts.size(); i++)
	{
		// First matrix is always the input weights
		// Last matrix is always the output weights
		Weights[i] = nmMatrix(HiddenLayerCounts[i], HiddenLayerCounts[i - 1]);
	}

	// ------------- Bias Matrix -------------
	// Each layer, except input, has its own bias
	Biases = nmMatrix(HiddenLayerCounts.size() + 1, 1);
}

static float ActivationSigmoid(float x, int i, int j)
{
	return 1 / (1 + glm::exp(-x));
}

static float ActivationRelu(float x, int i, int j)
{
	// Implementation from https://machinelearningmastery.com/rectified-linear-activation-function-for-deep-learning-neural-networks/
	if (x > 0)
	{
		return x;
	}
	else
	{
		return 0;
	}
}

nmMatrix NeuralNetwork::Feedforward(nmMatrix& input, ActivationFunction activationFunction)
{
	// Steps:
	// Perform initial input calculation
	// Loop through the hidden layer calculations
	// Perform the output calculation
	// Return

	// First weight matrix * input matrix + bias = Layer 2
	nmMatrix layer2 = nmMatrix::Multiply(Weights[0], input);
	layer2.Add(Biases.Matrix[0][0]);

	if (activationFunction == ActivationFunction::Sigmoid)
	{
		layer2.MapFunction(ActivationSigmoid);
	}
	else if (activationFunction == ActivationFunction::Relu)
	{
		layer2.MapFunction(ActivationRelu);
	}

	// Loop through each hidden layer
	for (unsigned int i = 1; i < this->HiddenLayerCounts.size(); i++)
	{
		layer2 = nmMatrix::Multiply(Weights[i], layer2);
		layer2.Add(Biases.Matrix[i][0]);

		if (activationFunction == ActivationFunction::Sigmoid)
		{
			layer2.MapFunction(ActivationSigmoid);
		}
		else if (activationFunction == ActivationFunction::Relu)
		{
			layer2.MapFunction(ActivationRelu);
		}
	}

	// Perform final calculation for the output
	nmMatrix output = nmMatrix(OutputCount, 1);
	output = nmMatrix::Multiply(Weights[HiddenLayerCounts.size()], layer2);
	output.Add(Biases.Matrix[HiddenLayerCounts.size()][0]);

	if (activationFunction == ActivationFunction::Sigmoid)
	{
		output.MapFunction(ActivationSigmoid);
	}
	else if (activationFunction == ActivationFunction::Relu)
	{
		output.MapFunction(ActivationRelu);
	}

	return output;
}

void NeuralNetwork::Mutate(float rate)
{
	for (unsigned int s = 0; s < Weights.size(); s++)
	{
		for (unsigned int i = 0; i < Weights[s].Rows; i++)
		{
			for (unsigned int j = 0; j < Weights[s].Columns; j++)
			{
				if ((double)rand() / RAND_MAX < rate)
				{
					float mut = (((double)rand() / RAND_MAX) - 0.5) / 1.5;
					Weights[s].Matrix[i][j] += mut;
				}
			}
		}
	}

	for (unsigned int i = 0; i < Biases.Rows; i++)
	{
		for (unsigned int j = 0; j < Biases.Columns; j++)
		{
			if ((double)rand() / RAND_MAX < rate)
			{
				Biases.Matrix[i][j] += (((double)rand() / RAND_MAX) - 0.5) / 1.5;
			}
		}
	}
}

void NeuralNetwork::RandomizeMatrices()
{
	for (unsigned int i = 0; i < Weights.size(); i++)
	{
		Weights[i].Randomize();
	}
	Biases.Randomize();
}

NeuralNetwork NeuralNetwork::CopyNetwork()
{
	NeuralNetwork nn = NeuralNetwork(this->InputCount, this->HiddenLayerCounts, this->OutputCount);
	nn.Biases = this->Biases;
	nn.Weights = this->Weights;
	return nn;
}
