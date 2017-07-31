#include "InputManager.h"



InputManager::InputManager()
{
	currentKeyboardState = SDL_GetKeyboardState(NULL);
}


InputManager::~InputManager()
{
}

bool InputManager::HandleInput()
{
	bool quit = false;
	while (SDL_PollEvent(&e) != 0 && e.key.repeat == 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				break;
			}
		}
		if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_RIGHT:
				break;
			}
		}
	}
	if (currentKeyboardState[SDL_SCANCODE_UP])
	{
	}
	if (currentKeyboardState[SDL_SCANCODE_LEFT])
	{
	}
	if (currentKeyboardState[SDL_SCANCODE_RIGHT])
	{
	}
	return quit;
}
