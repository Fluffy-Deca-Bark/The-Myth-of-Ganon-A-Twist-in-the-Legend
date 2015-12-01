#include "MapParser.h"
#include "Defines.h"
#include <stdlib.h>

MapParser::MapParser (Sprite* head, Sprite* forest, Sprite* fire, Sprite* water, Sprite* spirit, Sprite* shadow, Sprite* light, Sprite* home)
{
	list_head = head;
	map[0] = forest;
	map[1] = fire;
	map[2] = water;
	map[3] = spirit;
	map[4] = shadow;
	map[5] = light;
	map[6] = home;

	buffer_sprite.set_frame_w (TILE_SIZE);
	buffer_sprite.set_frame_h (TILE_SIZE);
	last_added_tile = NULL;
	current_layer = 0;
	keep_last_added_tile = false;
};

void MapParser::parse()
{
	FILE * pFile;
	char line_buffer[100];			// 100: arbitr�rio
	char argument_buffer[9][20];	// 9: m�ximo de args + fun��o; 20: arbitr�rio
	int size = 100;
	int arguments = 0;
	int current_argument = 0;
	char map_file_path[FILE_PATH_SIZE];
	cat_path (map_file_path, SH, "Files\\MapFileBG.txt");
	fopen_s (&pFile, map_file_path , "r");
	
	if (pFile == NULL) printf("Error opening background map file\n");
	else 
	{
		keep_last_added_tile = false;
		while (fgets(line_buffer, size, pFile) != NULL)
		{
			int arg_i, line_i;
			for (arg_i=0, line_i=0; line_buffer[line_i]!='('; arg_i++, line_i++)
			{
				if (line_buffer[line_i] != ' ' && line_buffer[line_i] != '\t')
					argument_buffer[0][arg_i] = line_buffer[line_i];
				else
					argument_buffer[0][arg_i] = '\0';
			};

			argument_buffer[0][arg_i] = '\0';
			line_i++;
			
			if (strcmp(argument_buffer[0], "Select")==0 || strcmp(argument_buffer[0], "Layer")==0)
				arguments = 1;
			if (strcmp(argument_buffer[0], "Assign_file") == 0)
				arguments = 2;
			else if (strcmp(argument_buffer[0], "Tile")==0 || strcmp(argument_buffer[0], "Stop")==0)
				arguments = 4;
			else if (strcmp(argument_buffer[0], "Rect")==0 || strcmp(argument_buffer[0], "As_is")==0 || strcmp(argument_buffer[0],"Stretch3")==0 || strcmp(argument_buffer[0], "Stretch4") == 0)
				arguments = 6;
			else if (strcmp(argument_buffer[0], "Row")==0 || strcmp(argument_buffer[0], "Col")==0)
				arguments = 5;


			for (current_argument = 1; current_argument < 9; current_argument++)
			{
				for (int i = 0; i<20; i++)
					argument_buffer[current_argument][i] = '\0'; //Cleaning
			};

				
			for (current_argument = 1; current_argument <= arguments; current_argument++)
			{
				for (arg_i=0; line_buffer[line_i]!=',' && line_buffer[line_i]!=')'; line_i++)
				{
					if (line_buffer[line_i] != ' ')
					{
						argument_buffer[current_argument][arg_i] = line_buffer[line_i];
						arg_i++;
					};
				};
				argument_buffer[current_argument][arg_i] = '\0';
				line_i++;
			};
		
			if(0)
			{
				printf("N = %d\n", arguments);
				for (int print_i = 0; print_i <= arguments; print_i++)
					printf("Arg %d = [%s]\n", print_i, argument_buffer[print_i]);
				printf("\n");
			};

			if (strcmp(argument_buffer[0], "Tile")==0)
				Tile(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]));
			else if (strcmp(argument_buffer[0], "Rect")==0)
				Rect(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]), p_atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Row")==0)
				Row(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]));
			else if (strcmp(argument_buffer[0], "Col")==0)
				Col(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]));
			else if (strcmp(argument_buffer[0], "As_is")==0)
				As_is(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]), p_atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Stretch3")==0)
				Stretch3(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]), p_atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Stretch4")==0)
				Stretch4(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]), p_atoi(argument_buffer[5]), p_atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Assign_file")==0)
				Assign_file(p_atoi(argument_buffer[1]), argument_buffer[2]);
			else if (strcmp(argument_buffer[0], "Select")==0)
				set_working_map (p_atoi(argument_buffer[1]));
			else if (strcmp(argument_buffer[0], "Stop")==0)
				Stop(p_atoi(argument_buffer[1]), p_atoi(argument_buffer[2]), p_atoi(argument_buffer[3]), p_atoi(argument_buffer[4]));
			else if (strcmp(argument_buffer[0], "Layer")==0)
				Layer (p_atoi(argument_buffer[1]));
		};
		

		for (int i=0; i<size; i++)
			line_buffer[i]=0;

		fclose (pFile);
	};
};

void MapParser::Tile (int tx, int ty, int x, int y)
{
	Sprite* p;
	buffer_sprite.select_frame (tx, ty);
	buffer_sprite.set_position (x, y);
	buffer_sprite.set_layer (current_layer);
	buffer_sprite.set_to_delete (false);
	buffer_sprite.set_generated (true);

	buffer_sprite.copy_base_data (working_map);
	p = list_head->insert_node (working_map);
	
	if (!keep_last_added_tile)
		last_added_tile = p;

	keep_last_added_tile = true;
};

void MapParser::Rect (int tx, int ty, int x, int y, int w, int h)
{
	//printf("Rect function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

	keep_last_added_tile = false;

	for (int i=0; i<h; i++)
	{
		Row (tx, ty, x, y+(TILE_SIZE*i), w);
	};
};

void MapParser::Row (int tx, int ty, int x, int y, int w)
{
	//printf("Row  function called with arguments %d, %d, %d, %d and %d.\n", tx, ty, x, y, w);

	keep_last_added_tile = false;

	for (int i=0; i<w; i++)
	{
		Tile (tx, ty, x + i*TILE_SIZE, y);
	};
};

void MapParser::Col (int tx, int ty, int x, int y, int h)
{
	//printf("Col  function called with arguments %d, %d, %d, %d and %d.\n", tx, ty, x, y, h);

	keep_last_added_tile = false;

	for (int i=0; i<h; i++)
	{
		Tile (tx, ty, x, y + i*TILE_SIZE);
	};
};

void MapParser::As_is (int tx, int ty, int x, int y, int w, int h)
{
	//printf("\'As is\' function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

	keep_last_added_tile = false;

	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			Tile (tx + j, ty + i, x + j*TILE_SIZE, y + i*TILE_SIZE);
		};
	};
};

void MapParser::Stretch3 (int tx, int ty, int x, int y, int w, int h)
{
	//printf("\'St\' function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

	keep_last_added_tile = false;

	Tile (tx, ty, x, y);
	Row (tx+1, ty, x+TILE_SIZE, y, w-2);
	Tile (tx+2, ty, x+(w-2)*TILE_SIZE, y);
	Col (tx, ty+1, x, y+TILE_SIZE, h-2);
	Tile (tx, ty+2, x, y+(h-2)*TILE_SIZE);
	Row (tx+1, ty+2, x+TILE_SIZE, y+(h-2)*TILE_SIZE, w-3);
	Col (tx+2, ty+1, x+(w-2)*TILE_SIZE, y+TILE_SIZE, h-3);
	Tile (tx+2, ty+2, x+(w-2)*TILE_SIZE, y+(h-2)*TILE_SIZE);
	Rect (tx+1, ty+1, x+TILE_SIZE, y+TILE_SIZE, w-3, h-3);
};

void MapParser::Stretch4 (int tx, int ty, int x, int y, int w, int h)
{
	//printf("\'St\' function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

	//printf ("Stretch4 called, but no definition available right now.\n");


	keep_last_added_tile = false;

	Tile (tx, ty, x, y);
	Tile (tx, ty + 3, x, y + TILE_SIZE*h);
	Tile (tx + 3, ty, x + TILE_SIZE*w, y);
	Tile (tx + 3, ty + 3, x + TILE_SIZE*w, y + TILE_SIZE*h);

	for (int i = 0; i < w - 1; i++)
	{
		Tile (tx + (i % 2) + 1, ty, x + TILE_SIZE * (i + 1), y);
		Tile (tx + (i % 2) + 1, ty + 3, x + TILE_SIZE * (i + 1), y + h*TILE_SIZE);
	};

	for (int j = 0; j < h-1; j++)
	{
		Tile (tx, ty + (j % 2) + 1, x, y + TILE_SIZE * (j + 1));
		Tile (tx + 3, ty + (j % 2) + 1, x + TILE_SIZE * w, y + TILE_SIZE * (j + 1));
	}

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			Tile (tx + 1 + i % 2, ty + 1 + j % 2, x + TILE_SIZE*(i + 1), y + (j % 2) * TILE_SIZE*(j + 1));
		}
	}


	/*Row (tx + 1, ty, x + TILE_SIZE, y, w - 2);
	Tile (tx + 2, ty, x + (w - 2)*TILE_SIZE, y);
	Col (tx, ty + 1, x, y + TILE_SIZE, h - 2);
	Tile (tx, ty + 2, x, y + (h - 2)*TILE_SIZE);
	Row (tx + 1, ty + 2, x + TILE_SIZE, y + (h - 2)*TILE_SIZE, w - 3);
	Col (tx + 2, ty + 1, x + (w - 2)*TILE_SIZE, y + TILE_SIZE, h - 3);
	Tile (tx + 2, ty + 2, x + (w - 2)*TILE_SIZE, y + (h - 2)*TILE_SIZE);
	Rect (tx + 1, ty + 1, x + TILE_SIZE, y + TILE_SIZE, w - 3, h - 3);*/
};

void MapParser::set_working_map (int n)
{
	working_map = map[n];
};

void MapParser::Assign_file (int n, char* c)
{
	if (strcmp (map[n]->get_path(), "") == 0)
		map[n]->load (TR, c);
	//printf("MapParser::Assign_file(...) -- map[%d] = \"%s\" \n",n,c);
};

Sprite* MapParser::get_working_map()
{
	return working_map;
};

int MapParser::p_atoi (char* c)
{
	if (c[0] == '*')
	{
		return TILE_SIZE * atoi (c+1);
		/*char* buffer;
		int i;
		int number;

		for (i = 1; c[i] != '\0'; i++);

		buffer = new char[i];
		
		for (i = 1; c[i] != '\0'; i++)
		{
			buffer[i-1] = c[i];
		};

		number = atoi(buffer);*/
	}
	else
		return atoi (c);
};


void MapParser::Stop (int x, int y, int w, int h)
{
	last_added_tile->set_stop_box (x, y, w+x, h+y);
};


void MapParser::Layer (int l)
{
	current_layer = l;
};