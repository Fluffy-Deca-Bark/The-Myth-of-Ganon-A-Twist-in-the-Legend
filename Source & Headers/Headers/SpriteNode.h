#ifndef SpriteNode_H
#define SpriteNode_H

#include "LiveSprite.h"

class SpriteNode
{
	private:
		int sheet_x;
		int sheet_y;
		int screen_x;
		int screen_y;
		int frame_w;
		int frame_h;
		int w_mod;
		int h_mod;
		int layer;
		int stop_box_x1;
		int stop_box_y1;
		int stop_box_x2;
		int stop_box_y2;
		int interact_x1;
		int interact_y1;
		int interact_x2;
		int interact_y2;
		int door_direction;

		SpriteNode* ptr;
		Sprite* sprite;

	public:

		SpriteNode();
		SpriteNode (Sprite* s, int l, SpriteNode* p);
	
		int get_sheet_x();
		int get_sheet_y();
		int get_crop_x();
		int get_crop_y();
		int get_screen_x();
		int get_screen_y();
		int get_frame_w();
		int get_frame_h();
		int get_layer();

		int get_stop_box_x1();
		int get_stop_box_y1();
		int get_stop_box_x2();
		int get_stop_box_y2();
		bool stop_box_is_set();
		void set_stop_box (int x1, int y1, int x2, int y2);

		SpriteNode* get_ptr();
		Sprite* get_sprite();

		void select_frame (int x, int y);
		void set_position (int x, int y);
		void set_size (int w, int h);
		void set_modifiers (int wm, int hm);

		void set_ptr (SpriteNode* p);
		void set_sprite (Sprite* s);

		SpriteNode* insert_node (Sprite* s, int l);
		void clear();
		void draw_list (iGraphics* i);
		void draw (iGraphics* i);

		void print_node();
		void print_sprite_file_name();
		void print_node_line();

		int get_width();
		int get_height();

		void set_door_direction (int d);
		int get_door_direction();

		bool check_stop_box_collision (direction d, int n1, int n2);
};


#endif