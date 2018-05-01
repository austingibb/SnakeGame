#pragma once
#include <string>
#include <random>
#include <SDL_mixer.h>

#include "Graphics.h"
#include "Sprite.h"
#include "Grid.h"
#include "Snake.h"
#include "Timer.h"
#include "SpriteAnimationFrameRotations.h"

class SnakeGame
{
public:
	const static std::string VERSION;

	// these sizes allow perfect scaling for the pixel art
	enum GameSize
	{
		SIX = 6,
		NINE = 9,
		TWELVE = 12,
		EIGHTEEN = 18,
		THRITY_SIX = 36
	};

	enum GameState
	{
		RUNNING,
		OVER
	};

	SnakeGame(GameSize size_);
	~SnakeGame();
	
	void RunGame();

private:
	GameState _game_state;

	Grid* _grid  = NULL;
	Snake* _snake = NULL;

	Graphics _graphics;
	Timer _fps_timer;
	int _frames;

	std::mt19937 _rng;

	Sprite _food;
	Sprite _ground;
	Sprite _wall;
	Mix_Music* _intro_music = NULL;
	Mix_Music* _main_music = NULL;
	Mix_Chunk* _hit_sound = NULL;
	Mix_Chunk* _death_sound = NULL;
	Mix_Chunk* _grow_sound = NULL;
	Mix_Chunk* _movement_sound = NULL;
	Mix_Chunk* _turn_sound = NULL;

	void LoadMedia();
	void UpdateAll();
	void RenderAll();
	void AddFood();
};

