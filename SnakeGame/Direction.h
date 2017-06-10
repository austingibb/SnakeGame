#ifndef DIRECTION_H
#define DIRECTION_H

enum Direction
{
	INVALID,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

Direction get_opposite_direction(Direction direction_);
#endif