#include "nmMatrix.h";
#include <vector>

using namespace std;

class NeuralNetwork {
public:
	NeuralNetwork(unsigned int inputCount, unsigned int hiddenCount, unsigned int outputCount);
	NeuralNetwork(unsigned int inputCount, vector<unsigned int> hiddenLayers, unsigned int outputCount);
	~NeuralNetwork();

public:
	enum ActivationFunction {
		Relu,
		Sigmoid
	};

protected:
	vector<nmMatrix> Weights;
	nmMatrix Biases;
	unsigned int InputCount;
	unsigned int OutputCount;
	vector<unsigned int> HiddenLayerCounts;

public:
	nmMatrix Feedforward(nmMatrix& input, ActivationFunction activationFunction);
	void Mutate(float rate);
	void RandomizeMatrices();
	NeuralNetwork CopyNetwork();

private:
	void InitializeMatrices();
};