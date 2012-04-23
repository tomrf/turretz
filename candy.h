#ifndef CANDY_H
#define CANDY_H

#include <allegro.h>



typedef struct struct_candy {
	
	float x;
	float y;
	int ttl;

	int vis_hit_value;
	int vis_hit_alpha;
	
	BITMAP *bitmap;
	
	struct struct_candy *next;
} candy_t;


#endif
