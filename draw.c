#include <allegro.h>
#include <assert.h>
#include "common.h"


void draw(game_data_t *gd, BITMAP *buffer)
{
	turret_t *turret;
	bullet_t *bullet;
	monster_t *monster;
	candy_t *candy;

	int i;
	int color;
	
	static float active_sector_alpha = 20;
	static int active_sector_alpha_dir = 1;
	
	static float turret_color_alpha = 20;
	static int turret_color_alpha_dir = 1;
	
	
	
	/* background */
	rectfill(buffer, 0, 0, buffer->w, buffer->h, makecol(0,0,0));
	draw_sprite(buffer, gd->bitmaps->map_background, 0, 0);
	
	
	/* grid*/
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	set_trans_blender(0,0,0,20);
	for (i = 0; i < 480+1; i += 32) {
		line(buffer, i, 0, i, buffer->h, makecol(255,255,255));
		line(buffer, 0, i , 480, i, makecol(255,255,255));
	}
	solid_mode();
	
	
	/* turrets */
	if (turret_color_alpha_dir == 1)
		turret_color_alpha += 0.7;
	else
		turret_color_alpha -= 0.4;
			
	if (turret_color_alpha > 100)
		turret_color_alpha_dir = 0;
	if (turret_color_alpha < 5)
		turret_color_alpha_dir = 1;
		
	turret = gd->turret;
	for (;;) {

		if (turret != NULL) {
			/* draw base and color */
			if (turret->level == 1)
				draw_sprite(buffer, gd->bitmaps->turret_level_1, turret->x * 32, turret->y * 32);
			else if (turret->level == 2)
				draw_sprite(buffer, gd->bitmaps->turret_level_2, turret->x * 32, turret->y * 32);
			else if (turret->level == 3)
				draw_sprite(buffer, gd->bitmaps->turret_level_3, turret->x * 32, turret->y * 32);
			
			drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
			set_trans_blender(0,0,0,turret_color_alpha);
			circlefill(buffer, turret->x * 32 + 16, turret->y * 32 + 16, 10, makecol(0,0,0));			
			solid_mode();
			draw_sprite(buffer, gd->bitmaps->turret_base, turret->x * 32, turret->y * 32);
			/* draw turret */
			tur_draw_turret(buffer, turret);
		}
		else
			break;

		turret = turret->next;
	}    
		
	
	/* monsters */
	monster = gd->monster;
	for (;;) {

		if (monster != NULL) {
				monster_draw_monster(buffer, monster);
		}
		else
			break;

		monster = monster->next;
	}    
	
	
 	/* bullets */
	bullet = gd->bullet;
	for (;;) {

		if (bullet != NULL) {
			bullet_draw_bullet(buffer, bullet);
		}
		else
			break;

		bullet = bullet->next;
	}
	
	
	/* slow fire */
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	turret = gd->turret;
	for (;;) {
		if (turret != NULL) {
			int a_a;
			int color = makecol(200,200,200);
			float per;
			per = (float)turret->slow_fire_distance / turret->range * 100;			
			a_a = 170 - per;
			if (a_a < 40) a_a = 40;

			if (turret->type == TURRET_TYPE_SLOW && turret->slow_fire_distance > 0) {
				set_trans_blender(0,0,0,a_a);
				circle(buffer, turret->x * 32 + 16, turret->y * 32 + 16, turret->slow_fire_distance, color);
				set_trans_blender(0,0,0,a_a - 20);
				circle(buffer, turret->x * 32 + 16, turret->y * 32 + 16, turret->slow_fire_distance - 4, color);				
				set_trans_blender(0,0,0,a_a - 35);
				circle(buffer, turret->x * 32 + 16, turret->y * 32 + 16, turret->slow_fire_distance - 8, color);

			}
		}
		else
			break;
		turret = turret->next;
	}    
	solid_mode();
		

	/* laser fire */
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	turret = gd->turret;
	for (;;) {
		if (turret != NULL) {
			if (turret->type == TURRET_TYPE_LASER && turret->laser_fire == 1 && turret->target_monster != NULL) {
				bullet_draw_laser(buffer, turret);
			}
		}
		else
			break;
		turret = turret->next;
	}    
	solid_mode();

	
	/* storm fire */
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
	turret = gd->turret;
	for (;;) {
		if (turret != NULL) {
			if (turret->type == TURRET_TYPE_STORM && turret->laser_fire > 0 && turret->target_monster != NULL) {
				bullet_draw_storm(buffer, turret);
			}
		}
		else
			break;
		turret = turret->next;
	}    
	solid_mode();


 	/* candy */
	candy = gd->candy;
	for (;;) {

		if (candy != NULL) {
			candy_draw_candy(buffer, candy);
		}
		else
			break;

		candy = candy->next;
	}

	
	/* build turret? */
	if (tile_is_turretslot(gd->mouse_map_x, gd->mouse_map_y) && !tur_is_turret(gd->mouse_map_x, gd->mouse_map_y)) {
		if (gd->build_turret != TURRET_TYPE_NONE) {
			draw_sprite(buffer, gd->bitmaps->turret_base, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
			draw_sprite(buffer, gd->bitmaps->turret_level_1, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
			if (gd->build_turret == TURRET_TYPE_GUN)   draw_sprite(buffer, gd->bitmaps->turret_bitmap_gun, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
			else if (gd->build_turret == TURRET_TYPE_SLOW)  draw_sprite(buffer, gd->bitmaps->turret_bitmap_slow, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
			else if (gd->build_turret == TURRET_TYPE_LASER) draw_sprite(buffer, gd->bitmaps->turret_bitmap_laser, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
			else if (gd->build_turret == TURRET_TYPE_STORM) draw_sprite(buffer, gd->bitmaps->turret_bitmap_storm, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16);
		}
	}	


	
	/* active / selected sector */

	if (active_sector_alpha_dir == 1)
		active_sector_alpha += 1.1;
	else
		active_sector_alpha -= 0.8;
		
	if (active_sector_alpha > 50)
		active_sector_alpha_dir = 0;
	if (active_sector_alpha < 30)
		active_sector_alpha_dir = 1;

	/* active */
	if (tile_is_turretslot(gd->mouse_map_x, gd->mouse_map_y) && !tur_is_turret(gd->mouse_map_x, gd->mouse_map_y)) {
		color = makecol(255,255,255);
		drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    	set_trans_blender(0,0,0,(int)active_sector_alpha);
		rect(buffer, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16 , gd->mouse_map_x_real + 16, gd->mouse_map_y_real + 16, color);
		set_trans_blender(0,0,0,active_sector_alpha - 10);
		rectfill(buffer, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16 , gd->mouse_map_x_real + 16, gd->mouse_map_y_real + 16, color);
		solid_mode();
	}

	
	if (tur_is_turret(gd->mouse_map_x, gd->mouse_map_y) && gd->mouse_map_x != gd->selected_x && gd->mouse_map_y != gd->selected_y) {
		color = makecol(255,0xfc,0x9e);
		drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
		set_trans_blender(0,0,0,(int)active_sector_alpha);
		rectfill(buffer, gd->mouse_map_x_real - 16, gd->mouse_map_y_real - 16 , gd->mouse_map_x_real + 16, gd->mouse_map_y_real + 16, color);
		solid_mode();
	}

	if (gd->selected_x != -1) {
		if (tur_is_turret(gd->selected_x, gd->selected_y)) {
			color = makecol(0x4b,255,0x2d);
			drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
			set_trans_blender(0,0,0,(int)active_sector_alpha);
			rectfill(buffer, gd->selected_x * 32, gd->selected_y * 32, gd->selected_x * 32 + 32, gd->selected_y * 32 + 32, color);
			solid_mode();
		}
	}
		

	/* XXX */
	rectfill(buffer, 480, 0, 1000, buffer->h, makecol(20,20,20));
	line(buffer, 481, 0, 481, screen->h, makecol(180,180,180));



	/* part: info */
	rectfill(gd->bitmaps->part_info, 0, 0, gd->bitmaps->part_info->w, gd->bitmaps->part_info->h, makecol(0,0,0));
	rect(gd->bitmaps->part_info, 0, 0, gd->bitmaps->part_info->w - 1, gd->bitmaps->part_info->h - 1, makecol(60,60,60));
	textprintf_ex(gd->bitmaps->part_info, font, 3, 3, makecol(255,255,255), -1,   "Money: %d", (int)gd->player_money);
	textprintf_ex(gd->bitmaps->part_info, font, 3, 13, makecol(255,255,255), -1,  "Lives: %d", gd->player_lives);
	draw_sprite(buffer, gd->bitmaps->part_info, 482 + 10, 10);
	
	/* part: turrets */
	draw_sprite(buffer, gd->bitmaps->part_turrets, 482 + 10, gd->bitmaps->part_info->h + 20);

	
	
	/* screen: debug */
	if (gd->toggle_show_debug) {
		set_trans_blender(0,0,0,180);
		draw_trans_sprite(buffer, gd->bitmaps->screen_debug, 10, 10);
	}
	
	
	/* mouse */
	drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(0,0,0,200);
	line(buffer, mouse_x-10, mouse_y, mouse_x+10, mouse_y, makecol(255,255,255));		
	line(buffer, mouse_x, mouse_y-10, mouse_x, mouse_y+10, makecol(255,255,255));
	solid_mode();


	
}
