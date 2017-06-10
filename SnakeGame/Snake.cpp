#include "Snake.h"

Snake::Snake(Graphics* graphics_, Uint8 red_, Uint8 green_, Uint8 blue_, Grid* grid_, int length_, int head_x_, int head_y_, Direction direction_)
	: _red(red_), _green(green_), _blue(blue_), _input_queue(3), _queued_direction(direction_), _last_direction(direction_), _head_x(head_x_), _head_y(head_y_),
	_current_tick(0), _extending(false), _state(SnakeState::MOVING), _update_status(NONE)
{
	_grid = grid_;

	if (length_ < 3)
	{
		length_ = 3;
	}
	_length = length_;
	
	_graphics = graphics_;

	_input_queue.enqueue(direction_);

	LoadAnimationFrames(_graphics->get_renderer());

	_grid->set_on_tile(head_y_, head_x_, SEGMENT);
	_grid->set_on_tile(head_y_ - 1, head_x_, SEGMENT);
	_grid->set_on_tile(head_y_ - 2, head_x_, SEGMENT);
	_grid->set_on_tile(head_y_ - 3, head_x_, SEGMENT);

	_head = CreateSegment();
	_head->set_type(Segment::SegmentType::HEAD);
	Tile* head_tile = _grid->get_tile(head_y_,head_x_);
	_head->set_tile(head_tile);
	_head->set_incoming(Direction::DOWN);
	_head->set_outgoing(Direction::UP);
	UpdateSegmentAnimation(_head);

	Tile* body_tile = _grid->get_tile(head_y_ - 1, head_x_);
	Segment* body = CreateSegment();
	body->set_tile(body_tile);
	body->set_type(Segment::SegmentType::BODY);
	body->set_incoming(Direction::DOWN);
	body->set_outgoing(Direction::UP);
	UpdateSegmentAnimation(body);

	Tile* body_tile2 = _grid->get_tile(head_y_ - 2, head_x_);
	Segment* body2 = CreateSegment();
	body2->set_tile(body_tile2);
	body2->set_type(Segment::SegmentType::BODY);
	body2->set_incoming(Direction::DOWN);
	body2->set_outgoing(Direction::UP);
	UpdateSegmentAnimation(body2);

	Tile* tail_tile = _grid->get_tile(head_y_ - 3, head_x_);
	_tail = CreateSegment();
	_tail->set_tile(tail_tile);
	_tail->set_type(Segment::SegmentType::TAIL);
	_tail->set_incoming(Direction::DOWN);
	_tail->set_outgoing(Direction::UP);
	UpdateSegmentAnimation(_tail);

	_head->set_next(body);
	body->set_previous(_head);
	body->set_next(body2);
	body2->set_previous(body);
	body2->set_next(_tail);
	_tail->set_previous(body2);

	if (_debug_animations) {
		_debug_anim = new Animation(NULL, true);
		UpdateTmpAnim();
		_queued_anim_update = 0;
	}
}

Snake::~Snake()
{
	if (_debug_animations)
	{
		delete _debug_anim;
	}

	Segment* _next = _head;
	while (_next != NULL)
	{
		Segment* segment = _next;
		Animation* animation = _next->get_animation();
		_next = _next->get_next();
		delete segment;
		delete animation;
	}
}

void Snake::LoadAnimationFrames(SDL_Renderer* renderer_)
{
	/* head */
	_head_straight_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyHeadStraight.png", 4, 3, 10, 20, 12, _red, _green, _blue);
	_head_straight_template[0].frames = _head_straight_rotations.get_rotation_frames(Sprite::NONE);
	_head_straight_template[0].bound = { 0, -10, 10, 20 };
	_head_straight_template[1].frames = _head_straight_rotations.get_rotation_frames(Sprite::ONE);
	_head_straight_template[1].bound = { 0, 0, 20, 10 };
	_head_straight_template[2].frames = _head_straight_rotations.get_rotation_frames(Sprite::TWO);
	_head_straight_template[2].bound = { 0, 0, 10, 20 };
	_head_straight_template[3].frames = _head_straight_rotations.get_rotation_frames(Sprite::THREE);
	_head_straight_template[3].bound = { -10, 0, 20, 10 };

	_head_turn_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyHeadTurn.png", 3, 4, 20, 10, 12, _red, _green, _blue);
	_head_turn_template[0].frames = _head_turn_rotations.get_rotation_frames(Sprite::NONE);
	_head_turn_template[0].bound = { -10, 0, 20, 10 };
	_head_turn_template[0].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_head_turn_template[1].frames = _head_turn_rotations.get_rotation_frames(Sprite::NONE);
	_head_turn_template[1].bound = { 0, 0, 20, 10 };

	_head_turn_template[2].frames = _head_turn_rotations.get_rotation_frames(Sprite::ONE);
	_head_turn_template[2].bound = { 0, -10, 10, 20 };
	_head_turn_template[2].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_head_turn_template[3].frames = _head_turn_rotations.get_rotation_frames(Sprite::ONE);
	_head_turn_template[3].bound = { 0, 0, 10, 20 };

	_head_turn_template[4].frames = _head_turn_rotations.get_rotation_frames(Sprite::TWO);
	_head_turn_template[4].bound = { 0, 0, 20, 10 };
	_head_turn_template[4].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_head_turn_template[5].frames = _head_turn_rotations.get_rotation_frames(Sprite::TWO);
	_head_turn_template[5].bound = { -10, 0, 20, 10 };

	_head_turn_template[6].frames = _head_turn_rotations.get_rotation_frames(Sprite::THREE);
	_head_turn_template[6].bound = { 0, 0, 10, 20 };
	_head_turn_template[6].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_head_turn_template[7].frames = _head_turn_rotations.get_rotation_frames(Sprite::THREE);
	_head_turn_template[7].bound = { 0, -10, 10, 20 };
	/* head */

	/* body */
	_body_straight_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyBodyStraight.png", 2, 3, 10, 10, 6, _red, _green, _blue);
	_body_straight_template[0].frames = _body_straight_rotations.get_rotation_frames(Sprite::TWO);
	_body_straight_template[0].bound = { 0, 0, 10, 10 };
	_body_straight_template[1].frames = _body_straight_rotations.get_rotation_frames(Sprite::THREE);
	_body_straight_template[1].bound = { 0, 0, 10, 10 };
	_body_straight_template[2].frames = _body_straight_rotations.get_rotation_frames(Sprite::NONE);
	_body_straight_template[2].bound = { 0, 0, 10, 10 };
	_body_straight_template[3].frames = _body_straight_rotations.get_rotation_frames(Sprite::ONE);
	_body_straight_template[3].bound = { 0, 0, 10, 10 };

	_body_turn_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyBodyTurn.png", 2, 3, 10, 10, 6, _red, _green, _blue);
	_body_turn_template[0].frames = _body_turn_rotations.get_rotation_frames(Sprite::NONE);
	_body_turn_template[0].bound = { 0, 0, 10, 10 };
	_body_turn_template[0].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_body_turn_template[1].frames = _body_turn_rotations.get_rotation_frames(Sprite::NONE);
	_body_turn_template[1].bound = { 0, 0, 10, 10 };

	_body_turn_template[2].frames = _body_turn_rotations.get_rotation_frames(Sprite::ONE);
	_body_turn_template[2].bound = { 0, 0, 10, 10 };
	_body_turn_template[2].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_body_turn_template[3].frames = _body_turn_rotations.get_rotation_frames(Sprite::ONE);
	_body_turn_template[3].bound = { 0, 0, 10, 10 };

	_body_turn_template[4].frames = _body_turn_rotations.get_rotation_frames(Sprite::TWO);
	_body_turn_template[4].bound = { 0, 0, 10, 10 };
	_body_turn_template[4].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_body_turn_template[5].frames = _body_turn_rotations.get_rotation_frames(Sprite::TWO);
	_body_turn_template[5].bound = { 0, 0, 10, 10 };

	_body_turn_template[6].frames = _body_turn_rotations.get_rotation_frames(Sprite::THREE);
	_body_turn_template[6].bound = { 0, 0, 10, 10 };
	_body_turn_template[6].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_body_turn_template[7].frames = _body_turn_rotations.get_rotation_frames(Sprite::THREE);
	_body_turn_template[7].bound = { 0, 0, 10, 10 };
	/* body */

	/* tail */
	_tail_straight_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyTailStraight.png", 4, 3, 10, 20, 12, _red, _green, _blue);
	_tail_straight_template[0].frames = _tail_straight_rotations.get_rotation_frames(Sprite::NONE);
	_tail_straight_template[0].bound = { 0, 0, 10, 20 };
	_tail_straight_template[1].frames = _tail_straight_rotations.get_rotation_frames(Sprite::ONE);
	_tail_straight_template[1].bound = { -10, 0, 20, 10 };
	_tail_straight_template[2].frames = _tail_straight_rotations.get_rotation_frames(Sprite::TWO);
	_tail_straight_template[2].bound = { 0, -10, 10, 20 };
	_tail_straight_template[3].frames = _tail_straight_rotations.get_rotation_frames(Sprite::THREE);
	_tail_straight_template[3].bound = { 0, 0, 20, 10 };

	_tail_turn_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyTailTurn.png", 4, 3, 10, 20, 12, _red, _green, _blue);
	_tail_turn_template[0].frames = _tail_turn_rotations.get_rotation_frames(Sprite::NONE);
	_tail_turn_template[0].bound = { 0, 0, 10, 20 };
	_tail_turn_template[0].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_tail_turn_template[1].frames = _tail_turn_rotations.get_rotation_frames(Sprite::NONE);
	_tail_turn_template[1].bound = { 0, 0, 10, 20 };

	_tail_turn_template[2].frames = _tail_turn_rotations.get_rotation_frames(Sprite::ONE);
	_tail_turn_template[2].bound = { -10, 0, 20, 10 };
	_tail_turn_template[2].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_tail_turn_template[3].frames = _tail_turn_rotations.get_rotation_frames(Sprite::ONE);
	_tail_turn_template[3].bound = { -10, 0, 20, 10 };

	_tail_turn_template[4].frames = _tail_turn_rotations.get_rotation_frames(Sprite::TWO);
	_tail_turn_template[4].bound = { 0, -10, 10, 20 };
	_tail_turn_template[4].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	_tail_turn_template[5].frames = _tail_turn_rotations.get_rotation_frames(Sprite::TWO);
	_tail_turn_template[5].bound = { 0, -10, 10, 20 };

	_tail_turn_template[6].frames = _tail_turn_rotations.get_rotation_frames(Sprite::THREE);
	_tail_turn_template[6].bound = { 0, 0, 20, 10 };
	_tail_turn_template[6].flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
	_tail_turn_template[7].frames = _tail_turn_rotations.get_rotation_frames(Sprite::THREE);
	_tail_turn_template[7].bound = { 0, 0, 20, 10 };

	_tail_wait_rotations.LoadRotations(_graphics->get_renderer(), "Art/Snake/GreyTailWait.png", 1, 1, 10, 10, 1, _red, _green, _blue);
	_tail_wait_template[0].frames = _tail_wait_rotations.get_rotation_frames(Sprite::NONE);
	_tail_wait_template[0].bound = { 0, 0, 10, 10 };
	_tail_wait_template[1].frames = _tail_wait_rotations.get_rotation_frames(Sprite::ONE);
	_tail_wait_template[1].bound = { 0, 0, 10, 10 };
	_tail_wait_template[2].frames = _tail_wait_rotations.get_rotation_frames(Sprite::TWO);
	_tail_wait_template[2].bound = { 0, 0, 10, 10 };
	_tail_wait_template[3].frames = _tail_wait_rotations.get_rotation_frames(Sprite::THREE);
	_tail_wait_template[3].bound = { 0, 0, 10, 10 };
	/* tail */
}

void Snake::HandleInput(SDL_Keycode key_)
{
	if (_debug_input)
	{
		switch (key_)
		{
		case SDLK_UP:
			printf("tryU ");
			break;
		case SDLK_DOWN:
			printf("tryD ");
			break;
		case SDLK_LEFT:
			printf("tryL ");
			break;
		case SDLK_RIGHT:
			printf("tryR ");
			break;
		}
	}

	Direction ahead_direction;
	Direction opposite_direction;
	Direction key_direction;
	
	if (_input_queue.get_size() == 0)
	{
		ahead_direction = _last_direction;
	}
	else
	{
		ahead_direction = _input_queue.peek();
	}

	opposite_direction = get_opposite_direction(ahead_direction);
	key_direction = direction_from_key(key_);

	// If it is a turn, allow it. If it is the same direction as forward, and it is moving there is no need to enqueue.
	if (key_direction != INVALID && (_state != MOVING || ahead_direction != key_direction) && opposite_direction != key_direction)
	{
		_input_queue.enqueue(key_direction);
		if (_debug_input)
		{
			std::string key_symbol;
			switch (key_direction)
			{
			case UP:
				key_symbol = "U";
				break;
			case DOWN:
				key_symbol = "D";
				break;
			case LEFT:
				key_symbol = "L";
				break;
			case RIGHT:
				key_symbol = "R";
				break;
			}
			printf("+%s%02d ", key_symbol.c_str(), _input_queue.get_size());
		}

		_ticks_per_frame = 1;
	}

	if (_debug_animations)
	{
		if (key_ == SDLK_e)
		{
			_queued_anim_update = 1;
		}
		else if (key_ == SDLK_q)
		{
			_queued_anim_update = -1;
		}
	}
}

void Snake::HandleRepeatInput(SDL_Keycode key_)
{
	if (_queued_direction == UP && key_ == SDLK_UP)
	{
		_ticks_per_frame = 1;
	}
	else if (_queued_direction == DOWN && key_ == SDLK_DOWN)
	{
		_ticks_per_frame = 1;
	}
	else if (_queued_direction == LEFT && key_ == SDLK_LEFT)
	{
		_ticks_per_frame = 1;
	}
	else if (_queued_direction == RIGHT && key_ == SDLK_RIGHT)
	{
		_ticks_per_frame = 1;
	}
}

Uint16 Snake::Update()
{
	_update_status = NONE;
	switch (_state)
	{
	case SnakeState::AGAINST_SEGMENT:
		_update_status |= HIT_SELF;
		break;
	case SnakeState::AGAINST_WALL:
		_update_status |= HIT_WALL;
		break;
	}

	switch (_state)
	{
	case SnakeState::MOVING:
		if(_debug_input) printf("%04d ", _current_tick);

		if (_current_tick >= _ticks_per_frame * _frames_per_action)
		{
			ActionComplete();
			_current_tick = 0;
			if (_debug_input) printf("\n");
		}
		else
		{
			if (_current_tick % _ticks_per_frame == 0 && _current_tick != 0)
			{
				if (_debug_animations) {
					_debug_anim->advance_frame();
				}

				_head->AdvanceAllAnimations();
				//_head->AdvanceAllAnimations();
				//_head->AdvanceAllAnimations();
			}

			_current_tick++;
		}
		break;
	case SnakeState::AGAINST_SEGMENT:
	case SnakeState::AGAINST_WALL:
		_fail_tick++;
		if (_fail_tick >= _ticks_before_fail)
		{
			_state = SnakeState::DEAD;
			_update_status |= DIED;
		}

		if (_input_queue.get_size() > 0)
		{
			OnTile next_on_tile = CheckNextTile(_input_queue.peek());
			if (next_on_tile == NOTHING || next_on_tile == FOOD)
			{
				CommitInput();
				CommitHeadDirection();
				_fail_tick = 0;
				_state = MOVING;
			}
			else
			{
				if (_debug_input) printf("cl:%d ", _input_queue.get_size());
				_input_queue.clear();
			}
		}

		break;
	case SnakeState::DEAD:
		break;
	}

	if (_debug_animations)
	{
		if (_debug_animation_tick >= _ticks_per_frame * _frames_per_action)
		{
			if (_queued_anim_update < 0) {
				_current_anim = ((_current_anim + 36) + _queued_anim_update) % 36;
			}
			else if (_queued_anim_update > 0)
			{
				_current_anim = (_current_anim + _queued_anim_update) % 36;
			}
			UpdateTmpAnim();
			_queued_anim_update = 0;

			_debug_animation_tick = 0;
		}
		else
		{
			if (_debug_animation_tick % _ticks_per_frame == 0 && _debug_animation_tick != 0)
			{
				_debug_anim->advance_frame();
			}

			_debug_animation_tick++;
		}
	}

	return _update_status;
}

void Snake::Render()
{
	_head->RenderAll(_graphics->get_renderer());
	if(_debug_animations) _debug_anim->Render(_graphics->get_renderer());
}

Snake::SnakeState Snake::get_state()
{
	return _state;
}

void Snake::UpdateTmpAnim()
{
	if (_queued_anim_update != 0)
	{
		SegmentAnimationTemplate *fill_anim = &_body_straight_template[0];
		if (_current_anim < 4)
		{
			fill_anim = &_head_straight_template[_current_anim];
		}
		else if (_current_anim < 12)
		{
			fill_anim = &_head_turn_template[_current_anim - 4];
		}
		else if (_current_anim < 16)
		{
			fill_anim = &_body_straight_template[_current_anim - 12];
		}
		else if (_current_anim < 24)
		{
			fill_anim = &_body_turn_template[_current_anim - 16];
		}
		else if (_current_anim < 28)
		{
			fill_anim = &_tail_straight_template[_current_anim - 24];
		}
		else if (_current_anim < 36)
		{
			fill_anim = &_tail_turn_template[_current_anim - 28];
		}

		Tile* tile = _grid->get_tile(_debug_animation_tile.y, _debug_animation_tile.x);
		_debug_anim->set_frames(fill_anim->frames);
		_debug_anim->set_pos(fill_anim->bound.x * _grid->get_scale_factor() + tile->pixel_rect.x, fill_anim->bound.y * _grid->get_scale_factor() + tile->pixel_rect.y);
		_debug_anim->set_bounds(fill_anim->bound.w * _grid->get_scale_factor(), fill_anim->bound.h * _grid->get_scale_factor());
		_debug_anim->set_flip(fill_anim->flip);
		_debug_anim->restart();
	}

}

Segment* Snake::CreateSegment()
{
	Animation* seg_animation = new Animation(NULL, true);
	Segment* segment = new Segment(seg_animation);
	return segment;
}

void Snake::ActionComplete()
{
	update_based_on_direction(_last_direction, _head_x, _head_y);
	CheckHeadTile();
	UpdateSegments();

	CommitInput();

	switch (CheckNextTile(_queued_direction))
	{
	case FOOD:
	case NOTHING:
		_ticks_per_frame = 1;
		CommitHeadDirection();
		break;
	case SEGMENT:
		_state = SnakeState::AGAINST_SEGMENT;
		_update_status |= HIT_SELF_INITIAL;
		if (_debug_input) printf("cl:%d ", _input_queue.get_size());
		_input_queue.clear();
		break;
	case WALL:
		_state = SnakeState::AGAINST_WALL;
		_update_status |= HIT_WALL_INITIAL;
		if (_debug_input) printf("cl:%d ", _input_queue.get_size());
		_input_queue.clear();
		break;
	}
}

void Snake::UpdateSegments()
{
	if (_length <= 3)
	{
		int tail_x = _tail->get_tile()->grid_x, tail_y = _tail->get_tile()->grid_y;
		update_based_on_direction(_tail->get_incoming(), tail_x, tail_y);
		_tail->set_tile(_grid->get_tile(tail_y, tail_x));
		_tail->set_incoming(_tail->get_previous()->get_incoming());
		_tail->set_outgoing(_tail->get_previous()->get_outgoing());

		Segment* next = _head->get_next();
		int middle_x = next->get_tile()->grid_x, middle_y = next->get_tile()->grid_y;
		update_based_on_direction(next->get_incoming(), middle_x, middle_y);
		next->set_tile(_grid->get_tile(middle_y, middle_x));
		next->set_incoming(next->get_previous()->get_incoming());
		next->set_outgoing(next->get_previous()->get_outgoing());

		_head->set_tile(_grid->get_tile(_head_y, _head_x));

		_grid->set_on_tile(tail_x, tail_y, OnTile::NOTHING);
		_grid->set_on_tile(_head_y, _head_x, OnTile::SEGMENT);
	}
	else
	{
		if (!_extending)
		{
			int tail_x = _tail->get_tile()->grid_x, tail_y = _tail->get_tile()->grid_y;
			_grid->set_on_tile(tail_y, tail_x, OnTile::NOTHING);

			update_based_on_direction(_tail->get_incoming(), tail_x, tail_y);
			_tail->set_tile(_grid->get_tile(tail_y, tail_x));
			_tail->set_incoming(_tail->get_previous()->get_incoming());
			_tail->set_outgoing(_tail->get_previous()->get_outgoing());
			UpdateSegmentAnimation(_tail);
		}
		else
		{
			_tail->set_outgoing(get_opposite_direction(_tail->get_incoming()));
			UpdateSegmentAnimation(_tail);
			_extending = false;
		}

		Segment* after_tail = _tail->get_previous();
		after_tail->remove();
		_head->insert_before(after_tail);
		after_tail->set_incoming(_head->get_incoming());
		after_tail->set_outgoing(_head->get_outgoing());
		after_tail->set_tile(_head->get_tile());

		_head->set_tile(_grid->get_tile(_head_y, _head_x));

		UpdateSegmentAnimation(_head->get_next());

		_grid->set_on_tile(_head_y, _head_x, OnTile::SEGMENT);
	}
}

void Snake::UpdateSegmentAnimation(Segment * segment_)
{
	SegmentAnimationTemplate* animation_template = &_body_straight_template[0];
	
	int index = 0;
	bool straight = false;

	switch (segment_->get_outgoing())
	{
	case DOWN:
		switch (segment_->get_incoming())
		{
		case LEFT:
			index = 0;
			break;
		case UP:
			straight = true;
			index = 0;
			break;
		case RIGHT:
			index = 1;
			break;
		}
		break;
	case LEFT:
		switch (segment_->get_incoming())
		{
		case UP:
			index = 2;
			break;
		case RIGHT:
			straight = true;
			index = 1;
			break;
		case DOWN:
			index = 3;
			break;
		}
		break;
	case UP:
		switch (segment_->get_incoming())
		{
		case RIGHT:
			index = 4;
			break;
		case DOWN:
			straight = true;
			index = 2;
			break;
		case LEFT:
			index = 5;
			break;
		}
		break;
	case RIGHT:
		switch (segment_->get_incoming())
		{
		case DOWN:
			index = 6;
			break;
		case LEFT:
			straight = true;
			index = 3;
			break;
		case UP:
			index = 7;
			break;
		}
		break;
	}

	switch (segment_->get_type())
	{
	case Segment::SegmentType::HEAD:
		if (straight)
		{
			animation_template = &_head_straight_template[index];
		}
		else
		{
			animation_template = &_head_turn_template[index];
		}
		break;
	case Segment::SegmentType::BODY:
		if (straight)
		{
			animation_template = &_body_straight_template[index];
		}
		else
		{
			animation_template = &_body_turn_template[index];
		}
		break;
	case Segment::SegmentType::TAIL:
		if (!_extending)
		{
			if (straight)
			{
				animation_template = &_tail_straight_template[index];
			}
			else
			{
				animation_template = &_tail_turn_template[index];
			}
		}
		else
		{
			animation_template = &_tail_wait_template[index];
		}
		break;
	}

	segment_->UpdateAnimation(*animation_template, _grid->get_scale_factor());
}

void Snake::CommitHeadDirection()
{
	_update_status |= MOVED;
	if (_queued_direction != _last_direction)
	{
		_update_status |= TURNED;
	}

	_head->set_outgoing(get_opposite_direction(_last_direction));
	_head->set_incoming(_queued_direction);
	UpdateSegmentAnimation(_head);
	_last_direction = _queued_direction;
	_head->ResetAllAnimations();
}

void Snake::CheckHeadTile()
{
	Tile* tile = _grid->get_tile(_head_y, _head_x);
	switch(tile->on_tile)
	{
	case FOOD:
		Extend();
		_grid->set_on_tile(_head_y, _head_x, NOTHING);
		_update_status |= ATE;
		break;
	}
}

OnTile Snake::CheckNextTile(Direction direction_)
{
	int next_x = _head_x, next_y = _head_y;
	update_based_on_direction(direction_, next_x, next_y, false);
	if (0 > next_x || next_x > _grid->get_grid_size() - 1
		|| 0 > next_y || next_y > _grid->get_grid_size() - 1)
	{
		return WALL;
	}
	else
	{
		return _grid->get_tile(next_y, next_x)->on_tile;
	}
}

void Snake::CommitInput()
{
	if (_input_queue.has_more())
	{
		_queued_direction = _input_queue.pop();
		switch (_queued_direction)
		{
		case UP:
			if (_debug_input) printf("-U%02d ", _input_queue.get_size());
			break;
		case DOWN:
			if (_debug_input) printf("-D%02d ", _input_queue.get_size());
			break;
		case LEFT:
			if (_debug_input) printf("-L%02d ", _input_queue.get_size());
			break;
		case RIGHT:
			if (_debug_input) printf("-R%02d ", _input_queue.get_size());
			break;
		}
	}
}

void Snake::Extend()
{
	_extending = true;
	Segment* segment = CreateSegment();
	segment->set_type(Segment::SegmentType::BODY);
	_tail->get_previous()->insert_before(segment);
}

int Snake::update_based_on_direction(Direction direction_, int& x_, int& y_, bool force_bound_)
{
	switch (direction_)
	{
	case UP:
		if (y_ == 0 && force_bound_)
		{
			return 0;
		}
		else
		{
			y_--;
		}
		break;
	case DOWN:
		if (y_ == _grid->get_grid_size() - 1 && force_bound_)
		{
			return 0;
		}
		else
		{
			y_++;
		}
		break;
	case LEFT:
		if (x_ == 0 && force_bound_)
		{
			return 0;
		}
		else
		{
			x_--;
		}
		break;
	case RIGHT:
		if (x_ == _grid->get_grid_size() - 1 && force_bound_)
		{
			return 0;
		}
		else
		{
			x_++;
		}
		break;
	}

	return 1;
}

Direction Snake::direction_from_key(SDL_Keycode key_)
{
	switch (key_)
	{
	case SDLK_UP:
		return UP;
	case SDLK_DOWN:
		return DOWN;
	case SDLK_LEFT:
		return LEFT;
	case SDLK_RIGHT:
		return RIGHT;
	default:
		return INVALID;
	}
}
