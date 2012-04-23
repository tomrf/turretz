#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <allegro.h>
#include <assert.h>
#include <math.h>
#include "common.h"


volatile int ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)

int updates_per_second = 100;

game_data_t* get_game_data_pointer(void)
{
	static game_data_t *game_data = NULL;
	
	if (game_data == NULL) {
		game_data = malloc(sizeof (game_data_t));
		assert(game_data != NULL);
	}
	
	return game_data;
}


int main(void)
{
 	int 		fps = 0;
	int 		frames_done = 0;
	int 		dropped_frames = 0;
	time_t 		old_time = 0;
	float 		draw_time;
	float 		logic_time;
	int 		i;
	
	int xxx_num_monsters = 2;
	int xxx_wave_num = 0;
	int xxx_next_wave_count = 2000;
	int xxx_next_monster_count = 180;
	int xxx_monsters_placed = 0;
	int xxx_monster_type = 0;
	game_data_t *gd;
	BITMAP *xxx_b;
	
	
	
	struct timeval tv;
	struct timezone tz;
	time_t 		ta_sec, ta_usec, tb_sec, tb_usec;;
	
	BITMAP 		*screen_buffer;
	BITMAP 		*debug;

	game_data_t *game_data;


       	
    /* DO STARTUP */
    startup();

	/* START TIMER */
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    /* CREATE BITMAPS */
    screen_buffer = create_bitmap(screen->w, screen->h);
    
    /* CREATE GAME_DATA */
    game_data = get_game_data_pointer();
    game_data->bullet = NULL;
    game_data->monster = NULL;
    game_data->turret = NULL;
    game_data->candy = NULL;
    game_data->build_turret = TURRET_TYPE_NONE;
    game_data->screen_buffer = screen_buffer;
    game_data->toggle_show_debug = 0;
    game_data->player_money = 250;
    game_data->player_lives = 10;
    game_data->selected_x = -1;
    game_data->selected_y = -1;

	/* LOAD BITMAPS */
    game_data->bitmaps = malloc(sizeof (struct struct_bitmaps));
    bitmap_load_bitmaps(game_data->bitmaps);
    
    /* LOAD SAMPLES */
	game_data->samples = malloc(sizeof (struct struct_samples));
	sound_load_samples(game_data->samples);

	/* HELPERS */
	debug = game_data->bitmaps->screen_debug;
	
	
	/* XXX */
	gd = game_data;
	xxx_b = gd->bitmaps->part_turrets;
	rectfill(xxx_b, 0, 0, xxx_b->w, xxx_b->h, makecol(0,0,0));
	rect(xxx_b, 0, 0, xxx_b->w - 1, xxx_b->h - 1, makecol(60,60,60));
	
	textprintf_ex(xxx_b, font, 4, 6, makecol(200,200,200), -1,  "GUN");
	textprintf_ex(xxx_b, font, 4, 24, makecol(150,150,150), -1,  "$ 100");
	draw_sprite(xxx_b, gd->bitmaps->turret_base, xxx_b->w - 37, 6);
	draw_sprite(xxx_b, gd->bitmaps->turret_level_1, xxx_b->w - 37, 6);
	draw_sprite(xxx_b, gd->bitmaps->turret_bitmap_gun, xxx_b->w - 37, 6);

	line(xxx_b, 4, 34 + 5, xxx_b->w - 5, 34 + 5, makecol(35,35,35));

	textprintf_ex(xxx_b, font, 4, 46, makecol(200,200,200), -1,  "SLOW");
	textprintf_ex(xxx_b, font, 4, 64, makecol(150,150,150), -1,  "$ 200");
	draw_sprite(xxx_b, gd->bitmaps->turret_base, xxx_b->w - 37, 46);
	draw_sprite(xxx_b, gd->bitmaps->turret_level_1, xxx_b->w - 37, 46);
	draw_sprite(xxx_b, gd->bitmaps->turret_bitmap_slow, xxx_b->w - 37, 46);

	line(xxx_b, 4, 74 + 5, xxx_b->w - 5, 74 + 5, makecol(35,35,35));

	textprintf_ex(xxx_b, font, 4, 86, makecol(200,200,200), -1,  "LASER");
	textprintf_ex(xxx_b, font, 4, 104, makecol(150,150,150), -1,  "$ 250");
	draw_sprite(xxx_b, gd->bitmaps->turret_base, xxx_b->w - 37, 86);
	draw_sprite(xxx_b, gd->bitmaps->turret_level_1, xxx_b->w - 37, 86);
	draw_sprite(xxx_b, gd->bitmaps->turret_bitmap_laser, xxx_b->w - 37, 86);

	line(xxx_b, 4, 114 + 5, xxx_b->w - 5, 114 + 5, makecol(35,35,35));

	textprintf_ex(xxx_b, font, 4, 126, makecol(200,200,200), -1,  "STORM");
	textprintf_ex(xxx_b, font, 4, 144, makecol(150,150,150), -1,  "$ 300");
	draw_sprite(xxx_b, gd->bitmaps->turret_base, xxx_b->w - 37, 126);
	draw_sprite(xxx_b, gd->bitmaps->turret_level_1, xxx_b->w - 37, 126);
	draw_sprite(xxx_b, gd->bitmaps->turret_bitmap_storm, xxx_b->w - 37, 126);
	
	textprintf_ex(xxx_b, font, 4, 200, makecol(200,200,200), -1,  "Press 1,2,3,4 to select");
	textprintf_ex(xxx_b, font, 4, 210, makecol(200,200,200), -1,  "towers.  Left mouse to");
	textprintf_ex(xxx_b, font, 4, 220, makecol(200,200,200), -1,  "place on aviable tiles.");
	textprintf_ex(xxx_b, font, 4, 240, makecol(200,200,200), -1,  "Hold F key to speed up.");



    /* GAME LOOP */
    
	for (;;) {
		
		if (time(NULL) > old_time) {
			fps = frames_done;
			
			if (frames_done < updates_per_second)
				dropped_frames += updates_per_second - frames_done;
			
			frames_done = 0;
		}
		
		old_time = time(NULL);

		while (ticks == 0) {
			rest(100 / updates_per_second);
		}
	
		while (ticks > 0) {
			int old_ticks = ticks;
		
			/* do logic */
			gettimeofday(&tv, &tz);
			ta_usec = tv.tv_usec;
			ta_sec = tv.tv_sec;			
			
			for (i = 0; i < 3; i++) {

	
				if (xxx_next_wave_count > 0)
					xxx_next_wave_count--;
				if (xxx_next_wave_count == 1)
					xxx_wave_num++;
				if (xxx_next_wave_count == 0) {
					xxx_next_monster_count--;
					if (xxx_next_monster_count == 0) {
						if (xxx_monster_type == 0)
							xxx_next_monster_count = 40 + rand()% 50;
						else
							xxx_next_monster_count = 150 + rand()% 50;
						monster_add_monster(game_data, xxx_monster_type, 0, 0);
						xxx_monsters_placed++;
						if (xxx_monsters_placed == xxx_num_monsters) {
							xxx_next_wave_count = 2000;
							xxx_monsters_placed = 0;
							xxx_monster_type++;
							if (xxx_monster_type > 3) {
								xxx_monster_type = 0;
								xxx_num_monsters += 2;	
							}							
						}
					}
				}
				
				
				game_data->player_money += INTEREST_RATE * game_data->player_lives;

				input_do_input(game_data);
				tur_logic_do_turrets(game_data);
				bullet_logic_do_bullets(game_data);
				monster_logic_do_monsters(game_data);
				candy_logic_do_candy(game_data);
				if (!key[KEY_F])
					break;
			}
			
			
			gettimeofday(&tv, &tz);
			tb_usec = tv.tv_usec;
			tb_sec = tv.tv_sec;
			logic_time = (float) (tb_usec - ta_usec) / 1000 + (tb_sec - ta_sec) * 1000;

			

			
			ticks--;
			if (old_ticks <= ticks)
				break;
		}

		/* draw everything to screen buffer*/

		gettimeofday(&tv, &tz);
		ta_usec = tv.tv_usec;
		ta_sec = tv.tv_sec;

		draw(game_data, screen_buffer);

		/* debug text */
	 	if (game_data->toggle_show_debug) {
 			rectfill(debug, 0, 0, debug->w, debug->h, makecol(0,0,0));
 			rect(debug, 0, 0, debug->w - 1, debug->h - 1, makecol(200,200,200));
			textprintf_ex(debug, font, 3, 3, makecol(255,255,255), -1,  "FPS: %d (%d) Drp: %d  ", fps, frames_done, dropped_frames);
			textprintf_ex(debug, font, 3, 23, makecol(255,255,255), -1, "Draw time:  %.1f ms", draw_time);
			textprintf_ex(debug, font, 3, 33, makecol(255,255,255), -1, "Logic time: %.1f ms", logic_time);
		}

		/* xxx tmp tmp xxx */
		textprintf_ex(screen_buffer, font, 482 + 3 + 10, 43, makecol(255,255,255), -1,  "Wave #: %d", xxx_wave_num);
		textprintf_ex(screen_buffer, font, 482 + 3 + 10, 53, makecol(255,255,255), -1,  "Next wave: %.0f ", (float)xxx_next_wave_count / 2000 * 100);


		/* dump buffer to screen */
		draw_sprite(screen, screen_buffer, 0, 0);
		frames_done++;

		
		gettimeofday(&tv, &tz);
		tb_usec = tv.tv_usec;
		tb_sec = tv.tv_sec;
		draw_time = (float) (tb_usec - ta_usec) / 1000 + (tb_sec - ta_sec) * 1000;

    	
	} /* for */


    /* SHUT DOWN EVERYTHING */
    LOG("shutting down...");
    clean_shutdown();

    return 0;
}
END_OF_MAIN()



int tile_is_turretslot(int x, int y)
{
	game_data_t *gd;
	
	if (x < 0 || y < 0)
		return 0;
	
	gd = get_game_data_pointer();
	
	if (getpixel(gd->bitmaps->map_background, x * 32 + 16, y * 32 + 16) == 8092540)
		return 1;
	
    return 0;		
}

int tile_is_path(int x, int y)
{
	game_data_t *gd;
	
	if (x < 0 || y < 0)
		return 0;

	gd = get_game_data_pointer();
		
	if (getpixel(gd->bitmaps->map_background, x * 32 + 16, y * 32 + 16) == 8164006)
		return 1;
	
    return 0;		
}

