#ifndef LiveSprite_H
#define LiveSprite_H

#include "Sprite.h"

class LiveSprite : public Sprite
{
private:
	int walk_speed;
	int dash_speed;
	bool dashing;
	//bool allowed_to_move[4];
	int move_restriction[4];

public:
	LiveSprite (int w, int h, int sheet_X, int sheet_Y, int screen_X, int screen_Y);
	void set_dashing (bool dash);
	void move_d (direction d);
	void toggle_dashing();
	bool is_dashing();
	int get_current_speed();
	//void set_allowed_to_move (direction d, bool b);
	void set_move_restriction (direction d, int r);
};

#endif