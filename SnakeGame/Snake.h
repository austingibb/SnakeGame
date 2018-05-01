#pragma once

#include "LimitQueue.h"
#include "Animation.h"
#include "SpriteAnimationFrameRotations.h"
#include "Direction.h"
#include "Grid.h"
#include "Segment.h"
#include "Timer.h"
#include "Graphics.h"
class Snake
{
public:
	enum SnakeState
	{
		AGAINST_WALL,
		AGAINST_SEGMENT,
		MOVING,
		DEAD
	};

	/* UpdateStatus */
	const static Uint8
		NONE = 0,
		ATE = 1,
		HIT_WALL_INITIAL = 2,
		HIT_WALL = 4,
		HIT_SELF_INITIAL = 8,
		HIT_SELF = 16,
		MOVED = 32,
		TURNED = 64,
		DIED = 128;


	Snake(Graphics* graphics_, Uint8 red_, Uint8 green_, Uint8 blue_, Grid* grid_, int length_, int head_x_, int head_y_, Direction direction_);
	~Snake();
	void LoadAnimationFrames(SDL_Renderer* renderer_);
	void HandleInput(SDL_Keycode key_);
	void HandleRepeatInput(SDL_Keycode key_);
	Uint16 Update();
	void Render();

	SnakeState get_state();
private:
	Grid* _grid;

	int _frames_per_action = 5;
	int _ticks_per_frame = 1;
	int _total_ticks;
	int _current_tick;

	int _ticks_before_fail = 20;
	int _fail_tick = 0;

	Uint16 _update_status;
	SnakeState _state;
	int _head_x, _head_y;
	int _tail_x, _tail_y;
	int _length;
	Segment* _head;
	Segment* _tail;
	bool _extending;

	LimitQueue _input_queue;
	Direction _queued_direction;
	Direction _last_direction;

	static const bool _debug_input = false;

	Graphics* _graphics;
	Uint8 _red = 255, _green = 255, _blue = 255;

	SpriteAnimationFrameRotations _head_straight_rotations;
	SegmentAnimationTemplate _head_straight_template[4];
	SpriteAnimationFrameRotations _head_turn_rotations;
	SegmentAnimationTemplate _head_turn_template[8];
	SpriteAnimationFrameRotations _body_straight_rotations;
	SegmentAnimationTemplate _body_straight_template[4];
	SpriteAnimationFrameRotations _body_turn_rotations;
	SegmentAnimationTemplate _body_turn_template[8];
	SpriteAnimationFrameRotations _tail_straight_rotations;
	SegmentAnimationTemplate _tail_straight_template[4];
	SpriteAnimationFrameRotations _tail_turn_rotations;
	SegmentAnimationTemplate _tail_turn_template[8];
	SpriteAnimationFrameRotations _tail_wait_rotations;
	SegmentAnimationTemplate _tail_wait_template[4];

	static const bool _debug_animations = false;
	int _debug_animation_tick = 0;
	SDL_Point _debug_animation_tile = { 0, 1 };
	Animation* _debug_anim;
	int _queued_anim_update = 1;
	int _current_anim = 0;

	void UpdateTmpAnim();
	Segment* CreateSegment();
	void ActionComplete();
	void UpdateSegments();
	void UpdateSegmentAnimation(Segment* segment_);
	void CommitHeadDirection();
	void CheckHeadTile();
	OnTile CheckNextTile(Direction direction_);
	void CommitInput();
	void Extend();

	int update_based_on_direction(Direction direction_, int& x_, int& y_, bool force_bound_ = true);
	Direction direction_from_key(SDL_Keycode key_);
};

