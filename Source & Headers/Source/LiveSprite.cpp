#include "LiveSprite.h"

LiveSprite::LiveSprite (int w, int h, int sheet_X, int sheet_Y, int screen_X, int screen_Y)
{
	frame_w = w;
	frame_h = h;
	sheet_x = sheet_X;
	sheet_y = sheet_Y;
	screen_x = screen_X;
	screen_y = screen_Y;

	walk_speed = 1;
	dash_speed = 6;
	dashing = false;
};

void LiveSprite::move_d (direction d)
{
	int s = get_current_speed();

	switch (d)
	{
		case up: move (0, -s); break;
		case right: move (s, 0); break;
		case down: move (0, s); break;
		case left: move (-s, 0); break;
	};
};

void LiveSprite::set_dashing (bool d)
{
	dashing = d;
};

void LiveSprite::toggle_dashing()
{
	dashing = !dashing;
};

bool LiveSprite::is_dashing()
{
	return dashing;
};

int LiveSprite::get_current_speed()
{
	return (dashing ? dash_speed : walk_speed);
};