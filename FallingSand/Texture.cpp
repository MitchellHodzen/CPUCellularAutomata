#include "Texture.h"
#include "Renderer.h"


Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
	pixels = NULL;
	pitch = 0;
}

Texture::~Texture()
{
	FreeTexture();
}

void Texture::CreateBlankTexture(int width, int height, Renderer* renderer)
{
	FreeTexture();
	texture = SDL_CreateTexture(renderer->GetRenderer(), SDL_GetWindowPixelFormat(renderer->GetWindow()), SDL_TEXTUREACCESS_STREAMING, width, height);
	if (texture == NULL)
	{
		std::cout<<"Unable to create blank texture. SDL Erorr: "<<SDL_GetError()<<std::endl;
	}
	else
	{
		this->width = width;
		this->height = height;
	}
}




void Texture::LoadTexture(std::string path, Renderer* renderer)
{
	FreeTexture();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image at path: " << path << " SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(renderer->GetWindow()), 0);
		if (formattedSurface == NULL)
		{
			std::cout<<"Unable to convert loaded surface to display format. SDL Error: " <<SDL_GetError()<<std::endl;
		}
		else
		{
			//texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
			texture = SDL_CreateTexture(renderer->GetRenderer(), SDL_GetWindowPixelFormat(renderer->GetWindow()), SDL_TEXTUREACCESS_STREAMING, loadedSurface->w, loadedSurface->h);
			if (texture == NULL)
			{
				//std::cout << "Unable to create texture. SDL error: " << SDL_GetError() << std::endl;
				std::cout<<"Unable to create blank texture. SDL Erorr: "<<SDL_GetError()<<std::endl;
			}
			else
			{
				std::cout<<"Unable to load textures at this time"<<std::endl;
				width = loadedSurface->w;
				height = loadedSurface->h;
			}
			SDL_FreeSurface(loadedSurface);
		}
	}
}

void Texture::FreeTexture()
{
	//std::cout<<"Freeing texture..."<<std::endl;
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
	//std::cout<<"texture freed"<<std::endl;
}
void Texture::Render(SDL_Renderer* renderer)
{
	if (renderer == NULL || renderer == nullptr)
	{
		std::cout << "renderer is null" << std::endl;
	}
	else
	{
		SDL_RenderCopy(renderer, texture, NULL, NULL);
	}
	//std::cout << "(" << cutRect->x << "," << cutRect->y << "), width: " << cutRect->w << " height: " << cutRect->h << std::endl;
}

bool Texture::LockTexture()
{
	bool success = true;
	if (pixels != NULL)
	{
		std::cout<<"Texture already locked"<<std::endl;
		success = false;
	}
	else
	{
		void* temp;
		if (SDL_LockTexture(texture, NULL, &temp, &pitch) != 0)
		{
			std::cout<<"Unable to lock texture. SDL Error: " <<SDL_GetError()<<std::endl;
			success = false;
		}
		else
		{
			pixels = (Uint32 *)temp;
		}
	}
	return success;
}	

bool Texture::UnlockTexture()
{
	bool success = true;

	if (pixels == NULL)
	{
		std::cout<<"Texture is not locked"<<std::endl;
		success = false;
	}
	else
	{
		SDL_UnlockTexture(texture);
		pixels = NULL;
		pitch = 0;
	}
	return success;
}

Uint32* Texture::GetPixels()
{
	return pixels;
}
int Texture::GetPitch()
{
	return pitch;
}
int Texture::GetWidth()
{
	return width;
}
int Texture::GetHeight()
{
	return height;
}

void Texture::ColorPixel(Uint32 x, Uint32 y, Uint32 color)
{
	if (pixels != NULL && x < width && y < height)
	{
		pixels[x + (y * width)] = color;
	}
}
void Texture::ColorPixel(Uint32 index, Uint32 color)
{
	if (pixels != NULL && index < width * height)
	{
		pixels[index] = color;
	}
}
Uint32 Texture::GetPixelColor(Uint32 x, Uint32 y)
{
	if (pixels == NULL || x >= width || y >= height)
	{
		return 0;
	}
	return pixels[x + (y * width)];
}
Uint32 Texture::UncheckedGetPixelColor(Uint32 x, Uint32 y)
{
	//assumes the board size is not Uint32 = -1 pixels long or wide
	if (x >= width || y >= height)
	{

		return 0;
	}
	return pixels[x + (y * width)];
}
