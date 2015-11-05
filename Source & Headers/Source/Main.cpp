#ifndef __Main__
#define __Main__

#include "SaveState.h"
#include "LiveSprite.h"
#include "iGraphics.h"
#include "MapBuilder.h"
#include "SpriteNode.h"
#include <math.h>

SaveState save_state;
MapBuilder map_builder;
iGraphics iGraph;
Image Ganondorfs_castle;
LiveSprite Ganondorf (40, 40, 1, 0, 80, 190);

Sprite portal (337, 270);
Sprite medallion (50, 50);
Sprite easter_egg (83, 58); //49, 51
Sprite forest_map (TILE_SIZE, TILE_SIZE);
Sprite Epona (48, 31);
Sprite heart (22, 19);
Sprite rupee (21, 24);
Sprite mp (1,12);
Sprite keyboard (56, 49);
Sprite box (260, 260);

Sprite spell (260, 260);

Sprite dins_fire (260, 260);
Sprite nayrus_love (260, 260);
Sprite farores_wind (260, 260);
Sprite f_wind_warpball (260, 260);
Sprite warp (260, 260);
void cast_Dins_Fire();
void cast_Nayrus_Love();
void cast_Farores_Wind();
int FW_x = -1;
int FW_y = -1;
int FW_stretch = 0;
int warping_animation = 0;
bool Ganondorf_visible = true;


SpriteNode sprite_list_head;

bool see_generated_map = false;

void load_images()
{
	char Ganondorfs_castle_path[FILE_PATH_SIZE];
	//cat_path (Ganondorfs_castle_path, "Level Design\\Casa do Ganondorf\\", "Casa do Ganondorf.png");
	cat_path (Ganondorfs_castle_path, CO, "Castle with organ and statues.png");
	Ganondorfs_castle.LoadPNGImage (Ganondorfs_castle_path);


	Ganondorf.load	(CO, "Ganondorf.png");
	Ganondorf.select_frame (0, 2);
	Ganondorf.set_position ((SCREEN_WIDTH - Ganondorf.get_frame_w()) / 2 - 8, 345);
	portal.load		(CO, "Portal.png");
	medallion.load	(CO, "Medallion.png");
	easter_egg.load (CO, "Easter Egg.png");
	forest_map.load	(TR, "Floresta 1.png");
	heart.load		(CO, "Heart.png");
	rupee.load		(CO, "Rupee.png");
	mp.load			(CO, "Mana bar.png");
	spell.load		(CO, "Spell.png");
	dins_fire.load	(CO, "Spell.png");
	nayrus_love.load(CO, "Spell.png");
	farores_wind.load(CO, "Spell.png");
	f_wind_warpball.load (CO, "Spell.png");
	warp.load		(CO, "Spell.png");
	keyboard.load	(CO, "Keys with spells.png");
	box.load		(CO, "White Square.png");


	Epona.load		(CO, "Epona.png");
	Epona.select_frame (1, 0);
	Epona.set_position (360, 230);
};

void main_loop()
{
	iGraph.DrawImage2D (0,0,736,448,0,0,736,448,Ganondorfs_castle);
	//Ganondorf.draw (&iGraph);
	//Ganondorf.print_pos();
	if (Ganondorf_visible)
		sprite_list_head.insert_node (&Ganondorf, 5);

	int portal_y = 280;

	for (int i=0; i<5; i++)
	{
		portal.select_frame (i,0);
		portal.set_position (-107 + i*120, portal_y);
		sprite_list_head.insert_node (&portal, portal.get_sheet_x() == 2 ? 4 : 5);
	};

	portal.set_position (-109 + 5*120, portal_y);
	switch (save_state.get_phase())
	{
		case 0: 
			portal.select_frame (5, 0); 
			sprite_list_head.insert_node (&portal, 5);
		break;

		case 1: 
			portal.select_frame (7, 0);
			sprite_list_head.insert_node (&portal, 5);
			portal.select_frame (8, 0);
			sprite_list_head.insert_node (&portal, 6);
			break;

		case 2:
			portal.select_frame (6, 0);
			sprite_list_head.insert_node (&portal, 5);
			break;
	};
		

	for (int i=0; i<6; i++)
	{
		if (save_state.get_temple(i))
		{
			medallion.select_frame (i, 0);
			medallion.set_position (38 + i*120, 220);
			sprite_list_head.insert_node (&medallion, 1);
		};
	};

	for (int i=0; i<6; i++)
		if (save_state.get_easter_egg(i))
		{
			easter_egg.select_frame (i, 0);
			easter_egg.set_position (19 + (SCREEN_WIDTH / 2) - ((3-i) * 109) - (30 * (3-i>0)), 414);
			sprite_list_head.insert_node (&easter_egg, 5);
		};

	if (save_state.light())
	{
		int i = 5;
		easter_egg.select_frame (i+1, 0);
		easter_egg.set_position (605, 352);
		sprite_list_head.insert_node (&easter_egg, 5);
	};




	if (see_generated_map)
	{
		MapNode* current_node = map_builder.get_list_head()->get_ptr();
		while (current_node != NULL)
		{
			forest_map.set_position (current_node->get_screen_x(), current_node->get_screen_y());
			forest_map.select_frame (current_node->get_sheet_x(), current_node->get_sheet_y());
			//forest_map.draw (&iGraph);
			sprite_list_head.insert_node (&forest_map, 3);
			current_node = current_node->get_ptr();
		};
	};



	//Epona.set_position ((SCREEN_WIDTH - Epona.get_frame_w()) / 2 - 12, 345);
	if (Ganondorf.get_screen_x() + Ganondorf.get_frame_w()/2 > Epona.get_screen_x() + Epona.get_frame_w()/2)
		Epona.select_frame (1, 0);
	if (Ganondorf.get_screen_x() + Ganondorf.get_frame_w()/2 < Epona.get_screen_x() + Epona.get_frame_w()/2)
		Epona.select_frame (0, 0);
	sprite_list_head.insert_node (&Epona, 5);





	float hearts = save_state.get_hearts();
	int whole_hearts = floor (hearts);
	float fraction = hearts - whole_hearts;
	int heart_pad = 21;
	for (int i=0; i<20; i++)
	{
		if (i < 10)				heart.set_position (heart_pad*i + 20, 20);
		else					heart.set_position (heart_pad*(i-10) + 20, 40);
		

		if (i < whole_hearts)				heart.select_frame (0, 0);
		else if (i == whole_hearts)
		{
			if (fraction == 0.0f)			heart.select_frame (4, 0);
			else if (fraction == 0.25f)		heart.select_frame (3, 0);
			else if (fraction == 0.50f)		heart.select_frame (2, 0);
			else if (fraction == 0.75f)		heart.select_frame (1, 0);
		}
		else if (i > whole_hearts)			heart.select_frame (4, 0);
	
		if (i+1 <= save_state.get_heart_containers())
			sprite_list_head.insert_node (&heart, 10);
	};





	int total_rupees = save_state.get_rupees();
	int single_digit_rupees = 0;
	char rupee_str[4];
	for (int i=0; i<3; i++)
	{
		int divisor = pow ((double) 10, (int) 2-i);
		single_digit_rupees = total_rupees / divisor;
		rupee_str[i] = '0' + single_digit_rupees;
		total_rupees -= single_digit_rupees * divisor;
	};
	rupee_str[3] = '\0';
	rupee.set_position (20, SCREEN_HEIGHT - 10);
	sprite_list_head.insert_node (&rupee, 10);
	iGraph.SetColor (255,255,255);
	iGraph.SetTextFont ("Helvetica", 25, 10, 0, 0);
	iGraph.draw_text (45, SCREEN_HEIGHT - 13, rupee_str);






	int casting_Dins_Fire = save_state.get_DF();
	int casting_Nayrus_Love = save_state.get_NL();
	int casting_Farores_Wind = save_state.get_FW();

	if (casting_Dins_Fire > 0)
	{
		int growth_rate = 2;
		if (casting_Dins_Fire > 240/2)
			growth_rate = 4;
		if (casting_Dins_Fire > 240 * 3/4)
			growth_rate = 6;
		dins_fire.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-dins_fire.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+dins_fire.get_height()/2);
		dins_fire.select_frame (0, 0);
		sprite_list_head.insert_node (&dins_fire, 6);
		SpriteNode* p;
		for (p=&sprite_list_head; p->get_sprite()!=(&dins_fire); p=p->get_ptr());
		p->set_modifiers (casting_Dins_Fire, casting_Dins_Fire);
		save_state.alter_DF(growth_rate);
		if (casting_Dins_Fire >= 240)
			save_state.set_DF (0);
	};


	
	if (casting_Nayrus_Love > 0)
	{
		static int angle = 0;
		nayrus_love.set_modifiers (70, 70);
		nayrus_love.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-nayrus_love.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+nayrus_love.get_height()/2);
		nayrus_love.select_frame (1, 0);
		sprite_list_head.insert_node (&nayrus_love, 6);
		SpriteNode* p;
		for (p=&sprite_list_head; p->get_sprite()!=(&nayrus_love); p=p->get_ptr());
		p->set_modifiers (70, 70);
		save_state.alter_NL (-1);
	};


	if (casting_Farores_Wind > 0)
	{
		if (casting_Farores_Wind < 260)
			FW_stretch = casting_Farores_Wind;
		else
			FW_stretch = 260;

		int growth_rate = 10;
		int FW_limit = 380;
		if (casting_Farores_Wind > FW_limit/2)
			growth_rate = 1;
		farores_wind.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-farores_wind.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+farores_wind.get_height()/2 + 15);
		farores_wind.select_frame (2, 0);
		sprite_list_head.insert_node (&farores_wind, 6);
		SpriteNode* p;
		for (p=&sprite_list_head; p->get_sprite()!=(&farores_wind); p=p->get_ptr());
		p->set_modifiers (FW_stretch, FW_stretch);
		save_state.alter_FW(growth_rate);
		if (casting_Farores_Wind >= FW_limit)
			save_state.set_FW (0);
	};


	if (FW_x != -1)
	{
		warp.select_frame (2, 1);
		warp.set_position (
			FW_x + Ganondorf.get_frame_w()/2 - warp.get_frame_w()/2,
			FW_y - Ganondorf.get_frame_h() + warp.get_frame_h()/2 + 20
			);
		if (casting_Farores_Wind > 50 || casting_Farores_Wind == 0)
			sprite_list_head.insert_node (&warp, 7);
	};
	




	for (int i=0; i<save_state.get_max_mp(); i++)
	{
		if (i==0)
			mp.select_frame (0, 0);
		else if (i<save_state.get_max_mp()-1)
		{
			if (save_state.get_mp() >= i)
				mp.select_frame (1,0);
			else
				mp.select_frame (2,0);
		}
		else
			mp.select_frame (3,0);

		mp.set_position (24 + i, 
			save_state.get_heart_containers() > 10 ? 55 : 36);

		sprite_list_head.insert_node (&mp, 10);
	};






	keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*3 - 10, 10 + keyboard.get_height());
	keyboard.select_frame (0, 0);
	sprite_list_head.insert_node (&keyboard, 10);

	keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*2 - 10, 10 + keyboard.get_height());
	keyboard.select_frame (1, 0);
	sprite_list_head.insert_node (&keyboard, 10);

	keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*1 - 10, 10 + keyboard.get_height());
	keyboard.select_frame (2, 0);
	sprite_list_head.insert_node (&keyboard, 10);



	


	sprite_list_head.draw_list (&iGraph);
	//sprite_list_head.print_node_line();
	sprite_list_head.clear();

	/*iGraph.SetColor (0, 255, 255);
	iGraph.draw_rectangle (
		Ganondorf.get_screen_x(), 
		Ganondorf.get_screen_y(),
		Ganondorf.get_screen_x() + Ganondorf.get_width(),
		Ganondorf.get_screen_y() - Ganondorf.get_height()
		);*/
};

int main (void)
{
	save_state.print_table();
	
	if(1)
	{
		iGraph.CreateMainWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "The Myth of Ganon: A Twist in the Legend");
		iGraph.SetKeyboardInput(KeyboardInput);
		iGraph.SetBackgroundColor (26,32,40);
		load_images();
		iGraph.SetMainLoop(main_loop);
		iGraph.StartMainLoop();
	};


	map_builder.delete_list();
	
	halt();

	return 0;
};

void cat_path (char* ptr, const char* sub_folder_path, const char* file_name)
{
	strcpy(ptr,PROJECT_PATH);
	strcat(ptr,sub_folder_path);
	strcat(ptr,file_name);
};

void KeyboardInput(int key, int state, int x, int y)
{
	if (state == KEY_STATE_DOWN)
	{
		switch (key)
		{
			case (KEY_RIGHT): Ganondorf.move (Ganondorf.get_current_speed(), horizontal); break;
			case (KEY_DOWN): Ganondorf.move (Ganondorf.get_current_speed(), vertical); break;
			case (KEY_LEFT): Ganondorf.move (-Ganondorf.get_current_speed(), horizontal); break;
			case (KEY_UP): Ganondorf.move (-Ganondorf.get_current_speed(), vertical); break;
			case (' '): Ganondorf.toggle_dashing(); break;
			case ('m'): see_generated_map = !see_generated_map; break;
			case ('v'): exit(0); break;

			case ('t'): save_state.tweak_temple(0); break;
			case ('y'): save_state.tweak_temple(1); break;
			case ('u'): save_state.tweak_temple(2); break;
			case ('i'): save_state.tweak_temple(3); break;
			case ('o'): save_state.tweak_temple(4); break;
			case ('p'): save_state.tweak_temple(5); break;

			case ('f'): save_state.alter_rupees(-100); break;
			case ('g'): save_state.alter_rupees(-10); break;
			case ('h'): save_state.alter_rupees(-1); break;
			case ('j'): save_state.alter_rupees(1); break;
			case ('k'): save_state.alter_rupees(10); break;
			case ('l'): save_state.alter_rupees(100); break;


			case ('b'): save_state.alter_hearts (-0.25f); break;
			case ('n'): save_state.alter_hearts (0.25f); break;
			
			case (','): save_state.alter_mp (-5); break;
			case ('.'): save_state.alter_mp (5); break;

			case ('e') : cast_Dins_Fire(); break;
			case ('w') : cast_Nayrus_Love(); break;
			case ('q') : cast_Farores_Wind(); break;

			case ('a'): save_state.gain_heart_container (16); break;
			case ('s'): save_state.lose_heart_container (16); break;
		};

		if (key>='5' && key<='9')
			save_state.tweak_easter_egg (key-'5');
		if (key=='0')
			save_state.tweak_easter_egg (5);
	};
};

void cast_Dins_Fire()
{
	if (save_state.get_mp() >= 40.0f)
	{
		save_state.alter_mp (-40.0f);
		save_state.set_DF (1);
	};
};

void cast_Nayrus_Love()
{
	if (save_state.get_mp() >= 40.0f)
	{
		save_state.alter_mp (-40.0f);
		save_state.set_NL (1200);
	};
};

void cast_Farores_Wind()
{
	int cost_to_set = 20;
	int cost_to_warp = 50;

	if (FW_x == -1)
	{
		if (save_state.get_mp() < cost_to_set)
			return;
		FW_x = Ganondorf.get_screen_x();
		FW_y = Ganondorf.get_screen_y();
		save_state.alter_mp (-cost_to_set);
		save_state.set_FW (1);
	}
	else
	{
		if (save_state.get_mp() < cost_to_warp)
			return;
		Ganondorf.set_position (FW_x, FW_y);
		save_state.alter_mp (-50);
		FW_x = -1;
		FW_y = -1;
		Ganondorf.select_frame (0, 0);
		save_state.set_FW (0);
	};
};

void halt()
{
	getchar();
};

#endif /* defined(__Main__) */