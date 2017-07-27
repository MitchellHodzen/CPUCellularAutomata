#pragma once
#include <SDL.h>
#include <stdio.h>
class InputManager
{
public:
	InputManager();
	~InputManager();
	bool HandleInput();
private:
	SDL_Event e;
	const Uint8* currentKeyboardState;
};

