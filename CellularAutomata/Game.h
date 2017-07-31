#pragma once
#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include "Texture.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Board.h"
class Game
{
public:
	Game();
	~Game();
	void Start();
private:
	bool Initialize();
	int screenWidth;
	int screenHeight;
	int levelWidth;
	int levelHeight;

	Renderer* renderer;
	InputManager* inputManager;
	Board* board;

	void MakeStatic(Texture* texture, Uint32 color1, Uint32 color2);
};

