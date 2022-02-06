#ifndef THREATS_OBJECT_H
#define THREATS_OBJECT_H
#define WIDTH_THREAT 80
#define HEIGHT_THREAT 33
#include "BaseObject.h"
#include "Common_Function.h"
#include "AmoObject.h"
#include <vector>
class ThreatObject : public BaseObject
{
public:
	ThreatObject();
	~ThreatObject();
	void InitAmo(AmoObject* p_amo);

	void HandleInputAction(SDL_Event events);
	void HandleMove(const int& x_border, const int& y_border);
	void MakeAmo(SDL_Surface* des, const int& x_limit, const int& y_limit);
	void set_x_val(const int& val) { x_val_ = val; }
	void set_y_val(const int& val) { y_val_ = val; }
	int get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }
	void Reset(const int& xborder);
	void ResetAmo(AmoObject* p_amo);
	std::vector<AmoObject*> GetAmoList() const { return p_amo_list_; }
private:
	int x_val_;
	int y_val_;
	std::vector<AmoObject*> p_amo_list_;
};
#endif

