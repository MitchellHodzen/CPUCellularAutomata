#include "Board.h"
#include "Renderer.h"
#include <thread>
#include "Texture.h"
#include "Barrier.h"
Board::Board(int width, int height, Renderer* renderer)
{
	//Set up texture (what is seen) 
	texture = new Texture();
	texture->CreateBlankTexture(width, height, renderer);
	this->width = width;
	this->height = height;

	//Get pixel format, used to generate pixel colors
	Uint32 format = SDL_GetWindowPixelFormat(renderer->GetWindow());
	mappingFormat = SDL_AllocFormat(format);
	white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
	black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00);
	blue = SDL_MapRGBA(mappingFormat, 0x00, 0x00, 0x80, 0x00);

	//Add random black pixels to the texture
	texture->LockTexture();
	MakeStatic(white, black);
	//Copy the texture to the buffer. The buffer always has the state of the board from the previous frame
	buffer1 = new Uint32[width * height];
	buffer2 = new Uint32[width * height];
	currentBoard = buffer1;
	currentBuffer = buffer2;
	memcpy((void*)currentBuffer, (void*) texture->GetPixels(), texture->GetPitch() * height);
	memcpy((void*)currentBoard, (void*) texture->GetPixels(), texture->GetPitch() * height);
	texture->UnlockTexture();

	//Create a water board
	waterBoardBuffer1 = new Uint8[width * height];
	waterBoardBuffer2 = new Uint8[width * height];
	waterBoard = waterBoardBuffer1;
	waterBuffer = waterBoardBuffer2;
	for(int i = 0; i < width * height; ++i)
	{
		waterBuffer[i] = 0;
	}
	for(int i = width/2 - 10; i < width/2 + 10; ++i)
	{
		for(int j = height /2 - 10; j < height/2 + 10; ++j)
		{
			int index = j - (height/2 - 10);
			Uint8 color = index*8;
			SetWater(i, j, 1, waterBuffer);
		}
		SetWater(i, height/2 + 100, 2, waterBuffer);
	}

	//Separating the board into pieces by row to be passed evenly to threads
	int total = height;
	threadCount = std::thread::hardware_concurrency();
	threads = new std::thread[threadCount];
	readBarrier = new Barrier(threadCount + 1);
	writeBarrier = new Barrier(threadCount + 1);
	bufferBarrier = new Barrier(threadCount + 1);

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
	delete[] buffer1;
	delete[] buffer2;
	currentBoard = NULL;
	currentBuffer = NULL;
	delete[] waterBoardBuffer1;
	delete[] waterBoardBuffer2;
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
	while(!endProgram)
	{
		//Waits until the buffer is coppied and the texture is unlocked in the update thread
		//writeBarrier->Wait(threadCount + 1);
		CGOL(rowIndex, rowCount);
		//SimulateWater(rowIndex, rowCount);
		//Wait to write to buffer
		bufferBarrier->Wait(threadCount + 1);
		writeBarrier->Wait(threadCount + 1);
		MergeBuffer(rowIndex, rowCount);
		readBarrier->Wait(threadCount + 1);
		//readBarrier->Wait(threadCount + 1);
	}
}

Texture* Board::GetTexture()
{
	return texture;
}
void Board::Update(bool quit)
{
	bufferBarrier->Wait(threadCount + 1);
	Uint8* temp1 = waterBoard;
	waterBoard = waterBuffer;
	waterBuffer = temp1;

	Uint32* temp = currentBoard;
	currentBoard = currentBuffer;
	currentBuffer = temp;

	texture->LockTexture();
	writeBarrier->Wait(threadCount + 1);
	endProgram = quit;
	readBarrier->Wait(threadCount + 1);
	texture->UnlockTexture();
	
}

void Board::MergeBuffer(int rowIndex, int rowCount)
{
	for(Uint32 x = 0; x < width; ++x)
	{
		for(Uint32 y = rowIndex; y < rowIndex + rowCount; ++y)
		{
			/*
			if (waterBoard[x + (y * width)] > 0)
			{
				texture->ColorPixel(x, y, SDL_MapRGBA(mappingFormat, 0, 0, 255 - waterBoard[x + (y * width)], 0));
			}
			else
			{
				texture->ColorPixel(x, y, white);
			}
			*/
			texture->ColorPixel(x, y, currentBoard[x + (y * width)]);
		}
	}
}
void Board::MergeBuffer()
{
	for(Uint32 i = 0; i < width * height; ++i)
	{
		texture->ColorPixel(i, currentBoard[i]);
		/*
		if (waterBoard[i] > 0)
		{
			texture->ColorPixel(i, SDL_MapRGBA(mappingFormat, 0, 0, 255 - waterBoard[i], 0));
		}
		else
		{
			texture->ColorPixel(i, white);
		}	
		*/
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
			/*
			if (texture->UncheckedGetPixelColor(x - 1, y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x , y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y - 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x - 1, y ) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y) == black) count++;
			if (texture->UncheckedGetPixelColor(x - 1, y + 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x , y + 1) == black) count++;
			if (texture->UncheckedGetPixelColor(x + 1, y + 1) == black) count++;
			*/
			if (GetBoardColor(x - 1, y - 1, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x , y - 1, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x + 1, y - 1, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x - 1, y , width, height, currentBoard) == black) count++;
			if (GetBoardColor(x + 1, y, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x - 1, y + 1, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x , y + 1, width, height, currentBoard) == black) count++;
			if (GetBoardColor(x + 1, y + 1, width, height, currentBoard) == black) count++;

			if (count == 3 || (count == 2 && GetBoardColor(x, y, width, height, currentBoard) == black))//texture->GetPixelColor(x, y) == black))
			{
				color = black;
			}
			else
			{
				color = white;
			}
		
			currentBuffer[x + (y * width)] = color;

		}
	}
}	
Uint32 Board::GetBoardColor(Uint32 x, Uint32 y, Uint32 width, Uint32 height, Uint32* board)
{
	if (x >= width || y >= height || board == NULL)
	{
		return 0;
	}
	return board[x + (y * width)];
}
void Board::SimulateWater(int rowIndex, int rowCount)
{
	for(Uint32 x = 0; x < width; ++x)
	{
		for (Uint32 y = rowIndex; y < rowIndex + rowCount; ++y)
		{
			Uint8 waterAmount = GetWater(x, y, waterBoard);
			//If water
			if (waterAmount == 1)
			{
				if (GetWater(x, y + 1, waterBoard) == 0)
				{
					waterAmount = 0;
				}
			}
			else if (waterAmount == 0)
			{
				if (GetWater(x, y - 1, waterBoard) == 1)
				{
					waterAmount = 1;
				}
			}
			SetWater(x, y, waterAmount, waterBuffer);
		}
	}
}
Uint8 Board::GetWater(Uint32 x, Uint32 y, Uint8* board)
{
	if (x > width || y > height)
	{
		return 0;
	}
	return board[x + (y * width)];
}
void Board::SetWater(Uint32 x, Uint32 y, Uint8 amount, Uint8* board)
{
	board[x + (y * width)] = amount;
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
