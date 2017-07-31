#pragma once
#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>

class Renderer;
class Texture
{
public:
	Texture();
	~Texture();
	void LoadTexture(std::string path, Renderer* renderer);
	void CreateBlankTexture(int width, int height, Renderer* renderer); 
	void FreeTexture();
	void Render(SDL_Renderer* renderer);
	bool LockTexture();
	bool UnlockTexture();

	Uint32* GetPixels();
	int GetPitch();

	int GetHeight();
	int GetWidth();

	void ColorPixel(Uint32 x, Uint32 y, Uint32 color);
	void ColorPixel(Uint32 index, Uint32 color);
	Uint32 GetPixelColor(Uint32 x, Uint32 y);
	Uint32 UncheckedGetPixelColor(Uint32 x, Uint32 y);
private:
	SDL_Texture* texture;
	int width;
	int height;
	Uint32* pixels;
	int pitch;

};

