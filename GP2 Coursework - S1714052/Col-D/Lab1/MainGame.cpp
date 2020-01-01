#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Audio* audioDevice();

	auto start = std::chrono::system_clock::now();
	// Some computation here
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	deltaTime = elapsed_seconds.count();
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
	_gameDisplay.initDisplay(); 
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	transform.SetPos(glm::vec3(0, 0.5, 0));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));
	mesh1.loadModel("..\\res\\rocket.obj");
	//mesh2.loadModel("..\\res\\monkey3.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
	myCamera.Position_Camera(0, 0, -0.5f, 0, 0.5f, 0, 0, 1, 0);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//std::cerr << "x:" << mesh1.getSpherePos().x << "y:" << mesh1.getSpherePos().y << "z:" << mesh1.getSpherePos().z << std::endl;
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));

	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}

		switch (evnt.type)
		{
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_s:
			{
				transform.SetPos(glm::vec3(mesh1.getSpherePos().x, mesh1.getSpherePos().y - 0.01, mesh1.getSpherePos().z));
				break;
			}
			case SDLK_w:
			{
				transform.SetPos(glm::vec3(mesh1.getSpherePos().x, mesh1.getSpherePos().y + 0.01, mesh1.getSpherePos().z));
				break;
			}
			case SDLK_a:
			{
				transform.SetRot(glm::vec3(transform.GetRot()->x, transform.GetRot()->y, transform.GetRot()->z - 1));
				break;
			}
			case SDLK_d:
			{
				transform.SetRot(glm::vec3(transform.GetRot()->x, transform.GetRot()->y, transform.GetRot()->z + 1));
				break;
			}
			case SDLK_SPACE:
			{
				transform.SetPos(glm::vec3(0, 0, 0));
				break;
			}
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			}
		}
	}
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		playAudio(whistle, m1Pos);
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
	
	/*Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED*/
	
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	Shader shader("..\\res\\shader"); //new shader
	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture
	Texture texture2("..\\res\\rubber.jpg"); //load texture
	
	/*transform.SetPos(glm::vec3(sinf(counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.05, 0.05, 0.05));*/

	//transform.SetPos(glm::vec3(0, 0.5, 0));
	//transform.SetScale(glm::vec3(0.05, 0.05, 0.05));

	shader.Bind();
	shader.Update(transform, myCamera);
	//texture.Bind(1);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);
	

	/*transform.SetPos(glm::vec3(-sinf(counter), -0.5, -sinf(counter)*5));
	//transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetRot(glm::vec3(90.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCamera);
	//texture.Bind(1);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);*/
	counter = counter + 0.05f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 