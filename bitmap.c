#include <allegro.h>
#include <assert.h>
#include <math.h>
#include "common.h"


inline BITMAP* _bitmap_load_bitmap(const char *filename)
{
	BITMAP *bitmap;
	
	bitmap = load_bitmap(filename, NULL);
	assert(bitmap != NULL);
	
	return bitmap;
}


void bitmap_load_bitmaps(struct struct_bitmaps *bitmaps)
{
	bitmaps->turret_base               = _bitmap_load_bitmap("gfx/turret_base.bmp");
	bitmaps->turret_level_1            = _bitmap_load_bitmap("gfx/turret_level_1.bmp");
	bitmaps->turret_level_2            = _bitmap_load_bitmap("gfx/turret_level_2.bmp");
	bitmaps->turret_level_3            = _bitmap_load_bitmap("gfx/turret_level_3.bmp");
	bitmaps->turret_bitmap_gun         = _bitmap_load_bitmap("gfx/turret_bitmap_gun.bmp");
	bitmaps->turret_bitmap_slow        = _bitmap_load_bitmap("gfx/turret_bitmap_slow.bmp");
	bitmaps->turret_bitmap_laser       = _bitmap_load_bitmap("gfx/turret_bitmap_laser.bmp");
	bitmaps->turret_bitmap_storm       = _bitmap_load_bitmap("gfx/turret_bitmap_storm.bmp");
	bitmaps->monster_drone			   = _bitmap_load_bitmap("gfx/monster_drone.bmp");
	bitmaps->monster_floater           = _bitmap_load_bitmap("gfx/monster_floater.bmp");
	bitmaps->monster_tank              = _bitmap_load_bitmap("gfx/monster_tank.bmp");
	bitmaps->monster_crawler	       = _bitmap_load_bitmap("gfx/monster_crawler.bmp");
	bitmaps->map_background			   = _bitmap_load_bitmap("gfx/bg_alt.bmp");
	
	bitmaps->screen_debug = create_bitmap(250,120);
	
	bitmaps->part_info    = create_bitmap(200, 65);
	bitmaps->part_turrets = create_bitmap(200, 300);
}
