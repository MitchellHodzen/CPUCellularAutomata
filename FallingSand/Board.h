#pragma once
#include "Texture.h"
#include "SDL.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Barrier.h"
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
		Uint32* boardCopy;
		SDL_PixelFormat* mappingFormat; 
		void MakeStatic(Uint32 color1, Uint32 color2);
		void CGOL(int rowIndex, int rowCount);
		void MergeBuffer();
		int width;
		int height;
		std::thread* threads;
		int threadCount;
		void SpawnThread(int index, int rowIndex, int rowCount);
		Barrier* currentBarrier;
		Barrier* writeBarrier;
		bool exiting = false;
};
