#include <allegro.h>
#include <assert.h>
#include "common.h"



void monster_draw_monster(BITMAP *dest, monster_t *monster)
{
	float hp_line_len;
	int x, y;
	
	x = monster->x + monster->x_fudge;
	y = monster->y + monster->y_fudge;
	
	/* monster */
	
	rotate_sprite(dest, monster->bitmap, x,y, monster->angle);
	
	/* shield */
	if (monster->shield > 0) {
		drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
		set_trans_blender(0,0,0,20 + monster->shield);
		circlefill(dest, x + 16, y + 16, 13, makecol(0x5e,0xae,0xe1));
		set_trans_blender(0,0,0,30 + monster->shield);
		circle(dest, x + 16, y + 16, 13, makecol(0x5e,0xae,0xe1));
		solid_mode();
	}
	
	
	/* hp bar */
	hp_line_len = (float)24 / monster->hp_max * monster->hp;
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	set_trans_blender(0,0,0,150);
	line(dest, x + 4, y + 29, x + 28, y + 29, makecol(255,0,0));
	line(dest, x + 4, y + 29, x + 4 + (int) hp_line_len, y + 29, makecol(0,255,0));
	set_trans_blender(0,0,0,100);
	line(dest, x + 4, y + 30, x + 28, y + 30, makecol(255,0,0));
	line(dest, x + 4, y + 30, x + 4 + (int) hp_line_len, y + 30, makecol(0,255,0));
	
	
	/* shield bar */
	if (monster->shield > 0) {
		hp_line_len = (float)24 / monster->shield_max * monster->shield;
		set_trans_blender(0,0,0,90);
		line(dest, x + 4, y + 31, x + 28, y + 31, makecol(100,100,255));
		set_trans_blender(0,0,0,170);
		line(dest, x + 4, y + 31, x + 4 + (int) hp_line_len, y + 31, makecol(0,0,255));
		set_trans_blender(0,0,0,80);
		line(dest, x + 4, y + 32, x + 28, y + 32, makecol(100,100,255));
		set_trans_blender(0,0,0,110);
		line(dest, x + 4, y + 32, x + 4 + (int) hp_line_len, y + 32, makecol(0,0,255));
	}
	
	solid_mode();
	
}


int monster_add_monster(game_data_t *gd, int type, int x, int y)
{
	monster_t *monster;
	monster_t *new_monster;
	
	new_monster = malloc(sizeof (monster_t));
	assert (new_monster != NULL);

	new_monster->type = type;
	new_monster->x = x;
	new_monster->y = y;
	new_monster->x_fudge = -4 + rand()% 8;
	new_monster->y_fudge = -4 + rand()% 8;
	new_monster->target_x = -1;
	new_monster->target_y = -1;
	new_monster->pmx = -1;
	new_monster->pmy = -1;
	new_monster->pdir = -1;
	new_monster->pixels_traveled = 0;
	new_monster->direction = 30;
	new_monster->angle = itofix(0);
	new_monster->desired_angle = itofix(0);
	new_monster->turn = MONSTER_TURN_NONE;
	new_monster->next = NULL;
	new_monster->slowness = 0;
	//if (rand()%2 == 1)
	//	new_monster->shield_max = 100;
	//else
		new_monster->shield_max = 0;
	
	new_monster->shield = new_monster->shield_max;
		
	if (type == MONSTER_TYPE_DRONE)	{
		new_monster->bitmap = gd->bitmaps->monster_drone;
		new_monster->speed = 0.5;
		new_monster->hp_max = 100;
		new_monster->hp = new_monster->hp_max;
		new_monster->money = 5;
	}
	
	if (type == MONSTER_TYPE_FLOATER) {
		new_monster->bitmap = gd->bitmaps->monster_floater;
		new_monster->speed = 0.3;
		new_monster->hp_max = 240;
		new_monster->hp = new_monster->hp_max;
		new_monster->money = 20;
	}
	
	if (type == MONSTER_TYPE_TANK) {
		new_monster->bitmap = gd->bitmaps->monster_tank;
		new_monster->speed = 0.4;
		new_monster->hp_max = 370;
		new_monster->hp = new_monster->hp_max;
		new_monster->money = 35;
	}	
	
	if (type == MONSTER_TYPE_CRAWLER) {
		new_monster->bitmap = gd->bitmaps->monster_crawler;
		new_monster->speed = 0.8;
		new_monster->hp_max = 120;
		new_monster->hp = new_monster->hp_max;
		new_monster->money = 7;
	}	
	
	
	if (gd->monster == NULL) {
		gd->monster = new_monster;
		return 0;
	}
	
	monster = gd->monster;
	for (;;)  {
				
		if (monster->next == NULL) {
			monster->next = new_monster;
			break;
		}
		
		monster = monster->next;
	}
	
	return 0;
}



void monster_path_finder(monster_t *monster)
{
	int mx, my;
	int nmx, nmy;
	int tx, ty;
	int dir = 0;
	int ttl;
		
	tx = 14;
	ty = 3;
	
	mx = monster->x / 32;
	my = monster->y / 32;

	for (;;) {

		ttl = 5;

		for (;;) {

			ttl--;
			if (ttl <= 0) {
				LOG("path finder timeout!");
				return;
			}

			nmx = mx;
			nmy = my;
		
			dir++;
			if (dir >= 4) dir = 0;
		
			if (dir == 0 && tile_is_path(mx, my - 1))
				nmy--;	
			if (dir == 1 && tile_is_path(mx, my + 1))
				nmy++;	
			if (dir == 2 && tile_is_path(mx - 1, my))
				nmx--;
			if (dir == 3 && tile_is_path(mx + 1, my))
				nmx++;	
			if (nmx == monster->pmx && nmy == monster->pmy)
				continue;	
			if (nmx == mx && nmy == my)
				continue;

			break;
		
		}
		
		monster->pmx = mx;
		monster->pmy = my;			
		mx = nmx;
		my = nmy;
		
		monster->target_x = mx * 32;
		monster->target_y = my * 32;
		
		/* turn? */
		
		if (dir == 0) monster->desired_angle = itofix(0);
		if (dir == 1) monster->desired_angle = itofix(-128);
		if (dir == 2) monster->desired_angle = itofix(-64);
		if (dir == 3) monster->desired_angle = itofix(64);
		
		if (monster->pdir == 0 && dir == 2)  monster->turn = MONSTER_TURN_LEFT;
		if (monster->pdir == 0 && dir == 3)  monster->turn = MONSTER_TURN_RIGHT;
		if (monster->pdir == 1 && dir == 2)  monster->turn = MONSTER_TURN_RIGHT;
		if (monster->pdir == 1 && dir == 3)  monster->turn = MONSTER_TURN_LEFT;		
		if (monster->pdir == 2 && dir == 0)  monster->turn = MONSTER_TURN_RIGHT;
		if (monster->pdir == 2 && dir == 1)  monster->turn = MONSTER_TURN_LEFT;
		if (monster->pdir == 3 && dir == 0)  monster->turn = MONSTER_TURN_LEFT;
		if (monster->pdir == 3 && dir == 1)  monster->turn = MONSTER_TURN_RIGHT;
		

		monster->pdir = dir;		
		
				
		break;
			
	}	
	
	
}


void monster_logic_do_monsters(game_data_t *gd)
{
	monster_t *monster;
	monster_t *delete_monster;
	float speed;
	
	monster = gd->monster;
	if (monster == NULL)
		return;

	for (;;) {
		
		delete_monster = NULL;
		
		if (monster != NULL) {

			speed = monster->speed;
			if (monster->slowness > 0) {
				monster->slowness--;
				speed = speed / 3;
			}
			
			/* fudgie fudgie !! */
			
			if (rand() % 2 == 1)
				monster->x_fudge += 0.01;	
			else
				monster->x_fudge -= 0.01;
			if (rand() % 2 == 1)
				monster->y_fudge += 0.01;	
			else
				monster->y_fudge -= 0.01;
				
			
			if ((int)monster->x_fudge > 4) monster->x_fudge = 4; 
			if ((int)monster->x_fudge < -4) monster->x_fudge = -4;
			if ((int)monster->y_fudge > 4) monster->y_fudge = 4; 
			if ((int)monster->y_fudge < -4) monster->y_fudge = -4;
				
			/* path findah! */
	 		if ((int)monster->target_x == -1 || (int)monster->target_y == -1)
				monster_path_finder(monster);

			/* move */
			if ((int)monster->target_x > (int)monster->x) {
				monster->x += speed;
				monster->pixels_traveled += speed;
			}
			else if ((int)monster->target_x < (int)monster->x) {
				monster->x -= speed;
				monster->pixels_traveled += speed;
			}
			else if ((int)monster->target_y > (int)monster->y) {
				monster->y += speed;
				monster->pixels_traveled += speed;
			}
			else if ((int)monster->target_y < (int)monster->y) {
				monster->y -= speed;
				monster->pixels_traveled += speed;
			}
			else {
				monster->target_x = -1;
				monster->target_y = -1;
			}
			
			
			if (monster->type == MONSTER_TYPE_DRONE) {
				/* angle */
				monster->angle = itofix(fixtoi(monster->angle) + 1);
				if (fixtoi(monster->angle) > 255) monster->angle = itofix(0);
			}
			
			
			if (monster->type == MONSTER_TYPE_FLOATER || monster->type == MONSTER_TYPE_TANK || monster->type == MONSTER_TYPE_CRAWLER) {
				int i;
				
				int a  = fixtoi(monster->angle);
				int da = fixtoi(monster->desired_angle);
				
				int turn_speed = 2;

				if (a != da && monster->turn == MONSTER_TURN_NONE)
					a = da;
					
				if (monster->type == MONSTER_TYPE_CRAWLER)
					turn_speed += 2;
			
				for (i = 0; i < turn_speed; i++) {
					if (a == da) {
						monster->turn = MONSTER_TURN_NONE;
						break;
					}
					if (monster->turn == MONSTER_TURN_LEFT)   a--;
					if (monster->turn == MONSTER_TURN_RIGHT)  a++;	
					if (a < -192) a = 64;
					if (a > 64)   a = -192;
				}
					
				monster->angle = itofix(a);
				
			}
				
			
			/* remove */
			if ((int)monster->x / 32 == 14 && (int)monster->y / 32 == 3) {
				delete_monster = monster;
				gd->player_lives--;
				if (gd->player_lives == 0)
					DIE("THE END");
			}
    
		}
		else
			break;
						
		monster = monster->next;
		
		if (delete_monster != NULL)
			monster_remove_monster(delete_monster);
		
	}
	

}


void monster_remove_monster(monster_t *monster)
{
	monster_t *b;
	game_data_t *gd;

	gd = get_game_data_pointer();
	
	b = gd->monster;
	if (b == NULL)
		return;

	if (b == monster) {
		if (monster->next != NULL)
			gd->monster = monster->next;
		else
			gd->monster = NULL;
		free(monster);
		return;
	}
	
	for (;;) {
		if (b->next == monster) {
			if (monster->next != NULL) 
				b->next = monster->next;
			else b->next = NULL;
			free(monster);
			return;
		}
		if (b->next == NULL)
			break;
		b = b->next;		
	}
}



void monster_register_hit(monster_t *monster, float damage)
{
	game_data_t *gd;
		
	if (monster == NULL)
		return;
		
	if (monster->shield < 0) monster->shield = 0;
		
	if (monster->shield > 0) {
		monster->shield -= damage;
	}
	else {	
		monster->hp -= damage;
		if (monster->hp <= 0) {
			gd = get_game_data_pointer();
			
			candy_add_vis_hit(gd, monster->x + monster->x_fudge + 16, monster->y + monster->y_fudge + 16, monster->money);
			gd->player_money += monster->money;
			
			tur_cleanup_target(monster);
			bullet_cleanup_target(monster);
			monster_remove_monster(monster);
			
		}
	}		

}

