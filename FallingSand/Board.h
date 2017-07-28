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
		void CGOL();
		void MergeBuffer();
		int width;
		int height;
		std::thread* threads;
		int threadCount;
		void SpawnThread(int index, int rowIndex, int rowCount);
		//std::mutex updateBufferMutex;
		std::mutex readBufferMutex;
		std::mutex writeBufferMutex;
		std::condition_variable readBufferConditionVariable;
		std::condition_variable writeBufferConditionVariable;
		int counter = 0;
		bool isReading;
		bool isWriting;
		Barrier* currentBarrier;
		std::once_flag* currentFlag = new std::once_flag();
};
