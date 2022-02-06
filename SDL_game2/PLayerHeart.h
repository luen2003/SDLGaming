#ifndef PLAYER_HEART_H
#define PLAYER_HEART_H
#include "BaseObject.h"
#include <vector>

class PlayerHeart : public BaseObject
{
public:
	PlayerHeart();
	~PlayerHeart();
	void SetNumber(const int& num) { number_ = num; }
	void AddPos(const int& xpos);
	void Render(SDL_Surface* des);
	void Init();
	void Decrease();

private:
	int number_;
	std::vector<int> pos_list_;

};
#endif

