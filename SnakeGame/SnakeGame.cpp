#include "SnakeGame.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <math.h>

const std::string SnakeGame::VERSION = "1.0";

SnakeGame::SnakeGame(GameSize size_) : _rng(std::random_device()()), _frames(0)
{
	SDL_Init(0);

	_graphics.Init("Snake Game Version " + SnakeGame::VERSION,
		SDL_WINDOW_SHOWN
		| SDL_WINDOW_FULLSCREEN_DESKTOP
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, IMG_INIT_PNG,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 360);

	// possibly make a class for this instead of just doing audio here.
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("Mixer failed to initialize. %s\n", Mix_GetError());
	}

	// make "background" refresh color black
	SDL_SetRenderDrawColor(_graphics.get_renderer(), 0x00, 0x00, 0x00, 0xFF);

	LoadMedia();

	_grid = new Grid(_graphics.get_screen_width(), _graphics.get_screen_height(), size_);

	for (int i = 0; i < 4; i++)
	{
		AddFood();
	}

	_grid->set_on_tile(1, 1, OnTile::WALL);
	_grid->set_on_tile(2, 2, OnTile::WALL);
	_grid->set_on_tile(3, 3, OnTile::WALL);
	_grid->set_on_tile(4, 4, OnTile::WALL);
	_grid->set_on_tile(5, 5, OnTile::WALL);
	_grid->set_on_tile(6, 6, OnTile::WALL);
	_grid->set_on_tile(7, 7, OnTile::WALL);
	_grid->set_on_tile(8, 8, OnTile::WALL);
	_grid->set_on_tile(9, 9, OnTile::WALL);
	_grid->set_on_tile(10, 10, OnTile::WALL);
	_grid->set_on_tile(11, 11, OnTile::WALL);
	_grid->set_on_tile(12, 12, OnTile::WALL);
	_grid->set_on_tile(13, 13, OnTile::WALL);
	_grid->set_on_tile(14, 14, OnTile::WALL);
	_grid->set_on_tile(15, 15, OnTile::WALL);
	_grid->set_on_tile(16, 16, OnTile::WALL);

	_snake = new Snake(&_graphics, 100, 100, 200, _grid, 4, 9, 4, Direction::DOWN);

	_fps_timer.start();
}

SnakeGame::~SnakeGame()
{
	delete _snake;
	delete _grid;

	Mix_FreeMusic(_intro_music);
	Mix_FreeMusic(_main_music);

	Mix_FreeChunk(_hit_sound);
	Mix_FreeChunk(_death_sound);
	Mix_FreeChunk(_grow_sound);
	Mix_FreeChunk(_movement_sound);
	Mix_FreeChunk(_turn_sound);

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void SnakeGame::RunGame()
{
	bool quit = false;
	SDL_Event e;
	Uint32 start = SDL_GetTicks();

	while (!quit)
	{
		while (SDL_PollEvent(&e) > 0)
		{	
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE || e.type == SDL_QUIT)
				{
					quit = true;
				}
				else
				{
					switch (_game_state)
					{
					case RUNNING:
						if (e.key.repeat == 0)
						{
							_snake->HandleInput(e.key.keysym.sym);
						}
						else
						{
							_snake->HandleRepeatInput(e.key.keysym.sym);
						}
						break;
					case OVER:
						if (e.key.keysym.sym == SDLK_r)
						{
							delete _snake;
							_snake = new Snake(&_graphics, 100, 100, 200, _grid, 4, 9, 4, Direction::DOWN);
							_game_state = RUNNING;
						}
						break;
					}
				}
			}

		}

		switch (_game_state)
		{
		case RUNNING:
			UpdateAll(); 
			RenderAll();
			break;
		case OVER:
			break;
		}
		
		/*
		if (_frames > 0 && _frames % 240 == 0)
		{
			double fps = ((double)_frames) / (((double)_fps_timer.get_runtime()) / 1000);
			printf("fps: %.2f\n", fps);
		}

		_frames++;
		
		system("CLS");
		_grid->print_grid();
		*/
	}
}

void SnakeGame::UpdateAll()
{
	Uint8 _result;
	_result = _snake->Update();

	if (_result & Snake::ATE)
	{
		Mix_PlayChannel(-1, _grow_sound, 0);
		AddFood();
	}

	if (_result & Snake::HIT_SELF_INITIAL || _result & Snake::HIT_WALL_INITIAL)
	{
		Mix_PlayChannel(-1, _hit_sound, 0);
	}

	if (_result & Snake::DIED)
	{
		Mix_PlayChannel(-1, _death_sound, 0);
		_game_state = OVER;
	}

	if (_result & Snake::TURNED)
	{
		Mix_PlayChannel(-1, _turn_sound, 0);
	}
	else if (_result & Snake::MOVED)
	{
		Mix_PlayChannel(-1, _movement_sound, 0);
	}
}

void SnakeGame::RenderAll()
{
	SDL_RenderClear(_graphics.get_renderer());

	for (int row = 0; row < _grid->get_grid_size(); row++)
	{
		for (int col = 0; col < _grid->get_grid_size(); col++)
		{
			Tile* tile = _grid->get_tile(row, col);
			_ground.Render(_graphics.get_renderer(), &tile->pixel_rect);

			switch (tile->on_tile)
			{
			case FOOD:
				_food.Render(_graphics.get_renderer(), &tile->pixel_rect);
				break;
			case WALL:
				_wall.Render(_graphics.get_renderer(), &tile->pixel_rect);
				break;
			}
		}
	}

	_snake->Render();

	SDL_RenderPresent(_graphics.get_renderer());
}

void SnakeGame::AddFood()
{
	int available_tiles = _grid->get_available_tiles();
	if (available_tiles <= 0)
	{
		return;
	}

	std::uniform_int_distribution<int> dist(1, available_tiles);
	int tile_index = dist(_rng) - 1;

	int available_count = 0;
	for (int row = 0; row < _grid->get_grid_size(); row++)
	{
		for (int col = 0; col < _grid->get_grid_size(); col++)
		{
			Tile* tile = _grid->get_tile(row, col);
			if (tile->on_tile == NOTHING)
			{
				if (available_count >= tile_index)
				{
					printf("Food added at: (%d, %d)\n", col, row);
					_grid->set_on_tile(row, col, FOOD);
					goto food_placed;
				}
				else
				{
					available_count++;
				}
			}
		}
	}
food_placed:;
}

void SnakeGame::LoadMedia()
{
	_wall.LoadTexture(_graphics.get_renderer(), "Art/Grid/PlaceholderWall.png");
	_ground.LoadTexture(_graphics.get_renderer(), "Art/Grid/PlaceholderGround.png");
	_food.LoadTexture(_graphics.get_renderer(), "Art/Grid/TempFood.png");

	/*_intro_music = Mix_LoadMUS("Sound/Snake_Intro.wav");
	if (_intro_music == NULL)
	{
		printf("Failed to load intro music. %s\n", Mix_GetError());
	}
	_main_music = Mix_LoadMUS("Sound/Snake_Main.wav");
	if (_main_music == NULL)
	{
		printf("Failed to load main music. %s\n", Mix_GetError());
	}*/

	_hit_sound = Mix_LoadWAV("Sound/Snake_Pre-Death.wav");
	if (_hit_sound == NULL)
	{
		printf("Failed to load hit sound. %s\n", Mix_GetError());
	}
	_death_sound = Mix_LoadWAV("Sound/Snake_Death.wav");
	if (_death_sound == NULL)
	{
		printf("Failed to load death sound. %s\n", Mix_GetError());
	}
	_grow_sound = Mix_LoadWAV("Sound/Snake_Grow.wav");
	if (_grow_sound == NULL)
	{
		printf("Failed to load grow sound. %s\n", Mix_GetError());
	}
	_movement_sound = Mix_LoadWAV("Sound/Snake_Movement.wav");
	if (_movement_sound == NULL)
	{
		printf("Failed to load move sound. %s\n", Mix_GetError());
	}
	_turn_sound = Mix_LoadWAV("Sound/Snake_Turn.wav");
	if (_turn_sound == NULL)
	{
		printf("Failed to load turn sound. %s\n", Mix_GetError());
	}
}

int main(int argc, char* args[])
{
	SnakeGame game(SnakeGame::GameSize::EIGHTEEN);
	game.RunGame();

	/*
	Graphics graphics;
	graphics.Init("Snake Game Version " + SnakeGame::VERSION,
		SDL_WINDOW_SHOWN
		//| SDL_WINDOW_FULLSCREEN_DESKTOP
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, IMG_INIT_PNG,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200);

	SpriteAnimationFrameRotations rotations;
	rotations.LoadRotations(graphics.get_renderer(), "Art/Snake/PlaceholderHeadStraight.png", 4, 3, 10, 20, 12);

	SpriteAnimationFrames frames = rotations.get_rotation_frames(Sprite::THREE);

	Animation anim(&frames, true);
	//anim.set_flip(SDL_RendererFlip::SDL_FLIP_VERTICAL);
	SDL_Rect dst = { 0, 0, 100, 200 };
	anim.set_dst(dst);

	SDL_SetRenderDrawColor(graphics.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	int count = 0;
	while (true)
	{
		count++;
		if (count % 5)
		{
			anim.advance_frame();
		}

		SDL_RenderClear(graphics.get_renderer());

		anim.Render(graphics.get_renderer());

		SDL_RenderPresent(graphics.get_renderer());

		SDL_Delay(100);
	}
	*/

	return 0;
}