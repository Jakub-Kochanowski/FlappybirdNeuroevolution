#include "GameWorld.h"

GameWorld::GameWorld()
{
	this->WindowWidth = 100;
	this->WindowHeight = 100;
}

GameWorld::GameWorld(float windowWidth, float windowHeight)
{
	this->WindowWidth = windowWidth;
	this->WindowHeight = windowHeight;
	
	Pipe p = Pipe();
	float h = ofRandom(WindowHeight - (PipeSpace * 2)) + PipeSpace + 25;
	p.Position = ofVec2f(WindowWidth + PipeWidth * 1.5, h);
	PipeList.push_back(p);
}

GameWorld::~GameWorld()
{
}

void GameWorld::Update()
{
	PipeRateCounter++;
	if (PipeRateCounter >= PipeSpawnRate)
	{
		PipeRateCounter = 0;
		Pipe p = Pipe();
		float h = ofRandom(WindowHeight - (PipeSpace*2)) + PipeSpace + 25;
		p.Position = ofVec2f(WindowWidth + PipeWidth * 1.5, h);
		PipeList.push_back(p);
	}

	for (unsigned int i = 0; i < PipeList.size(); i++)
	{
		PipeList[i].Position.x -= PipeMovementSpeed;
	}

	for (int i = PipeList.size() - 1; i >= 0; i--) // Standard reverse for loop for removal of objects
	{
		if (PipeList[i].Position.x < 0 - PipeWidth)
		{
			PipeList.erase(PipeList.begin() + i);
		}
	}
}

void GameWorld::Draw()
{
	ofSetColor(ofColor::green);
	for (unsigned int i = 0; i < PipeList.size(); i++)
	{
		ofDrawRectangle(PipeList[i].Position.x, 0, PipeWidth, PipeList[i].Position.y - PipeSpace);
		ofDrawRectangle(PipeList[i].Position.x, PipeList[i].Position.y + PipeSpace, PipeWidth, WindowHeight - PipeList[i].Position.y + PipeSpace);
	}

	//ofSetColor(ofColor::red);
	//ofDrawRectangle(PipeWidth - PipeWidth, 0, PipeWidth, 150 - PipeSpace);
	//ofSetColor(ofColor::blue);
	//ofDrawRectangle(PipeWidth - PipeWidth, 150 + PipeSpace, PipeWidth, WindowHeight - 150 + PipeSpace);
}

void GameWorld::Reset()
{
	PipeList.clear();
	this->PipeRateCounter = 0;
	Pipe p = Pipe();
	float h = ofRandom(WindowHeight - (PipeSpace * 2)) + PipeSpace + 25;
	p.Position = ofVec2f(WindowWidth + PipeWidth * 1.5, h);
	PipeList.push_back(p);
}
