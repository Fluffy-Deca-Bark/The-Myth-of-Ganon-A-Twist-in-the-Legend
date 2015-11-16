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
};

void MapParser::parse()
{
	FILE * pFile;
	char line_buffer[100];			// 100: arbitrário
	char argument_buffer[9][20];	// 9: máximo de args + função; 20: arbitrário
	int size = 100;
	int arguments = 0;
	int current_argument = 0;
	char map_file_path[FILE_PATH_SIZE];
	cat_path (map_file_path, SH, "Files\\MapFileBG.txt");
	fopen_s (&pFile, map_file_path , "r");
	
	if (pFile == NULL) printf("Error opening background map file\n");
	else 
	{
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
			
			if (strcmp(argument_buffer[0], "Select") == 0)
				arguments = 1;
			if (strcmp(argument_buffer[0], "Assign_file") == 0)
				arguments = 2;
			else if (strcmp(argument_buffer[0], "Tile")==0)
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
				Tile(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]));
			else if (strcmp(argument_buffer[0], "Rect")==0)
				Rect(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]), atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Row")==0)
				Row(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]));
			else if (strcmp(argument_buffer[0], "Col")==0)
				Col(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]));
			else if (strcmp(argument_buffer[0], "As_is")==0)
				As_is(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]), atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Stretch3")==0)
				Stretch3(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]), atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Stretch4")==0)
				Stretch4(atoi(argument_buffer[1]), atoi(argument_buffer[2]), atoi(argument_buffer[3]), atoi(argument_buffer[4]), atoi(argument_buffer[5]), atoi(argument_buffer[6]));
			else if (strcmp(argument_buffer[0], "Assign_file")==0)
				Assign_file(atoi(argument_buffer[1]), argument_buffer[2]);
			else if (strcmp(argument_buffer[0], "Select")==0)
				set_working_map (atoi(argument_buffer[1]));
		};
		
		fclose (pFile);
	};
};

void MapParser::Tile (int tx, int ty, int x, int y)
{
	buffer_sprite.select_frame (tx, ty);
	buffer_sprite.set_position (x, y);
	buffer_sprite.set_layer (3);
	buffer_sprite.copy_base_data (working_map);
	list_head->insert_node (working_map);
};

void MapParser::Rect (int tx, int ty, int x, int y, int w, int h)
{
	//printf("Rect function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

	for (int i=0; i<h; i++)
	{
		Row (tx, ty, x, y+(TILE_SIZE*i), w);
	};
};

void MapParser::Row (int tx, int ty, int x, int y, int w)
{
	//printf("Row  function called with arguments %d, %d, %d, %d and %d.\n", tx, ty, x, y, w);

	for (int i=0; i<w; i++)
	{
		Tile (tx, ty, x + i*TILE_SIZE, y);
	};
};

void MapParser::Col (int tx, int ty, int x, int y, int h)
{
	//printf("Col  function called with arguments %d, %d, %d, %d and %d.\n", tx, ty, x, y, h);

	for (int i=0; i<h; i++)
	{
		Tile (tx, ty, x, y + i*TILE_SIZE);
	};
};

void MapParser::As_is (int tx, int ty, int x, int y, int w, int h)
{
	//printf("\'As is\' function called with arguments %d, %d, %d, %d, %d and %d.\n", tx, ty, x, y, w, h);

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

	printf ("Stretch4 called, but no definition available right now.\n");
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