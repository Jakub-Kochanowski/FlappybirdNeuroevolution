#pragma once
#include <functional>
#include <vector>

class nmMatrix {
public:
	nmMatrix();
	nmMatrix(unsigned int rows, unsigned int columns);
	~nmMatrix();

public:
	void Scale(float n); // Element-wise scale the matrix by a number
	void Scale(nmMatrix& m); // Element-wise scale the matrix by a matrix. Matrix must be the same size
	void Multiply(nmMatrix& m);
	void Add(float n); // Element-wise addition by a single number
	void Add(nmMatrix& m); // Element-wise addition by a matrix. Matrix must be the same size
	void Subtract(float n);
	void Subtract(nmMatrix& m);
	void Randomize(float scale = 2.0, bool Integer = false); // Randomize each element in the matrix from -Scale/2 to Scale/2
	void Print(); // Print the matrix using cout
	void Transpose(); // Switches rows and columns of the matrix
	void Map(float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false); // Maps the values of the matrix to a specified range
	void MapFunction(std::function<float(float, int, int)> f); // Maps a function to each value of the matrix // Found on https://stackoverflow.com/questions/9410/how-do-you-pass-a-function-as-a-parameter-in-c by Richard

public:
	static nmMatrix Add(nmMatrix& m1, nmMatrix& m2);
	static nmMatrix Subtract(nmMatrix& m1, nmMatrix& m2);
	static nmMatrix Scale(nmMatrix& m1, nmMatrix& m2);
	static nmMatrix Transpose(nmMatrix& m);
	static nmMatrix Multiply(nmMatrix m1, nmMatrix m2);
	static nmMatrix Map(nmMatrix& m, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
	static nmMatrix MapFunction(nmMatrix& m, std::function<float(float, int, int)> f);
	static nmMatrix FromArray(float arr[], unsigned int arrayLength);

public:
	unsigned int Rows;
	unsigned int Columns;
	std::vector<std::vector<float>> Matrix;
};