#include "PLayerHeart.h"
#include "Common_Function.h"

PlayerHeart::PlayerHeart()
{

}
PlayerHeart::~PlayerHeart()
{

}
void PlayerHeart::AddPos(const int& xpos)
{
	pos_list_.push_back(xpos);
}
void PlayerHeart::Render(SDL_Surface* des)
{
	if (number_ == pos_list_.size())
	{
		for (int i = 0; i < pos_list_.size(); i++)
		{
			rect_.x = pos_list_.at(i);
			Show(des);
		}
	}
}
void PlayerHeart::Init()
{
	LoadImg(g_name_heart_player);
	number_ = 3;
	if (pos_list_.size() > 0)
	{
		pos_list_.clear();
	}
	AddPos(5);
	AddPos(35);
	AddPos(65);
}
void PlayerHeart::Decrease()
{
	number_--;
	pos_list_.pop_back();
}
