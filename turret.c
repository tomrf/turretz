#include <allegro.h>
#include <assert.h>
#include <math.h>
#include "common.h"


int tur_is_turret(int x, int y)
{
	game_data_t *gd;
	turret_t *turret;
	
	gd = get_game_data_pointer();
	turret = gd->turret;
	
	if (turret == NULL)
		return 0;
		
	for (;;) {

		if (turret->x == x && turret->y == y) {
			return 1;
		}
		if (turret->next == NULL)
			break;
		turret = turret->next;
	}

	return 0;
}


int tur_cleanup_target(monster_t *monster)
{
	game_data_t *gd;
	turret_t *turret;
	
	gd = get_game_data_pointer();
	turret = gd->turret;
	
	if (turret == NULL || monster == NULL)
		return 0;
		
	for (;;) {

		if (turret->target_monster == monster) {
			turret->target_monster = NULL;
		}

		if (turret->next == NULL)
			break;
		turret = turret->next;
	}

	return 0;
}


int tur_add_turret(game_data_t *gd, int type, int x, int y)
{
	turret_t *turret;
	turret_t *new_turret;
	
	turret = gd->turret;
	
	if (x < 0 || y < 0)
		return -1;
	
	if (tur_is_turret(x, y))
		return -1;
		
		
	new_turret = malloc(sizeof (turret_t));
	assert (new_turret != NULL);

	new_turret->x = x;
	new_turret->y = y;
	new_turret->type = type;
	new_turret->angle = itofix(0);
	new_turret->desired_angle = itofix(-900);
	new_turret->target_monster = NULL;
	new_turret->level = 1;	
	new_turret->fcount = 0;
	new_turret->slow_fire_distance = 0;
	new_turret->next = NULL;

	if (type == TURRET_TYPE_GUN) {	
		new_turret->bitmap = gd->bitmaps->turret_bitmap_gun;
		new_turret->range = 120;
		new_turret->fire_rate = 5;
		new_turret->fire_reset_count = 100 / new_turret->fire_rate;
		new_turret->price = 100;
	}
	if (type == TURRET_TYPE_SLOW) {
		new_turret->bitmap = gd->bitmaps->turret_bitmap_slow;
		new_turret->range = 80;
		new_turret->fire_rate = 1;
		new_turret->fire_reset_count = 100 / new_turret->fire_rate;
		new_turret->price = 200;
	}
	if (type == TURRET_TYPE_LASER) {
		new_turret->bitmap = gd->bitmaps->turret_bitmap_laser;
		new_turret->range = 90;
		new_turret->fire_rate = 1;
		new_turret->fire_reset_count = 1;
		new_turret->laser_fire = 0;
		new_turret->price = 250;
	}
	if (type == TURRET_TYPE_STORM) {
		new_turret->bitmap = gd->bitmaps->turret_bitmap_storm;
		new_turret->range = 130;
		new_turret->fire_rate = 1;
		new_turret->fire_reset_count = 200;
		new_turret->laser_fire = 0;
		new_turret->price = 300;
	}
	
	assert(new_turret->bitmap != NULL);
	
	if (new_turret->price > gd->player_money) {
		free(new_turret);
		return -1;
	}
	
	
	

	/* first turret ? */
	if (gd->turret == NULL) {
		gd->turret = new_turret;
	}
	else {
    	/* search for last node */
		turret = gd->turret;
		for (;;) {
			if (turret->next == NULL) {
				turret->next = new_turret;
				break;
			}
			turret = turret->next;
		}
	}
		
	
	/* monies monies */
	gd->player_money -= new_turret->price;
	
	return 0;
}


int tur_target_within_sight(turret_t *turret, int target_x, int target_y)
{
	int x, y;
	int map_x, map_y;
	turret_t *tp;
	game_data_t *gd;
	fixed angle, dx, dy;
	float bx, by;
	int i;
	int px, py, pc;
	
	dx = itofix((turret->x * 32 + 16) - target_x);
    dy = itofix((turret->y * 32 + 16) - target_y);    
    angle = fatan2(dy, dx) - itofix(64);
	
	x = turret->x * 32;
	y = turret->y * 32;
		
 	bx = x + turret->bitmap->w/2;
	by = y + turret->bitmap->h/2;

	gd = get_game_data_pointer();

	for (i = 0; i < turret->range / 15; i++) {
		float result;
		result = fixtof(fixcos(angle));
		by -= result * 15;
		result = fixtof(fixsin(angle));
		bx += result * 15;
		
		map_x = (int) bx / 32;
		map_y = (int) by / 32;
		
		if (map_x == target_x/32 && map_y == target_y/32)
			return 1;

		//putpixel(screen, bx, by, makecol(255,255,255));		
		
		tp = gd->turret;
		for (;;) {
			if (tp == NULL)
				break;
			if (tp != turret && (tp->x == map_x && tp->y == map_y)) {
				/* found a turret in this tile, check for pixel value */
				px = (int)bx - (map_x * 32);
				py = (int)by - (map_y * 32);
				pc = getpixel(tp->bitmap, px, py);
				if (pc != makecol(255, 0, 255)) {
					return 0;
				}
				
			}
			tp = tp->next;
		}				

	}	
	
	
	return 1;
}


int tur_target_within_range(turret_t *turret, int target_x, int target_y)
{
	
	if (tur_get_distance(turret, target_x, target_y) > turret->range)
		return 0;

	return 1;
}


int get_distance(int x, int y, int x2, int y2)
{
	return (int) sqrt((double) (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
}


int tur_get_distance(turret_t *turret, int target_x, int target_y)
{
	int x, y, distance;
	
	x = turret->x * 32 + 16;
	y = turret->y * 32 + 16;
		
	distance = (int) sqrt((double) (target_x - x) * (target_x - x) + (target_y - y) * (target_y - y));
	
	return distance;
}




void bezier_test(float t, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
	float cx = 3 * (x1 - x0);
	float bx = 3 * (x2 - x1) - cx;
	float ax = x3 - x0 - cx - bx;
	float cy = 3 * (y1 - y0);
	float by = 3 * (y2 - y1) - cy;
	float ay = y3 - y0 - cy - by;
	float tCubed = t * t * t;
	float tSquared = t * t;
	float resultX = (ax * tCubed) + (bx * tSquared) + (cx * t) + x0;
	float resultY = (ay * tCubed) + (by * tSquared) + (cy * t) + y0;

	putpixel(screen, (int)resultX, (int)resultY, makecol(255,0,0));

}



void tur_draw_turret(BITMAP *dest, turret_t *turret)
{
	int x, y;
	float bx, by;
	int i;
	
	x = turret->x * 32;
	y = turret->y * 32;
		
	/* XXX/DEBUG: SHOW ANGLE */
 	bx = x + turret->bitmap->w/2;
	by = y + turret->bitmap->h/2;
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(0,0,0,100);
	for (i = 0; i < turret->range; i += 10) {
		float result;
		result = fixtof(fixcos(turret->angle));
		by -= result * 10;
		result = fixtof(fixsin(turret->angle));
		bx += result * 10;			
		//putpixel(dest, bx, by, makecol(255,255,0));
		//putpixel(dest, bx+1, by, makecol(255,255,0));
	}
	solid_mode();
	/* ---- */

	/* range */
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(0,0,0,60);
	//circle(dest, turret->x * 32 + 16, turret->y * 32 + 16, turret->range, makecol(200,200,200));
	/*circlefill(dest, turret->x * 32 + 16, turret->y * 32 + 16, turret->range, makecol(200,200,200));*/
	solid_mode();


	/* turret */
	rotate_sprite(dest, turret->bitmap, turret->x * 32, turret->y * 32, turret->angle);
	
}



static inline void tur_rotate(turret_t *turret) {
	fixed dx, dy;
	int i;
	int dir_n, dir_p;
	int x, y;
	int rotate_direction;
	
	x = turret->x * 32;
	y = turret->y * 32;
	rotate_direction = 0;

	/* rotate: get desired angle from target */
	if (turret->target_monster != NULL) {
		dx = itofix((x + 16) - (int)(turret->target_monster->x + 16));
		dy = itofix((y + 16) - (int)(turret->target_monster->y + 16));    
		turret->desired_angle = fatan2(dy, dx) - itofix(64);
	}
	else
		turret->desired_angle = itofix(-420);
	
	if (turret->target_monster != NULL) {    		

		if (fixtoi(turret->angle) != fixtoi(turret->desired_angle) && tur_target_within_range(turret,(int)turret->target_monster->x + 16,(int)turret->target_monster->y + 16)) {

			dir_n = 0;
			dir_p = 0;

			/* calculate travel distances */
			i = fixtoi(turret->angle);
			for (;;) {
				i--; dir_n++;
				if (i < -192) i = 64;
				if (i > 64) i = -192;
				if (i == fixtoi(turret->desired_angle))
					break;
			}
			i = fixtoi(turret->angle);
			for (;;) {
				i++; dir_p++;
				if (dir_p > dir_n) 
					break;
				if (i < -192) i = 64;
				if (i > 64) i = -192;
				if (i == fixtoi(turret->desired_angle))
					break;
			}

			/* rotate */
			if (dir_n < dir_p) rotate_direction = 1;
			else rotate_direction = 2;
			if (rotate_direction == 1) turret->angle = itofix(fixtoi(turret->angle) - 1);
			if (rotate_direction == 2) turret->angle = itofix(fixtoi(turret->angle) + 1);
			if (fixtoi(turret->angle) > 64) turret->angle = itofix(-192);
			if (fixtoi(turret->angle) < -192) turret->angle = itofix(64);

		}
	
	} /* rotate */
	
}



static inline void tur_target(turret_t *turret)
{
	game_data_t *gd;
	monster_t *monster;
	monster_t *target_monster;
	int target_distance;
	
	gd = get_game_data_pointer();
		
	monster = gd->monster;
	target_monster = NULL;

	for (;;) {
		if (monster == NULL)
			break;
		target_distance = tur_get_distance(turret, (int)monster->x + 16, (int)monster->y + 16);
		if (target_distance <= turret->range && tur_target_within_sight(turret, (int)monster->x + 16, (int)monster->y + 16) == 1) {
			if (target_monster == NULL)
				target_monster = monster;
			if ((int)target_monster->pixels_traveled < (int)monster->pixels_traveled)
				target_monster = monster;
			turret->target_monster = target_monster;
								
		}
		monster = monster->next;
	}
}



static inline void tur_do_gun(game_data_t *gd, turret_t *turret)
{
	
	/* shoot something? */
	if (turret->target_monster != NULL) {
		if ((fixtoi(turret->angle) == fixtoi(turret->desired_angle)) && tur_target_within_range(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16)) {
			if (tur_target_within_sight(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16) == 1) {
				if (turret->fire_reset_count == 0) {
					turret->fire_reset_count = 100 / turret->fire_rate;
					bullet_add_bullet(turret, turret->target_monster, tur_get_distance(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16));	
					
					play_sample(gd->samples->gun_gun, 180, 128, 1000, 0);
					
				}
			}
		}
	}
	
	if (turret->fire_reset_count > 0)
		turret->fire_reset_count--;
		
	tur_rotate(turret);
	tur_target(turret);
				
}


static inline void tur_do_laser(game_data_t *gd, turret_t *turret)
{
	
	/* shoot something? */
	turret->laser_fire = 0;
	if (turret->target_monster != NULL) {

		if ((fixtoi(turret->angle) == fixtoi(turret->desired_angle)) && tur_target_within_range(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16)) {
			if (tur_target_within_sight(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16) == 1) {
				if (turret->fire_reset_count == 0) {
					turret->fire_reset_count = 1;
					turret->laser_fire = 1;
					
					stop_sample(gd->samples->gun_laser);
					play_sample(gd->samples->gun_laser, 255, 128, 1000, 0);
					
					/* register damage */
					monster_register_hit(turret->target_monster, 0.5);
				}
			}
		}

	}
	
	if (turret->fire_reset_count > 0)
		turret->fire_reset_count--;
		

	tur_rotate(turret);
	tur_target(turret);

}


static inline void tur_do_storm(game_data_t *gd, turret_t *turret)
{
	monster_t *monster;
	monster_t *area_monster[100];
	int area_i = 0;
	
	/* shoot something? */
	turret->laser_fire--;
	if (turret->laser_fire < 0) turret->laser_fire = 0;
	if (turret->target_monster == NULL) turret->laser_fire = 0;
	if (turret->target_monster != NULL) {
		if ((fixtoi(turret->angle) == fixtoi(turret->desired_angle)) && tur_target_within_range(turret, (int)turret->target_monster->x + 16, (int)turret->target_monster->y + 16)) {
			if (turret->fire_reset_count == 0 && turret->laser_fire == 0) {

				turret->laser_fire = 20;
				turret->fire_reset_count = 200 / turret->fire_rate;

				play_sample(gd->samples->gun_storm, 150, 128, 1000, 0);

			}
			
			if (turret->laser_fire > 0) {
	
				/* do areal damage ! */
				monster = gd->monster;
				for (;;) {
					if (monster == NULL)
						break;
					if (monster == turret->target_monster || get_distance(turret->target_monster->x, turret->target_monster->y, monster->x, monster->y) < 35) {
						area_monster[area_i++] = monster;
						if (area_i >= 99)
							break;
					}
					monster = monster->next;				
				} /* for */
				
				for (;;) {
					area_i--;
					if (area_i < 0)
						break;
					monster_register_hit(area_monster[area_i], 2);
				}
			
			} /* laser_fire > 0 */
			
		}
	} /* shoot */
	
	if (turret->fire_reset_count > 0)
		turret->fire_reset_count--;
		
		
	tur_rotate(turret);
	tur_target(turret);

}



static inline void tur_do_slow(game_data_t *gd, turret_t *turret)
{	
	monster_t *monster;
	int target_distance;
	
	turret->target_monster = NULL;
	tur_target(turret);
	
	/* fire? */
	if (turret->fire_reset_count > 0 && turret->slow_fire_distance == 0)
		turret->fire_reset_count--;

	if (turret->fire_reset_count == 0 && turret->target_monster == NULL)
		turret->fire_reset_count = 1;
	
	if (turret->slow_fire_distance > 0) {
		turret->slow_fire_distance++;
		if (turret->slow_fire_distance > turret->range) {			
			turret->slow_fire_distance = 0;
			turret->fire_reset_count = 100 / turret->fire_rate;								
		}

	}

	if (turret->fire_reset_count == 0 && turret->slow_fire_distance == 0 && turret->target_monster != NULL) {
		turret->slow_fire_distance = 1;
		play_sample(gd->samples->gun_slow, 250, 128, 1000, 0);
	}


	
	/* hit 'em ! */
	if (turret->fire_reset_count == 0 && turret->slow_fire_distance > 0) {
	
		monster = gd->monster;

		for (;;) {
			if (monster == NULL)
				break;
			target_distance = tur_get_distance(turret, (int)monster->x + 16, (int)monster->y + 16);
			
			if (target_distance == turret->slow_fire_distance) {
				monster->slowness = 230;						
			}
			
			monster = monster->next;
		}
					
	} /* hit */
	
	
	/* rotate */
	turret->fcount += 0.3;
	if (turret->fcount > 64)
		turret->fcount = -192.0;
	turret->angle = itofix((int)turret->fcount);
	
}



void tur_logic_do_turrets(game_data_t *gd)
{
	turret_t *turret;

	
	turret = gd->turret;
	if (turret == NULL)
		return;
		

	for (;;) {
		if (turret != NULL) {

			if (turret->type == TURRET_TYPE_GUN)
				tur_do_gun(gd, turret);
			else if (turret->type == TURRET_TYPE_SLOW)
				tur_do_slow(gd, turret);
			else if (turret->type == TURRET_TYPE_LASER)
				tur_do_laser(gd, turret);
			else if (turret->type == TURRET_TYPE_STORM)
				tur_do_storm(gd, turret);



		} /* turret */
		else
			break;
		turret = turret->next;
	}
	
}



