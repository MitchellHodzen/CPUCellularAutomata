#pragma once
#include "Texture.h"
#include "SDL.h"

class Renderer;
class Board
{
	public:
		Board(int width, int height, Renderer* renderer);
		~Board();
		void Update();
		Texture* GetTexture();
	private:
		Texture* texture = new Texture();
		Uint32* buffer;
		SDL_PixelFormat* mappingFormat; 
		void MakeStatic(Uint32 color1, Uint32 color2);
		void CGOL();
		void MergeBuffer();
		int width;
		int height;
};
