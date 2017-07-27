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
	//for(int i = 0; i < width * height; ++i)
	//{
		//texture->ColorPixel(i, white);
	//}
	MakeStatic(white, black);
	buffer = new Uint32[width * height];
	memcpy((void*)buffer, (void*) texture->GetPixels(), texture->GetPitch() * height);
	texture->UnlockTexture();
}
Board::~Board()
{
	delete[] buffer;
	delete texture;
	SDL_FreeFormat(mappingFormat);
}

Texture* Board::GetTexture()
{
	return texture;
}
void Board::Update()
{
	texture->LockTexture();
	//MakeStatic(SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00), SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00));
	TestBuffer();
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
void Board::TestBuffer()
{
	Uint32 black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0x00);
	Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0x00);
	Uint32 color;
	int count = 0;
	for(Uint32 x = 0; x < width; ++x)
	{
		for (Uint32 y = 0; y < height; ++y)
		{
			count = 0;
			/*
			for (int i = -1; i < 2; ++i)
			{
				for(int j = -1; j < 2; ++j)
				{
					if (!(i == 0 && j == 0) && texture->GetPixelColor(x + i, y + j) == black)
					{
						count++;
					}
				}
			}
			*/
			if (texture->GetPixelColor(x - 1, y - 1) == black) count++;
			if (texture->GetPixelColor(x , y - 1) == black) count++;
			if (texture->GetPixelColor(x + 1, y - 1) == black) count++;
			if (texture->GetPixelColor(x - 1, y ) == black) count++;
			if (texture->GetPixelColor(x + 1, y) == black) count++;
			if (texture->GetPixelColor(x - 1, y + 1) == black) count++;
			if (texture->GetPixelColor(x , y + 1) == black) count++;
			if (texture->GetPixelColor(x + 1, y + 1) == black) count++;


			if (count == 3 || (count == 2 && texture->GetPixelColor(x, y) == black))
			{
				color = black;
			}
			else
			{
				color = white;
			}
			/*
			if (texture->GetPixelColor(x, y) == white)
			{
				if(count == 3)
				{
					color = black;
				}
				else
				{
					color = white;
				}
			}
			else
			{
				if (count < 2 || count > 3)
				{
					color = white;
				}
				else
				{
					color = black;
				}
			}	
			*/
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
