#include "Segment.h"



Segment::Segment(Animation* anim_) : _anim(anim_), _next(NULL), _previous(NULL) {}


Segment::~Segment()
{
}

void Segment::RenderAll(SDL_Renderer * renderer_)
{
	_anim->Render(renderer_);

	if (_next != NULL)
	{
		_next->RenderAll(renderer_);
	}
}

void Segment::AdvanceAllAnimations()
{
	_anim->advance_frame();

	if(_next != NULL)
	{
		_next->AdvanceAllAnimations();
	}
}

void Segment::ResetAllAnimations()
{
	_anim->restart();

	if (_next != NULL)
	{
		_next->ResetAllAnimations();
	}
}

void Segment::UpdateAnimation(const SegmentAnimationTemplate &animation_template_, int scale_factor_)
{
	_anim->set_frames(animation_template_.frames);
	_anim->set_pos(animation_template_.bound.x * scale_factor_ + _tile->pixel_rect.x, animation_template_.bound.y * scale_factor_ + _tile->pixel_rect.y);
	_anim->set_bounds(animation_template_.bound.w * scale_factor_, animation_template_.bound.h * scale_factor_);
	_anim->set_flip(animation_template_.flip);
	_anim->restart();
}

Animation * Segment::get_animation()
{
	return _anim;
}

void Segment::insert_before(Segment * segment_)
{
	segment_->_previous = this;
	segment_->_next = _next;
	_next->_previous = segment_;
	_next = segment_;
}

void Segment::remove()
{
	_previous->_next = _next;
	_next->_previous = _previous;
}

void Segment::set_type(SegmentType type_)
{
	_type = type_;
}

Segment::SegmentType Segment::get_type()
{
	return _type;
}

void Segment::set_next(Segment * next_)
{
	_next = next_;
}

Segment * Segment::get_next()
{
	return _next;
}

void Segment::set_previous(Segment * previous_)
{
	_previous = previous_;
}

Segment * Segment::get_previous()
{
	return _previous;
}

void Segment::set_incoming(Direction incoming_)
{
	_incoming = incoming_;
}

Direction Segment::get_incoming()
{
	return _incoming;
}

void Segment::set_outgoing(Direction outgoing_)
{
	_outgoing = outgoing_;
}

Direction Segment::get_outgoing()
{
	return _outgoing;
}

void Segment::set_tile(Tile * tile_)
{
	_tile = tile_;
}

Tile * Segment::get_tile()
{
	return _tile;
}
