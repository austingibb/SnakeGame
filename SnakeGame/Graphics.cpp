#include "Graphics.h"
#include <cstdio>

Graphics::Graphics() : _window(NULL), _renderer(NULL) {}

Graphics::~Graphics()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

bool Graphics::Init(std::string title_, Uint32 window_flags_, Uint32 renderer_flags_, int img_flags_, int x_, int y_, int width_, int height_)
{
	SCREEN_WIDTH = width_;
	SCREEN_HEIGHT = height_;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL video didn't initialize. %s", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(img_flags_) & img_flags_))
	{
		printf("SDL image didn't initialize. %s", SDL_GetError());
		return false;
	}

	if (_window == NULL)
	{
		_window = SDL_CreateWindow(title_.c_str(), x_, y_, width_, height_, window_flags_);
		if (_window == NULL)
		{
			printf("Could not create window. %s", SDL_GetError());
			return false;
		}
	}

	if (_renderer == NULL)
	{
		_renderer = SDL_CreateRenderer(_window, -1, renderer_flags_);
		if (_renderer == NULL)
		{
			printf("Could not create renderer. %s", SDL_GetError());
			return false;
		}
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(_renderer, width_, height_);

	return true;
}

SDL_Window* Graphics::get_window()
{
	return _window;
}

SDL_Renderer* Graphics::get_renderer()
{
	return _renderer;
}

int Graphics::get_screen_width()
{
	return SCREEN_WIDTH;
}

int Graphics::get_screen_height()
{
	return SCREEN_HEIGHT;
}

