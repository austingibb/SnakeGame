#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "SDL2_rotozoom.h"
class Sprite
{
public:
	Sprite();
	~Sprite();

	enum Rotation
	{
		NONE,
		ONE,
		TWO,
		THREE
	};

	bool LoadTexture(SDL_Renderer* renderer_, std::string texture_path_, Rotation rotation_ = NONE);

	void Render(SDL_Renderer* renderer_, SDL_Rect* dst_ = NULL, double angle_ = 0.0, SDL_Point* center_ = NULL, SDL_RendererFlip flip_ = SDL_FLIP_NONE);

	void SetColor(Uint8 red_, Uint8 green_, Uint8 blue_);

	int get_texture_width();
	int get_texture_height();
protected:
	SDL_Texture* _texture;

	int _texture_width;
	int _texture_height;
};

