#include <allegro.h>
#include <assert.h>
#include <math.h>
#include "common.h"


static inline void thick_line(BITMAP *bmp, float x, float y, float x_, float y_, float thickness, int color)
{
        float dx = x - x_;
        float dy = y - y_;
        float d = sqrtf(dx * dx + dy * dy);
        if (!d)
        return;

        int v[4 * 2];

        /* left up */
        v[0] = x - thickness * dy / d;
        v[1] = y + thickness * dx / d;
        /* right up */
        v[2] = x + thickness * dy / d;
        v[3] = y - thickness * dx / d;
        /* right down */
        v[4] = x_ + thickness * dy / d;
        v[5] = y_ - thickness * dx / d;
        /* left down */
        v[6] = x_ - thickness * dy / d;
        v[7] = y_ + thickness * dx / d;

        polygon(bmp, 4, v, color);
}



void bullet_draw_laser(BITMAP *dest, turret_t *turret)
{
	int x, y;
	float bx, by;
	float bx_s, by_s;
	int dist;
	int mx, my;
	
	if (turret->target_monster == NULL)
		return;
	
	x = turret->x * 32;
	y = turret->y * 32;
	
	mx = turret->target_monster->x + turret->target_monster->x_fudge + 16;
	my = turret->target_monster->y + turret->target_monster->y_fudge + 16;
	dist = tur_get_distance(turret, mx, my);
				
 	bx = x + turret->bitmap->w/2;
	by = y + turret->bitmap->h/2;
	bx_s = bx;
	by_s = by;
	by_s -= fixtof(fixcos(turret->angle)) * 10;
	bx_s += fixtof(fixsin(turret->angle)) * 10;
	by -= fixtof(fixcos(turret->angle)) * dist;
	bx += fixtof(fixsin(turret->angle)) * dist;

	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	set_trans_blender(0,0,0,50 + rand()%40);
	thick_line(dest, bx_s, by_s, bx, by, 1, makecol(255,255,0));
	set_trans_blender(0,0,0,10 + rand()%30);
	line(dest, bx_s, by_s, bx, by, makecol(255,255,0));
	

	solid_mode();

}	
	

void bullet_draw_storm(BITMAP *dest, turret_t *turret)
{
	int x, y;
	float bx, by;
	float bx_s, by_s;
	int dist;
	int mx, my;
	int i;
		
	if (turret->target_monster == NULL)
		return;
	
	x = turret->x * 32;
	y = turret->y * 32;
	
	mx = turret->target_monster->x + turret->target_monster->x_fudge + 16;
	my = turret->target_monster->y + turret->target_monster->y_fudge + 16;
	dist = tur_get_distance(turret, mx, my);
	
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);	
	for (i = 0; i < 5; i++) {
		int z1_x, z1_y;
		int z2_x, z2_y;
		int color = rand()%3;
		
		if (color == 0) color = makecol(0xa1, 0xa3, 0xff);
		if (color == 1) color = makecol(240,240,240);
		if (color == 2) color = makecol(0xfd, 0xdc, 0xff);
		
		
		
	 	bx = x + turret->bitmap->w/2;
		by = y + turret->bitmap->h/2;
		bx_s = bx;
		by_s = by;
		by_s -= fixtof(fixcos(turret->angle)) * 15;
		bx_s += fixtof(fixsin(turret->angle)) * 15;
		by -= fixtof(fixcos(turret->angle)) * dist;
		bx += fixtof(fixsin(turret->angle)) * dist;

		z1_x = bx_s - 5 + rand() % 10;
		z1_y = by_s - 5 + rand() % 10;
		z2_x = bx - 32 + rand() % 64;
		z2_y = by - 32 + rand() % 64;
		by = by - 32 + rand() % 64;
		bx = bx - 32 + rand() % 64;
		
		set_trans_blender(0,0,0,40 + rand()%30);
		thick_line(dest, bx_s, by_s, z1_x, z1_y, 1, color);
		thick_line(dest, z1_x, z1_y, z2_x, z2_y, 1, color);
		thick_line(dest, z2_x, z2_y, bx, by, 1, color);
		set_trans_blender(0,0,0,40 + rand()%60);
		line(dest, bx_s, by_s, z1_x, z1_y, color);
		line(dest, z1_x, z1_y, z2_x, z2_y, color);
		line(dest, z2_x, z2_y, bx, by, color);
	}
	solid_mode();
	

}	



void bullet_draw_bullet(BITMAP *dest, bullet_t *bullet)
{
	int x, y;
	float bx, by, brx, bry;
	
	
	if (bullet->range < 0)
		return;
	
	x = bullet->x;
	y = bullet->y;
	
	bx = x;
	by = y;
	
	brx = fixtof(fixsin(bullet->angle));
	bx += brx * (float)bullet->range;

	bry = fixtof(fixcos(bullet->angle));
	by -= bry * (float)bullet->range;
	
	line(dest, bx, by, x+(brx*(bullet->range - 5)),y-(bry*(bullet->range - 5)), makecol(155,0,0));
	line(dest, bx+1, by, x+1+(brx*(bullet->range - 5)),y-(bry*(bullet->range - 5)), makecol(255,0,0));
	//line(dest, bx+2, by, x+1+(brx*(bullet->range - 5)),y-(bry*(bullet->range - 5)), makecol(205,0,0));
	//line(dest, bx-1, by, x+1+(brx*(bullet->range - 5)),y-(bry*(bullet->range - 5)), makecol(205,0,0));
	
	

	//putpixel(dest, (int)bx, (int)by, makecol(255,0,0));
	//circle(dest, (int)bx, (int)by, 10, makecol(rand()%255,rand()%255,rand()%255));
}


int bullet_add_bullet(turret_t *turret, monster_t *target_monster, int target_range)
{
	game_data_t *gd;
	bullet_t *bullet;
	bullet_t *new_bullet;
	
	gd = get_game_data_pointer();
	
	new_bullet = malloc(sizeof (bullet_t));
	assert (new_bullet != NULL);

	new_bullet->bitmap = NULL;
	new_bullet->x = turret->x * 32 + 16;
	new_bullet->y = turret->y * 32 + 16;
	new_bullet->angle = turret->angle;
	new_bullet->target_range = target_range;
	new_bullet->range = 16;
	new_bullet->damage = 5;
	new_bullet->target_monster = target_monster;
	new_bullet->next = NULL;
	

	/* first bullet ? */
	if (gd->bullet == NULL) {
		gd->bullet = new_bullet;
		return 0;
	}
	
	bullet = gd->bullet;
	for (;;)  {
				
		if (bullet->next == NULL) {
			bullet->next = new_bullet;
			break;
		}
		
		bullet = bullet->next;
	}
	
	return 0;
}


void bullet_remove_bullet(bullet_t *bullet)
{
	bullet_t *b;
	game_data_t *gd;
	
	gd = get_game_data_pointer();
	
	b = gd->bullet;
	assert (b != NULL);

	if (b == bullet) {
		if (bullet->next != NULL)
			gd->bullet = bullet->next;
		else
			gd->bullet = NULL;
		free(bullet);
		return;
	}
	
	for (;;) {
		if (b->next == bullet) {
			if (bullet->next != NULL) 
				b->next = bullet->next;
			else b->next = NULL;
			free(bullet);
			return;
		}
		if (b->next == NULL)
			break;
		b = b->next;		
	}
}


void bullet_logic_do_bullets(game_data_t *gd)
{
	bullet_t *bullet;
	int c = 0;
	
	bullet = gd->bullet;
	if (bullet == NULL)
		return;

	for (;;) {
		if (bullet != NULL) {
			c++;

			/* still alive */
			if (bullet->target_range != -1) {

				/* hit? */
				if (bullet->target_range <= bullet->range) {
					bullet->target_range = -1;
					bullet->range = -1;
					bullet_register_hit(bullet);
					bullet_remove_bullet(bullet);
				}

				if (bullet->range != bullet->target_range)
					bullet->range += 6;
			}	

	
		}
		else
			break;
		bullet = bullet->next;
	}
	

}


void bullet_register_hit(bullet_t *bullet)
{
	monster_register_hit(bullet->target_monster, bullet->damage);
}


int bullet_cleanup_target(monster_t *monster)
{
	game_data_t *gd;
	bullet_t *bullet;
	
	gd = get_game_data_pointer();
	bullet = gd->bullet;
	
	if (bullet == NULL || monster == NULL)
		return 0;
		
	for (;;) {

		if (bullet->target_monster == monster) {
			bullet->target_monster = NULL;
		}

		if (bullet->next == NULL)
			break;
		bullet = bullet->next;
	}

	return 0;
}
