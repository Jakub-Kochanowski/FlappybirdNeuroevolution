#include "ofApp.h"
#include "GameWorld.h"
#include "Bird.h"
#include <vector>
#include <algorithm>

GameWorld world;
vector<Bird> Birds;
vector<Bird> LastBirds;
vector<Bird> BestBirds;
int BirdCount = 200;
int BestBirdCount = 20;
int MutatedFromBestBirds = 120;
float MutationRate = 0.1;
float Rate = 2;
int TopScore = 0;
int CurrentScore = 0;
int Generation = 1;

//--------------------------------------------------------------
void ofApp::setup() {
	world = GameWorld(ofGetWidth(), ofGetHeight());
	for (unsigned int i = 0; i < BirdCount; i++)
	{
		Bird temp = Bird(ofGetWidth(), ofGetHeight(), &world);
		Birds.push_back(temp);
	}
}

void CalculateFitness(unsigned int index)
{
	Birds[index].Fitness += 1; // Fitness calculated by time alive should be good enough
}

void GetBestBirds()
{
	std::sort(LastBirds.begin(), LastBirds.end());

	for (int i = LastBirds.size() - 1; i >= LastBirds.size() - BestBirdCount; i--)
	{
		if (LastBirds[i].Score > TopScore)
		{
			TopScore = LastBirds[i].Score;
		}
		Birds.push_back(LastBirds[i]);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	for (unsigned int i = 0; i < Rate; i++)
	{
		world.Update();
		ofSetWindowTitle("FPS: " + to_string(ofGetFrameRate()) + " Generation: " + to_string(Generation) + " Current Score: " + to_string(CurrentScore) + " Top Score: " + to_string(TopScore) + " Rate: " + to_string(Rate) + " Bird Count: " + to_string(Birds.size()));
		for (unsigned int i = 0; i < Birds.size(); i++)
		{
			if (!Birds[i].Dead)
			{
				Birds[i].Update();
				CalculateFitness(i);
			}
			else
			{
				LastBirds.push_back(Birds[i]);
				Birds.erase(Birds.begin() + i);
			}
		}

		if (Birds.size() != 0)
		{
			for (unsigned int i = 0; i < world.PipeList.size(); i++)
			{
				if (world.PipeList[i].Counted == false && world.PipeList[i].Position.x < Birds[0].Position.x)
				{
					CurrentScore++;
					world.PipeList[i].Counted = true;
				}
			}
		}

		if (Birds.size() == 0)
		{
			world.Reset();
			GetBestBirds();
			LastBirds.clear();

			if (CurrentScore > TopScore)
			{
				TopScore = CurrentScore;
			}

			CurrentScore = 0;

			for (unsigned int i = 0; i < Birds.size(); i++)
			{
				//Birds[i].Brain.Mutate(MutationRate);
				Birds[i].Reset();
			}

			while (Birds.size() - BestBirdCount < MutatedFromBestBirds)
			{
				unsigned int randBird = glm::floor(ofRandom(0, BestBirdCount));
				Birds.push_back(Birds[randBird]);
				Birds[randBird].Reset();
				Birds[randBird].Brain.Mutate(MutationRate);
			}

			while (Birds.size() < BirdCount)
			{
				Bird b = Bird(ofGetWidth(), ofGetHeight(), &world);
				Birds.push_back(b);
			}

			BestBirds.clear();
			Generation++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	world.Draw();
	for (unsigned int i = 0; i < Birds.size(); i++)
	{
		Birds[i].Draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'w')
	{
		Rate++;
	}
	if (key == 's')
	{
		Rate--;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}