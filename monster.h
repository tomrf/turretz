#ifndef MONSTER_H
#define MONSTER_H

#define MONSTER_TYPE_DRONE    0
#define MONSTER_TYPE_FLOATER  1
#define MONSTER_TYPE_TANK     2
#define MONSTER_TYPE_CRAWLER  3

#define MONSTER_TURN_RIGHT    1
#define MONSTER_TURN_LEFT     2
#define MONSTER_TURN_NONE     0

typedef struct struct_monster {
	BITMAP *bitmap;
	
	int    type;
	
	float  x;
	float  y;
	float  target_x;
	float  target_y;
		
	float  speed;
	int	   money;
	
	float  x_fudge;
	float  y_fudge;
	
	int    pmx;
	int    pmy;
	int	   pdir;
	int	   turn;

	int    direction;
	
	float  pixels_traveled;

	fixed  angle;
	fixed  desired_angle;

	struct struct_monster *next;

	float    hp;
	int      hp_max;
	float    shield;
	int      shield_max;
	
	int    ttl;
	
	int    slowness;
	
} monster_t;

#endif
