#include <allegro.h>
#include <assert.h>
#include <math.h>
#include "common.h"


inline SAMPLE* _sound_load_sample(const char *filename)
{
	SAMPLE *sample;
	
	sample = load_sample(filename);
	assert(sample != NULL);
	
	return sample;
}


void sound_load_samples(struct struct_samples *samples)
{
	samples->gun_storm               = _sound_load_sample("snd/gun_storm.wav");
	samples->gun_laser               = _sound_load_sample("snd/gun_laser.wav");
	samples->gun_slow                = _sound_load_sample("snd/gun_slow.wav");
	samples->gun_gun                 = _sound_load_sample("snd/gun_gun.wav");
	samples->turret_rotate			 = _sound_load_sample("snd/turret_rotate.wav");
}
