#include "Board.h"
#include "Renderer.h"

Board::Board(int width, int height, Renderer* renderer)
{
	//Set up texture (what is seen) 
	texture->CreateBlankTexture(width, height, renderer);
	this->width = width;
	this->height = height;

	//Get pixel format, used to generate pixel colors
	Uint32 format = SDL_GetWindowPixelFormat(renderer->GetWindow());
	mappingFormat = SDL_AllocFormat(format);
	white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
	black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00);

	//Add random black pixels to the texture
	texture->LockTexture();
	MakeStatic(white, black);
	//Copy the texture to the buffer. The buffer always has the state of the board from the previous frame
	buffer = new Uint32[width * height];
	memcpy((void*)buffer, (void*) texture->GetPixels(), texture->GetPitch() * height);
	texture->UnlockTexture();

	//Separating the board into pieces by row to be passed evenly to threads
	int total = height;
	threadCount = std::thread::hardware_concurrency();
	threads = new std::thread[threadCount];
	readBarrier = new Barrier(threadCount + 1);
	writeBarrier = new Barrier(threadCount + 1);

	int count = 0;
	int rowsPerThread = height / threadCount;
	for (int i = 0; i < threadCount; ++i)
	{
		int rowIndex = i * rowsPerThread;
		int rowCount = rowsPerThread;
		count += rowCount;
		if (i == threadCount -1 && total - count != 0)
		{
			rowCount += total-count;
			count += total-count; 
		}
		//Thread is spawned
		threads[i] = std::thread(&Board::SpawnThread, this, i, rowIndex, rowCount);
	}
}
Board::~Board()
{
	delete[] buffer;
	delete texture;
	
	for(Uint32 i = 0; i < threadCount; ++i)
	{
		if (threads[i].joinable())
		{
			threads[i].detach();
		}
	}

	delete[] threads;	
	delete readBarrier;
	delete writeBarrier;
	SDL_FreeFormat(mappingFormat);
}

void Board::SpawnThread(int index, int rowIndex, int rowCount)
{
	while(true)
	{
		//Waits until the buffer is coppied and the texture is unlocked in the update thread
		writeBarrier->Wait(threadCount + 1);
		CGOL(rowIndex, rowCount);
		//Lets the board know the buffer is written
		readBarrier->Wait(threadCount + 1);
	}
	
}

Texture* Board::GetTexture()
{
	return texture;
}
void Board::Update()
{
	//Wait until the buffer is completely written
	texture->LockTexture();
	writeBarrier->Wait(threadCount + 1);
	readBarrier->Wait(threadCount + 1);
	//Write the buffer to the texture
	MergeBuffer();
	texture->UnlockTexture();
	
}


void Board::MergeBuffer()
{
	for(Uint32 i = 0; i < width * height; ++i)
	{
		texture->ColorPixel(i, buffer[i]);
	}
}	
void Board::CGOL(int rowIndex, int rowCount)
{
	//Conway's Game of Life
	Uint32 color;
	int count = 0;
	for(Uint32 x = 0; x < width; ++x)
	{
		for (Uint32 y = rowIndex; y < rowIndex + rowCount; ++y)
		{
			count = 0;

			if (texture->UncheckedGetPixelColor(x - 1, y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x , y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x - 1, y ) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y) == black) count++;
			if (texture->UncheckedGetPixelColor(x - 1, y + 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x , y + 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y + 1) == black) count++;

			if (count == 3 || (count == 2 && texture->GetPixelColor(x, y) == black))
			{
				color = black;
			}
			else
			{
				color = white;
			}
		
			buffer[x + (y * width)] = color;

		}
	}
}	
void Board::MakeStatic(Uint32 color1, Uint32 color2)
{
	for(Uint32 x = 0; x < texture->GetWidth(); ++x)
	{
		for (Uint32 y = 0; y < texture->GetHeight(); ++y)
		{
			int test = rand()%100;
			if (test < 80)
			{
				texture->ColorPixel(x, y, color1);
			}
			else
			{
				texture->ColorPixel(x, y, color2);
			}
		}
	}
}
