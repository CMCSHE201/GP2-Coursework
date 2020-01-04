#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* player();
	Mesh* rock1();
	Mesh* rock2();
	Mesh* rock3();
	Mesh* water();
	Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	//Initialising everything used.
	_gameDisplay.initDisplay(); 
	collisionSound = audioDevice.loadSound("..\\res\\Collision.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	player.loadModel("..\\res\\Canoe.obj");
	rock1.loadModel("..\\res\\rock.obj");
	//mesh3.loadModel("..\\res\\Wheel.obj");
	
	//Initialises the camera pos and projection
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
}

void MainGame::gameLoop()
{
	//Used to keep track of time played
	auto frame1 = chrono::high_resolution_clock::now();
	auto frame2 = chrono::high_resolution_clock::now();

	//Loops whilst the game is in play
	while (_gameState != GameState::EXIT)
	{
		//Calculates elapsed time
		frame2 = chrono::high_resolution_clock::now();
		chrono::duration<float> elapsed = frame2 - frame1;
		frame1 = frame2;
		float fElapsed = elapsed.count();
		fullTime = fullTime + fElapsed;

		processInput();
		drawGame();
		collision(player.getSpherePos(), player.getSphereRadius(), rock1.getSpherePos(), rock1.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
 	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		//Closes the game if the gameState changes to EXIT
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}

		//Checks for Keyboard input
		switch (evnt.type)
		{
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_s:
			{
				down = true;
				break;
			}
			case SDLK_w:
			{
				up = true;
				break;
			}
			case SDLK_a:
			{
				left = true;
				break;
			}
			case SDLK_d:
			{
				right = true;
				break;
			}
			case SDLK_SPACE:
			{
				hit = false;
				break;
			}
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			}
		}
	}
}


//Checks for collisions between the player and the rocks
bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) *(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		playAudio(collisionSound, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}

void MainGame::drawGame()
{
	//Clears the display
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	Shader shader("..\\res\\shader"); //new shader
	Texture texture("..\\res\\wood.jpg"); //load texture
	//Texture texture1("..\\res\\rock.jpg"); //load texture
	//Texture texture2("..\\res\\water.jpg"); //load texture
	
	//Checks if the player is stationary and if so will continue to render the player in the same spot.
	if (!left && !right)
	{
		transform.SetPos(glm::vec3(player.getSpherePos().x, -1, 2));
		transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
		transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(0);
		player.draw();
		player.updateSphereData(*transform.GetPos(), 0.05f);
	}

	//Checks if the player is to move left and if so will offset the player to make it move to the left
	if (left)
	{
		transform.SetPos(glm::vec3(player.getSpherePos().x + 0.1, -1, 2));
		transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
		transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(0);
		player.draw();
		player.updateSphereData(*transform.GetPos(), 0.05f);
		left = false;
	}
	
	//Checks if the player is to move right and if so will offset the player to make it move to the right
	if (right)
	{
		transform.SetPos(glm::vec3(player.getSpherePos().x - 0.1, -1, 2));
		transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
		transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(0);
		player.draw();
		player.updateSphereData(*transform.GetPos(), 0.05f);
		right = false;
	}

	Texture texture1("..\\res\\rock.jpg"); //load texture
	//Checks to see if rock has passed the player and if so then will reset the position to the top of the screen with a random x value.
	if (rock1.getSpherePos().y > -4)
	{
		transform.SetPos(glm::vec3(rock1.getSpherePos().x, rock1.getSpherePos().y - 1, 2));
		transform.SetRot(glm::vec3(0.0, 0, 0));
		transform.SetScale(glm::vec3(1, 1, 1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(1);
		rock1.draw();
		rock1.updateSphereData(*transform.GetPos(), 0.5f);
	}
	else
	{
		float xVal = rand() % 2;

		transform.SetPos(glm::vec3(xVal, 5, 2));
		transform.SetRot(glm::vec3(0.0, 0, 0));
		transform.SetScale(glm::vec3(1, 1, 1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(1);
		rock1.draw();
		rock1.updateSphereData(*transform.GetPos(), 0.5f);
	}

	if (rock2.getSpherePos().y > -4)
	{
		transform.SetPos(glm::vec3(rock2.getSpherePos().x, rock2.getSpherePos().y - 1, 2));
		transform.SetRot(glm::vec3(0.0, 0, 0));
		transform.SetScale(glm::vec3(1, 1, 1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(1);
		rock1.draw();
		rock1.updateSphereData(*transform.GetPos(), 0.5f);
	}
	else
	{
		float xVal = rand() % 2;

		transform.SetPos(glm::vec3(xVal, 5, 2));
		transform.SetRot(glm::vec3(0.0, 0, 0));
		transform.SetScale(glm::vec3(1, 1, 1));

		shader.Bind();
		shader.Update(transform, myCamera);
		texture.Bind(1);
		rock1.draw();
		rock1.updateSphereData(*transform.GetPos(), 0.5f);
	}

	//Drawing the background.
	transform.SetPos(glm::vec3(0, 0, 3));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(1, 100, 1));

	shader.Bind();
	shader.Update(transform, myCamera);
	texture.Bind(2);
	water.draw();
	water.updateSphereData(*transform.GetPos(), 1.0f);
				
	cerr << water.getSpherePos().x << ", " << water.getSpherePos().y << ", " << water.getSpherePos().z << endl;
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 
