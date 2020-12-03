#include "Bird.h"
#include <vector>

Bird::Bird()
{
	// Not sure how to properly handle default constructors in C++, but does not really matter here, I guess
	this->WindowWidth = 100;
	this->WindowHeight = 100;
	this->Position = ofVec2f(25, 50);
	this->Velocity = 0;

	this->Brain = NeuralNetwork(1, 1, 1);
}

Bird::Bird(float windowWidth, float windowHeight, GameWorld* world)
{

	this->WindowWidth = windowWidth;
	this->WindowHeight = windowHeight;
	this->Position = ofVec2f(WindowWidth / 4, WindowHeight / 2);
	this->World = world;
	this->Velocity = 0;

	std::vector<unsigned int> layers = std::vector<unsigned int>();
	layers.push_back(24);

	this->Brain = NeuralNetwork(5, layers, 2);
	this->Fitness = 0;
}

void Bird::Reset()
{
	this->Dead = false;
	this->Position = ofVec2f(WindowWidth / 4, WindowHeight / 2);
	this->Velocity = 0;
	this->Fitness = 0;
	this->Score = 0;
}


Bird::~Bird()
{

}

void Bird::Update()
{
	if (!Dead)
	{
		this->Velocity += this->Gravity * deltaTime;
		this->Position.y += this->Velocity * deltaTime;
		this->PerformMovement();
		this->CheckForCollision();
	}
}

void Bird::Draw()
{
	if (!Dead)
	{
		ofSetColor(ofColor::yellow, 255 / 4);
		ofDrawCircle(this->Position.x, this->Position.y, BirdRadius);
	}
}

void Bird::Jump()
{
	this->Velocity = -this->JumpVelocity;
}

void Bird::PerformMovement()
{
	nmMatrix inputs = GetBrainInputs();
	nmMatrix outputs = Brain.Feedforward(inputs, NeuralNetwork::ActivationFunction::Relu);

	float firstOut = outputs.Matrix[0][0];
	float secondOut = outputs.Matrix[1][0];

	if (firstOut > secondOut)
	{
		this->Jump();
	}
}

nmMatrix Bird::GetBrainInputs()
{
	unsigned int closestPipeIndex = 0;
	float tempXDist = 500; // Wont ever exceed around 4 pipes, anyway
	for (unsigned int i = 0; i < this->World->PipeList.size(); i++)
	{
		if (this->World->PipeList[i].Position.x > this->Position.x)
		{
			if (this->World->PipeList[i].Position.x < tempXDist)
			{
				closestPipeIndex = i;
				tempXDist = World->PipeList[i].Position.x;
			}
		}
	}

	// Normalize inputs to window dimensions?
	float arr[5] = {
		this->Velocity / 10000, // Arbitrary
		this->Position.x / WindowWidth,
		this->Position.y / WindowHeight,
		(this->World->PipeList[closestPipeIndex].Position.x - this->Position.x) / WindowWidth,
		(this->Position.y - this->World->PipeList[closestPipeIndex].Position.y) / WindowHeight
	};

	return nmMatrix::FromArray(arr, 5);
}

void Bird::CheckForCollision()
{
	if (this->Position.y > WindowHeight)
	{
		this->Dead = true;
	}

	// This can definitely be optimized, but it doesn't really matter here
	for (unsigned int i = 0; i < World->PipeList.size(); i++) // This might go bad, maybe?
	{
		if (this->Position.x + this->BirdRadius >= World->PipeList[i].Position.x && this->Position.x + this->BirdRadius <= World->PipeList[i].Position.x + World->PipeWidth)
		{
			if (this->Position.y + this->BirdRadius > World->PipeList[i].Position.y + World->PipeSpace || this->Position.y - this->BirdRadius < World->PipeList[i].Position.y - World->PipeSpace)
			{
				this->Dead = true;
			}
		}
	}
}