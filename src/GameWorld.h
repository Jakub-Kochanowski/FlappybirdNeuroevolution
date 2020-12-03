#include "ofApp.h"
#include <vector>

#pragma once
class GameWorld
{
private:
	struct Pipe
	{
		ofVec2f Position;
		bool Counted = false;
	};

public:
	GameWorld();
	GameWorld(float windowWidth, float windowHeight);
	~GameWorld();

public:
	void Update();
	void Draw();
	void Reset();

public:
	float PipeSpace = 100;
	float PipeMovementSpeed = 10;
	float PipeSpawnRate = 150;
	float PipeWidth = 150;
	vector<Pipe> PipeList;

private:
	float WindowWidth;
	float WindowHeight;
	float PipeRateCounter = 0;
};