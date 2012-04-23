#include <allegro.h>
#include <assert.h>
#include "common.h"

void candy_add_vis_hit(game_data_t *gd, int x, int y, int value)
{
	candy_t *candy;
	candy_t *new_candy;
		
	candy = gd->candy;	

	new_candy = malloc(sizeof (candy_t));
	assert (new_candy != NULL);

	new_candy->x = x;
	new_candy->y = y;
	new_candy->vis_hit_value = value;
	new_candy->ttl = 80;
	new_candy->vis_hit_alpha = 200;
	new_candy->next = NULL;
	
	new_candy->bitmap = create_bitmap(32,32);
	
	assert (new_candy->bitmap != NULL);
	rectfill(new_candy->bitmap, 0, 0, 32, 32, makecol(255,0,255));
	textprintf_centre_ex(new_candy->bitmap, font, 16, 16, makecol(0,255,0), -1, "+%d", new_candy->vis_hit_value);
	
	if (gd->candy == NULL) {
		gd->candy = new_candy;
		return;
	}

	candy = gd->candy;
	for (;;) {
		if (candy->next == NULL) {
			candy->next = new_candy;
			break;
		}
		candy = candy->next;
	}
	
	return;		
}

void candy_draw_candy(BITMAP *dest, candy_t *candy)
{
	if (candy->ttl < 0)
		return;
	
	set_trans_blender(0,0,0, 40);
	draw_trans_sprite(dest, candy->bitmap, candy->x - 16 + 1, candy->y - 16 + 1);
	set_trans_blender(0,0,0, candy->vis_hit_alpha);
	draw_trans_sprite(dest, candy->bitmap, candy->x - 16, candy->y - 16);
}



static inline void candy_do_candy(candy_t *candy)
{
	if (candy->ttl < 0)
		return;
	
	candy->ttl--;
	candy->vis_hit_alpha -= 2;
	candy->y -= 1.1;
	
	if (candy->vis_hit_alpha < 0)
		candy->vis_hit_alpha = 0;
	
	/*if (rand()%2 == 1) 
		candy->x += 0.3;
	else
		candy->x -= 0.3; */
}


void candy_logic_do_candy(game_data_t *gd)
{
	candy_t *candy;
	candy_t *first_victim = NULL;

	candy = gd->candy;
	if (candy == NULL)
		return;

	for (;;) {
		if (candy != NULL) {

			candy_do_candy(candy);
	
			if (candy->ttl < 0)
				first_victim = candy;
			

		} /* candy */
		else
			break;
		candy = candy->next;
	}
	
	if (first_victim != NULL)
		candy_remove_candy(first_victim);
	
}




void candy_remove_candy(candy_t *candy)
{
	candy_t *b;
	game_data_t *gd;

	gd = get_game_data_pointer();
	
	b = gd->candy;
	if (b == NULL)
		return;
	

	if (b == candy) {
		if (candy->next != NULL)
			gd->candy = candy->next;
		else
			gd->candy = NULL;
		destroy_bitmap(candy->bitmap);
		free(candy);
		return;
	}
	
	for (;;) {
		if (b->next == candy) {
			if (candy->next != NULL) 
				b->next = candy->next;
			else b->next = NULL;
			destroy_bitmap(candy->bitmap);
			free(candy);
			return;
		}
		if (b->next == NULL)
			break;
		b = b->next;		
	}
}

