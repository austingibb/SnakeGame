#include "Sprite.h"
#include <SDL_image.h>
#include <cstdio>

Sprite::Sprite() : _texture(NULL) {}

Sprite::~Sprite() 
{
	SDL_DestroyTexture(_texture);
}

bool Sprite::LoadTexture(SDL_Renderer* renderer_, std::string texture_path_, Rotation rotation_)
{
	SDL_Surface* surface = IMG_Load(texture_path_.c_str());
	if (surface == NULL)
	{
		printf("Texture at %s couldn't load. %s", texture_path_, IMG_GetError());
		return false;
	}
	
	if (rotation_ != NONE)
	{
		surface = rotateSurface90Degrees(surface, rotation_);
	}

	_texture_width = surface->w;
	_texture_height = surface->h;

	SDL_Texture* loaded_texture = SDL_CreateTextureFromSurface(renderer_, surface);
	if (loaded_texture == NULL)
	{
		printf("Texture at %s couldn't load. %s", texture_path_, SDL_GetError());
		return false;
	}

	SDL_FreeSurface(surface);

	_texture = loaded_texture;
	return true;
}

void Sprite::Render(SDL_Renderer* renderer_, SDL_Rect* dst_, double angle_, SDL_Point* center_, SDL_RendererFlip flip_)
{
	SDL_RenderCopyEx(renderer_, _texture, NULL, dst_, angle_, center_, flip_);
}

void Sprite::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(_texture, red, green, blue);
}

int Sprite::get_texture_width()
{
	return _texture_width;
}

int Sprite::get_texture_height()
{
	return _texture_height;
}