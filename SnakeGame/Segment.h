#pragma once
#include <SDL.h>
#include "Direction.h"
#include "Animation.h"
#include "Grid.h"

struct SegmentAnimationTemplate
{
	SpriteAnimationFrames* frames;
	SDL_Rect bound;
	SDL_RendererFlip flip;
};

class Segment
{
public:
	enum SegmentType
	{
		HEAD,
		BODY,
		TAIL
	};

	Segment(Animation* anim_);
	~Segment();

	void RenderAll(SDL_Renderer* renderer_);
	void AdvanceAllAnimations();
	void ResetAllAnimations();
	void UpdateAnimation(const SegmentAnimationTemplate &animation_template_, int scale_factor_);
	Animation* get_animation();
	void insert_before(Segment* segment_);
	void remove();
	void set_type(SegmentType type_);
	SegmentType get_type();
	void set_next(Segment* next_);
	Segment* get_next();
	void set_previous(Segment* previous_);
	Segment* get_previous();
	void set_incoming(Direction incoming_);
	Direction get_incoming();
	void set_outgoing(Direction outgoing_);
	Direction get_outgoing();
	void set_tile(Tile* tile_);
	Tile* get_tile();
private:
	SegmentType _type;
	Animation* _anim;
	Tile* _tile;

	Direction _incoming;
	Direction _outgoing;

	Segment* _next;
	Segment* _previous;
};

