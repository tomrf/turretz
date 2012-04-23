#include <allegro.h>
#include <assert.h>
#include "common.h"


void decal_logic_do_decals(game_data_t *gd)
{
	decal_t *decal;	
	decal_t *prev_decal = NULL;
	
	decal = gd->decal;
	if (decal == NULL)
		return;
		
	for (;;) { // XXX: first decal, gd->decal, is never removed / recycled 
		if (decal != NULL) {
			decal->ttl--;
			if (decal->ttl <= 0) {
				/* remove expired decal */
				if (prev_decal != NULL) {
					if (decal->next != NULL)
						prev_decal->next = decal->next;
					else
						prev_decal->next = NULL;
					
					free(decal);
						
					decal = prev_decal;
					if (decal == NULL)
						break;
				}
			}
		}
		else
			break;
		prev_decal = decal;
		decal = decal->next;
	}
	
}


int decal_add_decal(game_data_t *gd, int x, int y)
{
	decal_t *decal;
	decal_t *new_decal;
	decal = gd->decal;
	
	if (x < 0 || y < 0)
		return -1;
	
	new_decal = malloc(sizeof (decal_t));
	assert (new_decal != NULL);
	
	new_decal->bitmap = NULL;
	new_decal->x = x;
	new_decal->y = y;
	new_decal->ttl = 100;
	new_decal->next = NULL;

	/* first decal ? */
	if (gd->decal == NULL) {
		gd->decal = new_decal;
		return 0;
	}
	
    /* search for last node */
	decal = gd->decal;
	for (;;) {
		if (decal != NULL) {
		    /* found a decal */
			if (decal->x == x && decal->y == y) {
				/* this decal has our position, abort placement */
				return -1;
			}
		}
		if (decal->next == NULL) {
			/* found last node, adding new */
			decal->next = new_decal;
			break;
		}
		decal = decal->next;
	}
	
	return 0;
}
