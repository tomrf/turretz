#ifndef PROTO_H
#define PROTO_H


/* main.c */
game_data_t*    get_game_data_pointer(void);
int 			tile_is_turretslot(int x, int y);
int 			tile_is_path(int x, int y);


/* log.c */
void       		LOG(char *fmt, ...);
void       		DIE(char *fmt, ...);


/* startup.c */
int        		startup(void);
void       		clean_shutdown(void);


/* input.c */
void 			input_do_input(game_data_t *gd);


/* draw.c */
void 			draw(game_data_t *gd, BITMAP *buffer);


/* monster.c*/
void 			monster_draw_monster(BITMAP *dest, monster_t *monster);
int 			monster_add_monster(game_data_t *gd, int type, int x, int y);
void 			monster_logic_do_monsters(game_data_t *gd);
void 			monster_remove_monster(monster_t *monster);
void 			monster_register_hit(monster_t *monster, float damage);


/* turret.c */
void 			tur_draw_turret(BITMAP *dest, turret_t *turret);
int 			tur_add_turret(game_data_t *gd, int type, int x, int y);
int 			tur_is_turret(int x, int y);
void 			tur_logic_do_turrets(game_data_t *gd);
int 			tur_target_within_range(turret_t *turret, int target_x, int target_y);
int 			get_distance(int x, int y, int x2, int y2);
int 			tur_get_distance(turret_t *turret, int target_x, int target_y);
int 			tur_cleanup_target(monster_t *monster);


/* decal.c */
int 			decal_add_decal(game_data_t *gd, int x, int y);
void 			decal_logic_do_decals(game_data_t *gd);


/* bullet.c */
void 			bullet_draw_bullet(BITMAP *dest, bullet_t *bullet);
int 			bullet_add_bullet(turret_t *turret, monster_t *target_monster, int target_range);	
void 			bullet_logic_do_bullets(game_data_t *gd);
void 			bullet_register_hit(bullet_t *bullet);
int 			bullet_cleanup_target(monster_t *monster);
void 			bullet_draw_laser(BITMAP *dest, turret_t *turret);
void 			bullet_draw_storm(BITMAP *dest, turret_t *turret);


/* candy.c */
void 			candy_add_vis_hit(game_data_t *gd, int x, int y, int value);
void 			candy_draw_candy(BITMAP *dest, candy_t *candy);
void 			candy_logic_do_candy(game_data_t *gd);
void 			candy_remove_candy(candy_t *candy);


/* bitmap.c */
void 			bitmap_load_bitmaps(struct struct_bitmaps *bitmaps);


/* sound.c */
void 			sound_load_samples(struct struct_samples *samples);


#endif
