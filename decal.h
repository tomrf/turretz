#ifndef DECAL_H
#define DECAL_H

typedef struct struct_decal {
	BITMAP *bitmap;
	int    x;
	int    y;
	int    ttl;
	struct struct_decal *next;
} decal_t;

#endif
