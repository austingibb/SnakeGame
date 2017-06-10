#include "SpriteAnimationFrames.h"

void SpriteAnimationFrames::Render(SDL_Renderer* renderer_, SDL_Rect* dst_, int sprite_index_, double angle_, SDL_Point* center_, SDL_RendererFlip flip_)
{
	SDL_Rect* src = NULL;
	if (_sprite_defs.size() > 0)
	{
		src = &_sprite_defs[sprite_index_];

		if (center_ == NULL)
		{
			SDL_Point point = { _sprite_defs[sprite_index_].w / 2, _sprite_defs[sprite_index_].h / 2 };
			center_ = &point;
		}
	}

	SDL_RenderCopyEx(renderer_, _texture, src, dst_, angle_, center_, flip_);
}

void SpriteAnimationFrames::AddSpriteDef(SDL_Rect def_)
{
	_sprite_defs.push_back(def_);
}

void SpriteAnimationFrames::ClearSpriteDefs()
{
	_sprite_defs.clear();
}

void SpriteAnimationFrames::CreateSpriteDefs(int cols_, int rows_, int width_, int height_, int num_sprites_, 
	bool up_first_, bool left_first_, bool cols_first_)
{
	_sprite_defs.clear();

	int col_mod = 0, col_start = 0, row_mod = 0, row_start = 0;;
	if (left_first_)
	{
		col_mod = 1;
	}
	else
	{
		col_mod = -1;
		col_start = cols_ - 1;
	}

	if (up_first_)
	{
		row_mod = 1;
	}
	else
	{
		row_mod = -1;
		row_start = rows_ - 1;
	}

	int sprite_counter = 0;
	if (cols_first_)
	{
		for (int row = row_start; row < rows_ && row >= 0; row += row_mod)
		{
			for (int col = col_start; col < cols_ && col >= 0; col += col_mod)
			{
				SDL_Rect rect = { col * width_, row * height_, width_, height_ };
				_sprite_defs.push_back(rect);

				sprite_counter++;
				if (sprite_counter >= num_sprites_) goto sprites_finished;
			}
		}
	}
	else
	{
		for (int col = col_start; col < cols_ && col >= 0; col += col_mod)
		{
			for (int row = row_start; row < rows_ && row >= 0; row += row_mod)
			{
				SDL_Rect rect = { col * width_, row * height_, width_, height_ };
				_sprite_defs.push_back(rect);

				sprite_counter++;
				if (sprite_counter >= num_sprites_) goto sprites_finished;
			}
		}
	}

sprites_finished:;
}

void SpriteAnimationFrames::CreateSpriteDefs(int cols_, int rows_, int width_, int height_, bool up_first_, bool left_first_, bool cols_first_)
{
	CreateSpriteDefs(cols_, rows_, width_, height_, cols_ * rows_, up_first_, left_first_, cols_first_);
}

SDL_Rect SpriteAnimationFrames::get_frame(int frame_)
{
	return _sprite_defs[frame_];
}

int SpriteAnimationFrames::get_num_frames()
{
	return _sprite_defs.size();
}
