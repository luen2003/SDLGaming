#include "ExplosionObject.h"
ExplosionObject::ExplosionObject()
{
	frame_ = 0;
}
ExplosionObject::~ExplosionObject() 
{
	;
}
void ExplosionObject::set_clip()
{
	clip[0].x = 0;
	clip[0].y = 0;
	clip[0].w = EXP_WIDTH;
	clip[0].h = EXP_HEIGHT;

	clip[1].x = EXP_WIDTH;
	clip[1].y = 0;
	clip[1].w = EXP_WIDTH;
	clip[1].h = EXP_HEIGHT;

	clip[2].x = EXP_WIDTH*2;
	clip[2].y = 0;
	clip[2].w = EXP_WIDTH;
	clip[2].h = EXP_HEIGHT;

	clip[3].x = EXP_WIDTH*3;
	clip[3].y = 0;
	clip[3].w = EXP_WIDTH;
	clip[3].h = EXP_HEIGHT;

}


void ExplosionObject::set_frame(const int& fr) { frame_ = fr; }
void ExplosionObject::ShowEx(SDL_Surface* des)
{
	if (frame_ >= 4)
	{
		frame_ = 0;
	}
	SDLCommonFunc::ApplySurfaceClip(this->p_object_, des, &clip[frame_], rect_.x, rect_.y);
}


