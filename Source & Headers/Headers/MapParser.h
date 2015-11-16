#ifndef __MapParser__
#define __MapParser__

#include "MapNode.h"
#include "StillSprite.h"
#include "Door.h"

class MapParser
{
	private:

		Sprite* list_head;
		Sprite* map[7]; /*[6] = G's Castle*/
		Sprite* working_map;
		Sprite* last_added_tile;
		bool keep_last_added_tile;
		int current_layer;
		StillSprite buffer_sprite;
		

	public:
	
		MapParser (Sprite* head, Sprite* forest, Sprite* fire, Sprite* water, Sprite* spirit, Sprite* shadow, Sprite* light, Sprite* home);

		void parse();

		void Tile		(int tx, int ty, int x, int y);
		void Rect		(int tx, int ty, int x, int y, int w, int h);
		void Row		(int tx, int ty, int x, int y, int w);
		void Col		(int tx, int ty, int x, int y, int h);
		void As_is		(int tx, int ty, int x, int y, int w, int h);
		void Stretch3	(int tx, int ty, int x, int y, int w, int h);
		void Stretch4	(int tx, int ty, int x, int y, int w, int h);
		void set_working_map (int n);
		void Assign_file (int n, char* c);
		void Stop (int x, int y, int w, int h);
		void Layer (int l);

		int p_atoi (char* c);

		Sprite* get_working_map();
};

#endif /* defined(__MapParser__) */

