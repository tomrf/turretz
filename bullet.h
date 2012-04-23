#ifndef BULLET_H
#define BULLET_H

typedef struct struct_bullet {
	BITMAP *bitmap;
	float  x;
	float  y;
	float  target_x;
	float  target_y;
	fixed  angle;
	int range;
	int target_range;
	int damage;
	monster_t *target_monster;
	struct struct_bullet *next;
} bullet_t;

#endif
