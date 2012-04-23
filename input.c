#include <allegro.h>
#include <assert.h>
#include "common.h"

void input_do_input(game_data_t *gd)
{
	int map_x, map_y;
	
	map_x = mouse_x / 32;
	map_y = mouse_y / 32;
	if (map_x > 14) map_x = -1;
	if (map_y > 14) map_y = -1;
	
	gd->mouse_map_x = map_x;
	gd->mouse_map_y = map_y;
	gd->mouse_map_x_real = map_x * 32 + 16;
	gd->mouse_map_y_real = map_y * 32 + 16;	
	
	
	if (key[KEY_0])  gd->build_turret = TURRET_TYPE_NONE;
	if (key[KEY_1])  gd->build_turret = TURRET_TYPE_GUN;
	if (key[KEY_2])  gd->build_turret = TURRET_TYPE_SLOW;
	if (key[KEY_3])  gd->build_turret = TURRET_TYPE_LASER;
	if (key[KEY_4])  gd->build_turret = TURRET_TYPE_STORM;

	if (mouse_b & 2) {
		gd->build_turret = TURRET_TYPE_NONE;
		gd->selected_x = -1;
		gd->selected_y = -1;
	}


	if (mouse_b & 1) {
		if (tur_is_turret(map_x, map_y)) {
			gd->selected_x = map_x;
			gd->selected_y = map_y;
		}
		if (tile_is_turretslot(map_x, map_y) && !tur_is_turret(map_x, map_y) && gd->build_turret != TURRET_TYPE_NONE) {
			tur_add_turret(gd, gd->build_turret, map_x, map_y);
		}	
	}
				

	if (key[KEY_D]) {
		gd->toggle_show_debug++;
		if (gd->toggle_show_debug == 2)
			gd->toggle_show_debug = 0;
	}
	
	if (key[KEY_ESC]) clean_shutdown();
}
