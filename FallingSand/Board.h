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
		SDL_PixelFormat* mappingFormat; 
		void MakeStatic(Uint32 color1, Uint32 color2);
		void CGOL(int rowIndex, int rowCount);
		void MergeBuffer();
		int width;
		int height;
		std::thread* threads;
		int threadCount;
		void SpawnThread(int index, int rowIndex, int rowCount);
		Barrier* readBarrier;
		Barrier* writeBarrier;
		Uint32 white;
		Uint32 black;
		Uint32 blue;
		Uint8* waterBoardBuffer1;
		Uint8* waterBoardBuffer2;
		Uint8* waterBoard;
		Uint8* waterBuffer;
		Uint8 GetWater(Uint32 x, Uint32 y, Uint8* board);	
		void SetWater(Uint32 x, Uint32 y, Uint8 amount, Uint8* board);
		void SimulateWater(int rowIndex, int rowCount);
};
