#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include "Texture.h"
class Renderer
{
public:
	Renderer(int screenWidth, int screenHeight, bool debug);
	~Renderer();
	//void Render(Entity* entity, Camera* camera);
	SDL_Renderer* GetRenderer();
	void SetRenderer(SDL_Renderer* renderer);
	void Draw(Texture* texture);
	bool Initialize();
	SDL_Window* GetWindow();
private:
	SDL_Renderer* sdlRenderer;
	SDL_Window* sdlWindow;
	int screenWidth;
	int screenHeight;
	bool debug;
};

