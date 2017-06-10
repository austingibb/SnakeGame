#include "Animation.h"

Animation::Animation(SpriteAnimationFrames* frames_, bool loop_) 
{
	_frames = frames_;
	_loop = loop_;
}

void Animation::Render(SDL_Renderer* renderer_)
{
	_frames->Render(renderer_, &_dst, _current_frame, _rotation, &_center, _flip);
}

void Animation::advance_frame()
{
	if (_current_frame + 1 >= _frames->get_num_frames())
	{
		if (_loop)
		{
			restart();
		}
	}
	else
	{
		_current_frame++;
	}
}

void Animation::restart()
{
	_current_frame = 0;
}

void Animation::set_frames(SpriteAnimationFrames * frames_)
{
	_frames = frames_;
}

void Animation::set_pos(int x_, int y_)
{
	_dst.x = x_;
	_dst.y = y_;
}

void Animation::set_bounds(int width_, int height_)
{
	_dst.w = width_;
	_dst.h = height_;
}

void Animation::set_dst(SDL_Rect &rect_)
{
	_dst = rect_;
}

void Animation::set_center(int x_, int y_)
{
	_center.x = x_;
	_center.y = y_;
}

void Animation::set_rotation(double rotation_)
{
	_rotation = rotation_;
}

void Animation::set_flip(SDL_RendererFlip flip_)
{
	_flip = flip_;
}
