#include "Game.h"


Game::Game()
{
	screenWidth = 800;
	screenHeight = 601;
	//levelWidth = 1000;
	//levelHeight = 1001;

	renderer = new Renderer(screenWidth, screenHeight, false);
	//mediaManager = new MediaManager();
	inputManager = new InputManager();
}


Game::~Game()
{
	delete renderer;
	//delete mediaManager;
	delete inputManager;
}

void Game::Start()
{
	if (Initialize())
	{
		bool quit = false;

		Uint32 lastFrameTime = 0;
		Uint32 currentFrameTime = 0;
		float deltaTime = 0.0f;
		currentFrameTime = SDL_GetTicks();


		Board board(screenWidth, screenHeight, renderer);


		while (quit == false)
		{
			//Calculate frame time
			lastFrameTime = currentFrameTime;
			currentFrameTime = SDL_GetTicks();
			deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
			std::cout<<1/deltaTime<<std::endl;
			
			//Get user input
			quit = inputManager->HandleInput();
			
			//Update board
			board.Update();

			//Draw board
			renderer->Draw(board.GetTexture());
			//SDL_Delay(30);
		}
	}
}

bool Game::Initialize()
{
	if (renderer->Initialize() == true)
	{
		//mediaManager->CreateTexture("Media/Sprites/megamanx.png", renderer->GetRenderer());
		return true;
	}
	else
	{
		return false;
	}
}


