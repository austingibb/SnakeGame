#include "SpriteAnimationFrameRotations.h"



SpriteAnimationFrameRotations::SpriteAnimationFrameRotations()
{
}

void SpriteAnimationFrameRotations::LoadRotations(SDL_Renderer* renderer_, std::string texture_path_, int cols_, int rows_, int width_, int height_, int num_sprites_)
{
	_rotations[0].LoadTexture(renderer_, texture_path_);
	_rotations[1].LoadTexture(renderer_, texture_path_, Sprite::ONE);
	_rotations[2].LoadTexture(renderer_, texture_path_, Sprite::TWO);
	_rotations[3].LoadTexture(renderer_, texture_path_, Sprite::THREE);

	_rotations[0].CreateSpriteDefs(cols_, rows_, width_, height_, num_sprites_);
	_rotations[1].CreateSpriteDefs(rows_, cols_, height_, width_, num_sprites_, true, false, false);
	_rotations[2].CreateSpriteDefs(cols_, rows_, width_, height_, num_sprites_, false, false, true);
	_rotations[3].CreateSpriteDefs(rows_, cols_, height_, width_, num_sprites_, false, true, false);
}

void SpriteAnimationFrameRotations::LoadRotations(SDL_Renderer * renderer_, std::string texture_path_, int cols_, int rows_, int width_, int height_, int num_sprites_, Uint8 red_, Uint8 green_, Uint8 blue_)
{
	_rotations[0].LoadTexture(renderer_, texture_path_);
	_rotations[0].SetColor(red_, green_, blue_);
	_rotations[1].LoadTexture(renderer_, texture_path_, Sprite::ONE);
	_rotations[1].SetColor(red_, green_, blue_);
	_rotations[2].LoadTexture(renderer_, texture_path_, Sprite::TWO);
	_rotations[2].SetColor(red_, green_, blue_);
	_rotations[3].LoadTexture(renderer_, texture_path_, Sprite::THREE);
	_rotations[3].SetColor(red_, green_, blue_);

	_rotations[0].CreateSpriteDefs(cols_, rows_, width_, height_, num_sprites_);
	_rotations[1].CreateSpriteDefs(rows_, cols_, height_, width_, num_sprites_, true, false, false);
	_rotations[2].CreateSpriteDefs(cols_, rows_, width_, height_, num_sprites_, false, false, true);
	_rotations[3].CreateSpriteDefs(rows_, cols_, height_, width_, num_sprites_, false, true, false);
}



SpriteAnimationFrames* SpriteAnimationFrameRotations::get_rotation_frames(Sprite::Rotation rotation_)
{
	return &_rotations[rotation_];
}
