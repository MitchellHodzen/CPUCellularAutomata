#include "Game.h"


Game::Game()
{
	screenWidth = 800;
	screenHeight = 600;

	renderer = new Renderer(screenWidth, screenHeight, false);
	//mediaManager = new MediaManager();
	inputManager = new InputManager();
}


Game::~Game()
{
	delete renderer;
	//delete mediaManager;
	delete inputManager;
	delete board;
}

void Game::Start()
{
	if (Initialize())
	{
		float averageFramerate = 0;
		float averageSimFramerate = 0;
		int frames = 0;
		bool quit = false;

		//How often the board updates, must be a low enough framerate to ensure update finishes in time
		const float fixedTimeStep = 1.0/30.0;
		float physicsTimer = 0;

		Uint32 lastFrameTime = 0;
		Uint32 currentFrameTime = 0;
		float deltaTime = 0.0f;
		currentFrameTime = SDL_GetTicks();


		Uint32 lastSimFrameTime = 0;
		Uint32 currentSimFrameTime = 0;
		float simDeltaTime = 0.0f;
		currentSimFrameTime = SDL_GetTicks();


		board = new Board(screenWidth, screenHeight, renderer);


		while (quit == false)
		{
			//Calculate frame time
			lastFrameTime = currentFrameTime;
			currentFrameTime = SDL_GetTicks();
			deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
			averageFramerate += 1/deltaTime;
			frames++;
			
			//Get user input
			quit = inputManager->HandleInput();
			
			//Fixed time step for calculating change in board
			physicsTimer += deltaTime;
			//while (physicsTimer >= fixedTimeStep)
			//{
				lastSimFrameTime = currentSimFrameTime;
				currentSimFrameTime = SDL_GetTicks();
				simDeltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;
				averageSimFramerate += 1/simDeltaTime;
				//Update board
				board->Update(quit);
				physicsTimer -= fixedTimeStep;
			//}
				
			//Draw board
			renderer->Draw(board->GetTexture());
			//SDL_Delay(30);
		}
		averageFramerate = averageFramerate / frames;
		averageSimFramerate = averageSimFramerate / frames;
		std::cout<<"Average Framerate: " << averageFramerate << std::endl;
		std::cout<<"Average Sim Framerate: " << averageSimFramerate << std::endl;
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


