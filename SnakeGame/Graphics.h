#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init(std::string title_, Uint32 window_flags_, Uint32 renderer_flags_, int img_flags_, int x_, int y_, int width_, int height_);

	SDL_Window* get_window();
	SDL_Renderer* get_renderer();
	int get_screen_width();
	int get_screen_height();
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int value;

	SDL_Window* _window;
	SDL_Renderer* _renderer;
};

