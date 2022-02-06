#include "ThreatObject.h"
ThreatObject::ThreatObject()
{
	rect_.x = SCREEN_WIDTH;
	rect_.y = (int)SCREEN_HEIGHT/2;
	rect_.w = WIDTH_THREAT;
	rect_.h = HEIGHT_THREAT;
	x_val_ = 0;
	y_val_ = 0;
}
ThreatObject:: ~ThreatObject()
{
	if (p_amo_list_.size() > 0)
	{
		for (size_t it = 0; it < p_amo_list_.size(); it++)
		{
			AmoObject* p_amo = p_amo_list_.at(it);
			if (p_amo != NULL)
			{
				delete p_amo;
				p_amo = NULL;

			}
		}
		p_amo_list_.clear();
	}
}
void ThreatObject::InitAmo(AmoObject* p_amo)
{

	if (p_amo != NULL)
	{
		bool ret = p_amo->LoadImg("sphere_threats.png");
			if (ret)
			{
				p_amo->set_is_move(true);
				p_amo->SetWidthHeight(WIDTH_SPHERE, HEIGHT_SPHERE);
				p_amo->set_type(AmoObject::SPHERE);
				p_amo->SetRect(rect_.x, rect_.y + rect_.h/2);
				p_amo->set_x_val(SPEED_AMO_THREATS);
				p_amo_list_.push_back(p_amo);
			}
	}
}
void ThreatObject::MakeAmo(SDL_Surface* des, const int& x_limit, const int& y_limit)
{
	for (std:: size_t i = 0; i< p_amo_list_.size(); i++)
	{
		
		AmoObject* p_amo = p_amo_list_.at(i);
		if (p_amo != NULL)
		{
			if (p_amo->get_is_move())
			{
				p_amo->Show(des);
				p_amo->HandleMoveRighttoLeft();
			}
			else
			{
				p_amo->set_is_move(true);
				p_amo->SetRect(rect_.x, rect_.y + floor(rect_.h * 0.5));
			}
		}
	}
}
void ThreatObject::HandleInputAction(SDL_Event events)
{

}
void ThreatObject::HandleMove(const int& x_border, const int& y_border)
{
	rect_.x -= x_val_;
	if (rect_.x < 0)
	{
		rect_.x = SCREEN_WIDTH;
		int rect_y = rand() % VAL_OFFSET_START_POST_THREAT;
		if (rect_y > SCREEN_HEIGHT - UNDER_LIMIT_THREAT)
		{
			rect_y = floor(SCREEN_HEIGHT * 0.3);
		}
		rect_.y = rect_y;
	}
}
void ThreatObject::ResetAmo(AmoObject * p_amo)
{
		p_amo->SetRect(rect_.x, rect_.y + rect_.h / 2);

}
void ThreatObject::Reset(const int& xborder)
{
	rect_.x = xborder;
	rect_.x = SCREEN_WIDTH;
	int rect_y = rand() % VAL_OFFSET_START_POST_THREAT;
	if (rect_y > SCREEN_HEIGHT - UNDER_LIMIT_THREAT)
	{
		rect_y = floor(SCREEN_HEIGHT * 0.3);
	}
	rect_.y = rect_y;
	for (int i = 0; i < p_amo_list_.size(); i++)
	{
		AmoObject* p_amo = p_amo_list_.at(i);
		if (p_amo)
		{
			ResetAmo(p_amo);
		}
	}

}


	