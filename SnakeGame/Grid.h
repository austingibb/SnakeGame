#ifndef GRID_H
#define GRID_H
#include "OnTile.h"
#include <SDL.h>

class Tile
{
public:
	int grid_x, grid_y;
	OnTile on_tile;
	SDL_Rect pixel_rect;

	Tile() {}

	void Init(int grid_x_, int grid_y_, int pixel_x_, int pixel_y_, int pixel_width_, int pixel_height_, OnTile on_tile_)
	{
		grid_x = grid_x_;
		grid_y = grid_y_;
		on_tile = on_tile_;
		
		pixel_rect.x = pixel_x_;
		pixel_rect.y = pixel_y_;
		pixel_rect.w = pixel_width_;
		pixel_rect.h = pixel_height_;
	}
};

class Grid
{
public:
	Grid(int pixel_width_, int pixel_height_, int grid_size_);
	~Grid();

	Tile* get_tile(int row_, int col_);
	void set_on_tile(int row_, int col_, OnTile on_tile_);
	void set_on_tile(Tile* tile_, OnTile on_tile_);

	int get_scale_factor();
	int get_available_tiles();
	int get_grid_size();
	int get_margin_offset();
	int get_pixels_per_tile();
	void print_grid();

private:
	Tile** _tiles;
	int _available_tiles;

	int _grid_size;
	int _grid_pixels;
	int _margin_offset;
	int _pixels_per_tile;
	int _scale_factor;
};

#endif

