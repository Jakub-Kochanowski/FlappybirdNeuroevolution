#include "nmMatrix.h"
#include <iostream>
#include <string>
#include "ofApp.h"

nmMatrix::nmMatrix()
{
	this->Rows = 0;
	this->Columns = 0;
}

nmMatrix::nmMatrix(unsigned int rows, unsigned int columns)
{
	if (rows == 0 || columns == 0)
	{
		throw("Cannot have a matrix with a dimension of zero!");
	}
	else
	{
		this->Rows = rows;
		this->Columns = columns;
		//Matrix = vector<vector<float>>();
		Matrix.resize(Rows);
		for (int i = 0; i < Matrix.size(); i++)
		{
			Matrix[i].resize(Columns);
		}
	}
}

nmMatrix::~nmMatrix()
{

}

void nmMatrix::Scale(nmMatrix& m)
{
	if (m.Rows != this->Rows || m.Columns != this->Columns)
	{
		throw("Element-wise matrix multiplication requires matrices of the same size!");
	}
	else
	{
		for (unsigned int i = 0; i < Rows; i++)
		{
			for (unsigned int j = 0; j < Columns; j++)
			{
				Matrix[i][j] = Matrix[i][j] * m.Matrix[i][j];
			}
		}
	}
}

void nmMatrix::Multiply(nmMatrix& m)
{
	if (this->Columns != m.Rows)
	{
		throw("Columns of matrix m1 must match rows of matrix m2!");
	}
	else
	{
		for (unsigned int i = 0; i < this->Rows; i++)
		{
			for (unsigned int j = 0; j < m.Columns; j++)
			{
				float sum = 0;
				for (unsigned int k = 0; k < this->Columns; k++)
				{
					sum += this->Matrix[i][k] * m.Matrix[k][j];
				}
			}
		}
	}
}

void nmMatrix::Scale(float n)
{
	for (unsigned int i = 0; i < Rows; i++)
	{
		for (unsigned int j = 0; j < Columns; j++)
		{
			Matrix[i][j] = Matrix[i][j] * n;
		}
	}
}

void nmMatrix::Add(float n)
{
	for (unsigned int i = 0; i < Rows; i++)
	{
		for (unsigned int j = 0; j < Columns; j++)
		{
			Matrix[i][j] = Matrix[i][j] + n;
		}
	}
}

void nmMatrix::Add(nmMatrix& m)
{
	if (m.Rows != this->Rows || m.Columns != this->Columns)
	{
		throw("Matrix addition requires matrices of the same size!");
	}
	else
	{
		for (unsigned int i = 0; i < Rows; i++)
		{
			for (unsigned int j = 0; j < Columns; j++)
			{
				Matrix[i][j] = Matrix[i][j] + m.Matrix[i][j];
			}
		}
	}
}

void nmMatrix::Subtract(float n)
{
	for (unsigned int i = 0; i < Rows; i++)
	{
		for (unsigned int j = 0; j < Columns; j++)
		{
			Matrix[i][j] -= n;
		}
	}
}

void nmMatrix::Subtract(nmMatrix& m)
{
	if (this->Rows != m.Rows || this->Columns != m.Columns)
	{
		throw("Matrix subtraction requires matrices of the same size!");
	}
	else
	{
		for (unsigned int i = 0; i < Rows; i++)
		{
			for (unsigned int j = 0; j < Columns; j++)
			{
				this->Matrix[i][j] -= m.Matrix[i][j];
			}
		}
	}
}

void nmMatrix::Randomize(float scale, bool Integer)
{
	// TODO: Fix integer random number generation
	if (Integer) {
		for (unsigned int i = 0; i < Rows; i++)
		{
			for (unsigned int j = 0; j < Columns; j++)
			{
				Matrix[i][j] = (int)(scale * ((double)rand() / RAND_MAX));
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < Rows; i++)
		{
			for (unsigned int j = 0; j < Columns; j++)
			{
				Matrix[i][j] = scale * ((double)rand() / RAND_MAX) - scale / 2;
			}
		}
	}
}

void nmMatrix::Transpose()
{
	nmMatrix m = nmMatrix(this->Columns, this->Rows);
	for (unsigned int i = 0; i < m.Rows; i++)
	{
		for (unsigned int j = 0; j < m.Columns; j++)
		{
			m.Matrix[j][i] = this->Matrix[i][j];
		}
	}

	Matrix.clear();
	Matrix.resize(Columns);
	for (unsigned int i = 0; i < Rows; i++)
	{
		Matrix[i].resize(Rows);
		for (unsigned int j = 0; j < Columns; j++)
		{
			Matrix[i][j] = m.Matrix[i][j];
		}
	}
}

void nmMatrix::Map(float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
{
	for (unsigned int i = 0; i < this->Rows; i++)
	{
		for (unsigned int j = 0; j < this->Columns; j++)
		{
			Matrix[i][j] = ofMap(Matrix[i][j], inputMin, inputMax, outputMin, outputMax, clamp);
		}
	}
}

void nmMatrix::MapFunction(std::function<float(float, int, int)> f)
{
	for (unsigned int i = 0; i < Rows; i++)
	{
		for (unsigned int j = 0; j < Columns; j++)
		{
			Matrix[i][j] = f(Matrix[i][j], i, j);
		}
	}
}

void nmMatrix::Print()
{
	std::cout << "\n---------------------------------------------------------\n";
	for (unsigned int i = 0; i < Rows; i++)
	{
		std::string currentRow = "";
		for (unsigned int j = 0; j < Columns; j++)
		{
			currentRow.append(std::to_string(Matrix[i][j]) + " | ");
		}
		std::cout << currentRow << "\n";
	}
	std::cout << "---------------------------------------------------------\n";
}

nmMatrix nmMatrix::Add(nmMatrix& m1, nmMatrix& m2)
{
	static unsigned int r = m1.Rows;
	static unsigned int c = m1.Columns;
	if (r != m2.Rows || c != m2.Columns)
	{
		throw("Matrix addition requires matrices of the same size!");
	}
	else
	{
		nmMatrix mat = nmMatrix(r, c);
		for (unsigned int i = 0; i < r; i++)
		{
			for (unsigned int j = 0; j < c; j++)
			{
				mat.Matrix[i][j] = m1.Matrix[i][j] + m2.Matrix[i][j];
			}
		}
		return mat;
	}
}

nmMatrix nmMatrix::Subtract(nmMatrix& m1, nmMatrix& m2)
{
	if (m1.Rows != m2.Rows || m1.Columns != m2.Columns)
	{
		throw("Matrix subtraction requires matrices of the same size!");
	}
	else
	{
		nmMatrix result = nmMatrix(m1.Rows, m1.Columns);
		for (unsigned int i = 0; i < m1.Rows; i++)
		{
			for (unsigned int j = 0; j < m1.Columns; j++)
			{
				result.Matrix[i][j] = m1.Matrix[i][j] - m2.Matrix[i][j];
			}
		}
		return result;
	}
}

nmMatrix nmMatrix::Scale(nmMatrix& m1, nmMatrix& m2)
{
	if (m1.Rows != m2.Rows || m1.Columns != m2.Columns)
	{
		throw("Matrix element-wise multiplication requires matrices of the same size!");
	}
	else
	{
		nmMatrix mat = nmMatrix(m1.Rows, m1.Columns);
		for (unsigned int i = 0; i < m1.Rows; i++)
		{
			for (unsigned int j = 0; j < m1.Columns; j++)
			{
				mat.Matrix[i][j] = m1.Matrix[i][j] * m2.Matrix[i][j];
			}
		}
		return mat;
	}
}

nmMatrix nmMatrix::Transpose(nmMatrix& m)
{
	nmMatrix result = nmMatrix(m.Columns, m.Rows);
	for (unsigned int i = 0; i < m.Rows; i++)
	{
		for (unsigned int j = 0; j < m.Columns; j++)
		{
			result.Matrix[j][i] = m.Matrix[i][j];
		}
	}
	return result;
}

nmMatrix nmMatrix::Multiply(nmMatrix m1, nmMatrix m2)
{
	nmMatrix result = nmMatrix(m1.Rows, m2.Columns);
	if (m1.Columns != m2.Rows)
	{
		throw("Columns of matrix m1 must match rows of matrix m2!");
	}
	else
	{
		for (unsigned int i = 0; i < m1.Rows; i++)
		{
			for (unsigned int j = 0; j < m2.Columns; j++)
			{
				float sum = 0;
				for (unsigned int k = 0; k < m1.Columns; k++)
				{
					sum += m1.Matrix[i][k] * m2.Matrix[k][j];
				}
				result.Matrix[i][j] = sum;
			}
		}
	}
	return result;
}

nmMatrix nmMatrix::Map(nmMatrix& m, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
{
	nmMatrix r = nmMatrix(m.Rows, m.Columns);
	for (unsigned int i = 0; i < m.Rows; i++)
	{
		for (unsigned int j = 0; j < m.Columns; j++)
		{
			r.Matrix[i][j] = ofMap(m.Matrix[i][j], inputMin, inputMax, outputMin, outputMax, clamp);
		}
	}
	return r;
}
nmMatrix nmMatrix::MapFunction(nmMatrix& m, std::function<float(float, int, int)> func)
{
	nmMatrix result = nmMatrix(m.Rows, m.Columns);
	for (unsigned int i = 0; i < m.Rows; i++)
	{
		for (unsigned int j = 0; j < m.Columns; j++)
		{
			result.Matrix[i][j] = func(m.Matrix[i][j], i, j);
		}
	}
	return result;
}

nmMatrix nmMatrix::FromArray(float arr[], unsigned int arraySize)
{
	nmMatrix result = nmMatrix(arraySize, 1);
	for (unsigned int j = 0; j < arraySize; j++)
	{
		result.Matrix[j][0] = arr[j];
	}
	return result;
}