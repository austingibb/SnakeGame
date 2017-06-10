#pragma once
#include "Sprite.h"
class SpriteAnimationFrames : public Sprite
{
public:
	void Render(SDL_Renderer* renderer_, SDL_Rect* dst_ = NULL, int sprite_index_ = 0, double angle_ = 0.0, SDL_Point* center_ = NULL, SDL_RendererFlip flip_ = SDL_FLIP_NONE);

	void AddSpriteDef(SDL_Rect def_);
	void ClearSpriteDefs();
	void CreateSpriteDefs(int cols_, int rows_, int width_, int height_, int num_sprites_, bool up_first_ = true, bool left_first_ = true, bool cols_first_ = true);
	void CreateSpriteDefs(int cols_, int rows_, int width_, int height_, bool up_first_ = true, bool left_first_ = true, bool cols_first_ = true);

	SDL_Rect get_frame(int frame_);
	int get_num_frames();

private:
	std::vector<SDL_Rect> _sprite_defs;
};

