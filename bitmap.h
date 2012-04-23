#ifndef BITMAP_H
#define BITMAP_H

#include <allegro.h>

struct struct_bitmaps {
	BITMAP *turret_base;
	BITMAP *turret_level_1;
	BITMAP *turret_level_2;
	BITMAP *turret_level_3;
	BITMAP *turret_bitmap_gun;
	BITMAP *turret_bitmap_slow;
	BITMAP *turret_bitmap_laser;
	BITMAP *turret_bitmap_storm;
	
	BITMAP *monster_drone;
	BITMAP *monster_floater;
	BITMAP *monster_tank;
	BITMAP *monster_crawler;
	
	BITMAP *map_background;
	
	BITMAP *screen_debug;
	BITMAP *part_info;
	BITMAP *part_turrets;
};


#endif
