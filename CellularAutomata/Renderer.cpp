#include "Renderer.h"



Renderer::Renderer(int screenWidth, int screenHeight, bool debug)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->debug = debug;
}


Renderer::~Renderer()
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	sdlRenderer = NULL;
	sdlWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

//void Renderer::Render(Entity* entity, Camera* camera)
//{
	//entity->GetVisualComponent()->Render((int)entity->GetPosX() - (int)camera->GetPosX(), (int)entity->GetPosY() - (int)camera->GetPosY(), sdlRenderer);
	//if (debug == true)
	//{
	//}
//}
SDL_Window* Renderer::GetWindow()
{
	return sdlWindow;
}
void Renderer::Draw(Texture* texture)
{
	SDL_RenderClear(sdlRenderer);
	texture->Render(sdlRenderer);	
	//for (unsigned i = 0; i < entityList->size(); ++i)
	//{
		//Render((*entityList)[i], camera);
	//}
	SDL_RenderPresent(sdlRenderer);
}
void Renderer::SetRenderer(SDL_Renderer* renderer)
{
	sdlRenderer = renderer;
}
SDL_Renderer* Renderer::GetRenderer()
{
	return sdlRenderer;
}

bool Renderer::Initialize()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized. SDL error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		sdlWindow = SDL_CreateWindow("Falling Sand Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (sdlWindow == NULL)
		{
			std::cout << "SDL Window could not be created. SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN);
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
			if (sdlRenderer == NULL)
			{
				std::cout << "SDL Renderer could not be created. SDL error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags)) & imgFlags)
				{
					std::cout << "SDL Img could not be initialized. SDL Img error: " << IMG_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	return success;
}
