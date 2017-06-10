#include "Direction.h"

Direction get_opposite_direction(Direction direction_)
{
	switch (direction_)
	{
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	}
}
