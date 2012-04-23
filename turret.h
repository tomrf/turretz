#ifndef TURRET_H
#define TURRET_H

#include <allegro.h>

#define TURRET_TYPE_NONE   -1
#define TURRET_TYPE_GUN     0
#define TURRET_TYPE_SLOW    1
#define TURRET_TYPE_LASER   2
#define TURRET_TYPE_STORM   3


typedef struct struct_turret {
	BITMAP *bitmap;

	int    x;
	int    y;
	
	int    type;
	
	int	   price;
	
	int	   range;
	int    fire_rate;
	int	   fire_reset_count;
	
	float  fcount;
	int    slow_fire_distance;
	int    laser_fire;

	fixed  angle;
	fixed  desired_angle;
	
	monster_t *target_monster;

	int		level;

	struct struct_turret *next;
} turret_t;



#endif
