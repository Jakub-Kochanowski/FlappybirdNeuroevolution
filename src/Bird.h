#pragma once
#include "NeuralNetwork.h"
#include "ofApp.h"
#include "GameWorld.h"

class Bird
{
public:
	Bird();
	Bird(float windowWidth, float windowHeight, GameWorld* world);
	~Bird();

public:
	void Update();
	void Draw();
	void Reset();
	void Jump();
	void PerformMovement();
	nmMatrix GetBrainInputs();
	void CheckForCollision();

public:
	ofVec2f Position;
	float Velocity;
	bool Dead = false;

	int Score = 0;
	float Fitness = 0;
	bool operator< (const Bird& b) const {
		return this->Fitness < b.Fitness;
	};

public:
	NeuralNetwork Brain = NeuralNetwork(1, 1, 1);
	GameWorld* World;

public:
	float WindowWidth;
	float WindowHeight;
	float BirdRadius = 25;
	float Gravity = 8000;
	float deltaTime = 0.01;
	float JumpVelocity = 1500;
};