#include "Grid.h"
#include <cstdio>

Grid::Grid(int pixel_width_, int pixel_height_, int grid_size_)
{
	bool width_larger = true;
	int larger = 0;
	if (pixel_width_ > pixel_height_)
	{
		larger = pixel_width_;
		_grid_pixels = pixel_height_;
	}
	else
	{
		width_larger = false;
		larger = pixel_height_;
		_grid_pixels = pixel_width_;
	}

	_grid_size = grid_size_;

	_margin_offset = (larger - _grid_pixels) / 2;
	_pixels_per_tile = _grid_pixels / _grid_size;
	_scale_factor = _pixels_per_tile / 10;

	_tiles = new Tile*[_grid_size];
	for (int row = 0; row < _grid_size; ++row)
	{
		_tiles[row] = new Tile[_grid_size];
	}

	for (int row = 0; row < _grid_size; ++row)
	{
		for (int col = 0; col < _grid_size; ++col)
		{
			int pixel_x = col * _pixels_per_tile;
			int pixel_y = row * _pixels_per_tile;

			if (width_larger)
			{
				pixel_x += _margin_offset;
			}
			else
			{
				pixel_y += _margin_offset;
			}

			_tiles[row][col].Init(col, row, pixel_x, pixel_y, _pixels_per_tile, _pixels_per_tile, OnTile::NOTHING);
		}
	}

	_available_tiles = _grid_size * _grid_size;
}

Grid::~Grid()
{
	for (int row = 0; row < _grid_size; ++row) {
		delete[] _tiles[row];
	}
	delete[] _tiles;
}

Tile * Grid::get_tile(int row_, int col_)
{
	return &_tiles[row_][col_];
}

void Grid::set_on_tile(int row_, int col_, OnTile on_tile_)
{
	Tile* tile = &_tiles[row_][col_];
	if (tile->on_tile == NOTHING && on_tile_ != NOTHING)
	{
		_available_tiles--;
	}
	else if (tile->on_tile != NOTHING && on_tile_ == NOTHING)
	{
		_available_tiles++;
	}

	tile->on_tile = on_tile_;
}

void Grid::set_on_tile(Tile * tile_, OnTile on_tile_)
{
	set_on_tile(tile_->grid_y, tile_->grid_x, on_tile_);
}

int Grid::get_scale_factor()
{
	return _scale_factor;
}

int Grid::get_available_tiles()
{
	return _available_tiles;
}

int Grid::get_grid_size()
{
	return _grid_size;
}

int Grid::get_margin_offset()
{
	return _margin_offset;
}

int Grid::get_pixels_per_tile()
{
	return _pixels_per_tile;
}

void Grid::print_grid()
{
	for (int row = 0; row < _grid_size; ++row)
	{
		for (int col = 0; col < _grid_size; ++col)
		{
			printf("%d", get_tile(row, col)->on_tile);
		}
		printf("\n");
	}
}
