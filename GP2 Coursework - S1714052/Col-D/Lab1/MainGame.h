#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include <chrono>
#include <ctime> 

enum class GameState{PLAY, EXIT};

using namespace std;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh player;
	Mesh rock1;
	Mesh rock2;
	Mesh rock3;
	Mesh water;
	Camera myCamera;
	Audio audioDevice;

	float counter;
	float counter2;
	float counter3;
	float fullTime;

	bool shoot;
	bool hit;
	bool left;
	bool right;
	bool up;
	bool down;

	glm::vec2 bulletPos;

	unsigned int collisionSound;
	unsigned int backGroundMusic;
};

