#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "common.h"

int startup(void)
{

    LOG("=== startup ===");

    LOG("seeding rng");
    srand(time(NULL));


    LOG("initializing allegro");
    allegro_init();

    LOG("installing timer");
    install_timer();

    LOG("setting color depth to 32");
    set_color_depth(32);

    LOG("attempting graphics mode");

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 700, 480, 0, 0) < 0)
        DIE("error: failed to enable gfx mode: %s", allegro_error);

    LOG("color depth is now %d", get_color_depth());

    LOG("installing digital sound driver");
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0)
        DIE("error: failed to install sound system: %s", allegro_error);

    LOG("setting volume 255");
    set_volume(255, 0);

    LOG("installing keyboard");
    install_keyboard();

    LOG("installing mouse");
    install_mouse();

    LOG("set mouse speed");
    set_mouse_speed(3, 3);

    //LOG("setting keyboard delay and repeat values");
    //   set_keyboard_rate(100, 100);

    LOG("hide builtin mouse");
    show_mouse(NULL);

    LOG("=== startup complete ===");

    return 0;
}


void clean_shutdown(void)
{
    LOG("clean_shutdown called!");

    exit(1);
}
