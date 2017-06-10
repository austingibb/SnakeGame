#pragma once 
#include "SpriteAnimationFrames.h"

class SpriteAnimationFrameRotations
{
public:
	SpriteAnimationFrameRotations();
	
	void LoadRotations(SDL_Renderer* renderer_, std::string texture_path_, int cols_, int rows_, int width_, int height_, int num_sprites_);

	void LoadRotations(SDL_Renderer* renderer_, std::string texture_path_, int cols_, int rows_, int width_, int height_, int num_sprites_, 
		Uint8 red_, Uint8 green_, Uint8 blue_);

	SpriteAnimationFrames* get_rotation_frames(Sprite::Rotation rotation_);

private:
	SpriteAnimationFrames _rotations[4];
};

