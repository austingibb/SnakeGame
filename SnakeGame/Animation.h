#pragma once
#include "SpriteAnimationFrames.h"
class Animation
{
public:
	Animation(SpriteAnimationFrames* frames_, bool loop_);

	void Render(SDL_Renderer* renderer_);

	void advance_frame();
	void restart();
	void set_frames(SpriteAnimationFrames* frames_);
	void set_pos(int x_, int y_);
	void set_bounds(int width_, int height_);
	void set_dst(SDL_Rect &rect_);
	void set_center(int x_, int y_);
	void set_rotation(double rotation_);
	void set_flip(SDL_RendererFlip flip_);
private:
	SpriteAnimationFrames* _frames;
	int _current_frame;
	bool _loop;

	SDL_Rect _dst;
	SDL_Point _center;
	double _rotation;

	SDL_RendererFlip _flip;
};

