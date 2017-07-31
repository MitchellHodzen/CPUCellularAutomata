#pragma once
#include "SDL.h"
#include <thread>
class Renderer;
class Barrier;
class Texture;
class Board
{
	public:
		Board(int width, int height, Renderer* renderer);
		~Board();
		void Update(bool quit);
		Texture* GetTexture();
	private:
		Texture* texture;
		Uint32* buffer1;
		Uint32* buffer2;
		Uint32* currentBuffer;
		Uint32* currentBoard;
		SDL_PixelFormat* mappingFormat; 
		void MakeStatic(Uint32 color1, Uint32 color2);
		void CGOL(int rowIndex, int rowCount);
		void MergeBuffer(int rowIndex, int rowCount);
		void MergeBuffer();
		int width;
		int height;
		std::thread* threads;
		int threadCount;
		void SpawnThread(int index, int rowIndex, int rowCount);
		Barrier* readBarrier;
		Barrier* writeBarrier;
		Barrier* bufferBarrier;
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
		Uint32 GetBoardColor(Uint32 x, Uint32 y);
		bool endProgram = false;
};
