#include "Sprite.h"

Sprite::Sprite()
{
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;
};

Sprite::Sprite (int w, int h)
{
	frame_w = w;
	frame_h = h;
	w_mod = 0;
	h_mod = 0;

	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;
};

Sprite::Sprite (int w, int h, const char* p)
{
	frame_w = w;
	frame_h = h;
	int count = 0;

	for (int i=0; p[i]!='\0'; i++)
		path[i] = p[i];

	w_mod = 0;
	h_mod = 0;
	
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;
};

Sprite::Sprite (int w, int h, int sheet_X, int sheet_Y, int screen_X, int screen_Y)
{
	frame_w = w;
	frame_h = h;
	sheet_x = sheet_X;
	sheet_y = sheet_Y;
	screen_x = screen_X;
	screen_y = screen_Y;
	w_mod = 0;
	h_mod = 0;
	
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;
};

void Sprite::set_frame_w (int w)
{
	frame_w = w;
};

void Sprite::set_frame_h (int h)
{
	frame_h = h;
};

int Sprite::get_frame_w()
{
	return frame_w;
};

int Sprite::get_frame_h()
{
	return frame_h;
};

int Sprite::get_crop_x()
{
	return sheet_x * frame_w;
};

int Sprite::get_crop_y()
{
	return sheet_y * frame_h;
};

int Sprite::get_screen_x()
{
	return screen_x;
};

int Sprite::get_screen_y()
{
	return screen_y;
};

int Sprite::get_sheet_x()
{
	return sheet_x;
};

int Sprite::get_sheet_y()
{
	return sheet_y;
};

int Sprite::get_width()
{
	if (w_mod == 0)
		return frame_w;

	return w_mod;
};

int Sprite::get_height()
{
	if (h_mod == 0)
		return frame_h;

	return h_mod;
};

int Sprite::get_stop_box_x1()
{
	return stop_box_x1;
};

int Sprite::get_stop_box_y1()
{
	return stop_box_y1;
};

int Sprite::get_stop_box_x2()
{
	return stop_box_x2;
};

int Sprite::get_stop_box_y2()
{
	return stop_box_y2;
};

void Sprite::set_stop_box (int x1, int y1, int x2, int y2)
{
	stop_box_x1 = x1;
	stop_box_y1 = y1;
	stop_box_x2 = x2;
	stop_box_y2 = y2;
};

bool Sprite::stop_box_is_set()
{
	if (stop_box_x1 == -1 || stop_box_y2 == -1 || stop_box_x2 == -1 || stop_box_y2 == -1)
		return false;
	return true;
};

void Sprite::select_frame (int x, int y)
{
	sheet_x = x;
	sheet_y = y;
};

void Sprite::load (const char* sub_folder_path, char* file_name)
{
	cat_path (path, sub_folder_path, file_name);
	LoadPNGImage (path);
};

void Sprite::load_direct (char* file_path)
{
	LoadPNGImage (file_path);
};

void Sprite::print_path()
{
	printf("\nSprite path:\n\t%s\n", path);
};

void Sprite::print_pos ()
{
	printf ("(sheet_x = %d , sheet_y = %d , screen_x = %d , screen_y = %d , w = %d , h = %d)\n", sheet_x, sheet_y, screen_x, screen_y, frame_w, frame_h);
};

void Sprite::set_position (int x, int y)
{
	screen_x = x;
	screen_y = y;
};

void Sprite::draw (Graphics* g)
{
	int cx = sheet_x * frame_w;
	int cy = sheet_y * frame_h;

	g->DrawImage2D (screen_x, screen_y, frame_w, frame_h, cx, cy, frame_w, frame_h, *this);
};

void Sprite::draw (iGraphics* i)
{
	int cx = sheet_x * frame_w;
	int cy = sheet_y * frame_h;

	int dw = (w_mod == 0) ? frame_w : w_mod;
	int dh = (h_mod == 0) ? frame_h : h_mod;

	i->draw_image (screen_x, screen_y, dw, dh, cx, cy, frame_w, frame_h, *this);
};

void Sprite::move (int x, int y)
{
	static int last_step_right_leg = false;

	screen_x += x;
	screen_y += y;

	if (x > 0)
	{
		if (sheet_y != 3)
		{
			sheet_x = 0;
			sheet_y = 3;
		};

		if (sheet_y == 3)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	}
	else if (x < 0)
	{
		if (sheet_y != 1)
		{
			sheet_x = 0;
			sheet_y = 1;
		};

		if (sheet_y == 1)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	}
	else if (y > 0)
	{
		if (sheet_y != 0)
		{
			sheet_x = 0;
			sheet_y = 0;
		};

		if (sheet_y == 0)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		}
	}
	else if (y < 0)
	{
		if (sheet_y != 2)
		{
			sheet_x = 0;
			sheet_y = 2;
		};

		if (sheet_y == 2)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	};
};
		
void Sprite::move (int n, axis a)
{
	a == horizontal ? move(n,0) : move(0,n);
};

char* Sprite::get_path()
{
	return path;
};

char* Sprite::get_name (char* buffer)
{
	int last_backslash = 0;
	int explorer = 0;
	int buffer_i = 0;
	
	//delete buffer;
	buffer = new char[40];

	for (;;)
	{
		explorer++;
		if (path[explorer] == '\\')
		{
			last_backslash = explorer;
			continue;
		}
		else if (path[explorer] == '\0')
		{
			break;
		};
	};

	for (int path_i = last_backslash+1; path[path_i] != '.'; path_i++, buffer_i++)
	{
		buffer[buffer_i] = path[path_i];
	};
	buffer[buffer_i] = '\0';

	return buffer;
};

void Sprite::set_modifiers (int wm, int hm)
{
	w_mod = wm;
	h_mod = hm;
};