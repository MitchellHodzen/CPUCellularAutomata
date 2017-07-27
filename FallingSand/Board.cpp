#include "Board.h"
#include "Renderer.h"

Board::Board(int width, int height, Renderer* renderer)
{
	texture->CreateBlankTexture(width, height, renderer);
	this->width = width;
	this->height = height;
	Uint32 format = SDL_GetWindowPixelFormat(renderer->GetWindow());
	mappingFormat = SDL_AllocFormat(format);
	Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
	Uint32 black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00);

	texture->LockTexture();
	MakeStatic(white, black);
	buffer = new Uint32[width * height];
	memcpy((void*)buffer, (void*) texture->GetPixels(), texture->GetPitch() * height);
	texture->UnlockTexture();

	int total = height;
	threadCount = std::thread::hardware_concurrency();
	std::cout<<"Number of threads: "<< threadCount <<std::endl;
	threads = new std::thread[threadCount];

	int count = 0;
	int rowsPerThread = height / threadCount;
	std::cout<<"Rows per thread: "<<rowsPerThread<<std::endl;
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
		threads[i] = std::thread(&Board::SpawnThread, this, i, rowIndex, rowCount);
	}
	std::cout<< "total = " << total << ". count = " << count<<std::endl;
}
Board::~Board()
{
	delete[] buffer;
	delete texture;
	for(Uint32 i = 0; i < threadCount; ++i)
	{
		if (threads[i].joinable())
		{
			std::cout<<"Joining thread " << i <<std::endl;
			threads[i].join();
			std::cout<<"Joined thread " << i <<std::endl;
		}
	}
	delete[] threads;	
	SDL_FreeFormat(mappingFormat);
}

void Board::SpawnThread(int index, int rowIndex, int rowCount)
{
	std::cout<<"Spawning thread " << index << " with row index of " << rowIndex << " and row count of " << rowCount << std::endl;
}

Texture* Board::GetTexture()
{
	return texture;
}
void Board::Update()
{
	texture->LockTexture();
	CGOL();
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
void Board::CGOL()
{
	//Conway's Game of Life
	Uint32 black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00);
	Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
	Uint32 color;
	int count = 0;
	for(Uint32 x = 0; x < width; ++x)
	{
		for (Uint32 y = 0; y < height; ++y)
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
