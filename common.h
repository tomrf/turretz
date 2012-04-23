#ifndef COMMON_H
#define COMMON_H

#include <allegro.h>

#include "bitmap.h"
#include "monster.h"
#include "turret.h"
#include "decal.h"
#include "bullet.h"
#include "candy.h"
#include "sound.h"


#define INTEREST_RATE (0.001 * game_data->player_money) / (100 * game_data->player_lives)


typedef struct struct_game_data {
	
	turret_t *turret;
	monster_t *monster;
	decal_t *decal;
	bullet_t *bullet;
	candy_t *candy;
		
	struct struct_bitmaps *bitmaps;
	struct struct_samples *samples;
	
	int mouse_map_x;
	int mouse_map_y;
	int mouse_map_x_real;
	int mouse_map_y_real;
	
	float	player_money;
	int		player_lives;
	
	int     build_turret;
	int		selected_x;
	int     selected_y;
	
	BITMAP *screen_buffer;
	
	int toggle_show_debug;
	
} game_data_t;

#include "proto.h"

#endif
