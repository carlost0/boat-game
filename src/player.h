#ifndef _PLAYER_H
#define _PLAYER_H
#include "vector.h"
#include "ngl.h"
#include "main.h"

#include "projectile.h"

#define PLAYER_MAX_W 12
#define PLAYER_MAX_H 4

typedef struct {
    size_t w, h;
    char *chars;
} Sprite;

typedef struct {
	Vec2 pos;
	Vec2 vel;
	Vec2 speed;
    Vec2 acceleration;

    i32 shoot_time;
    i32 shoot_timer;
    i32 hp;

    Sprite sprite;
} Player;

void player_new(Player *player, Vec2 pos);
void player_move(Player *player, DirFlag dir, f64 dt);
void player_shoot(Player *player, Projectiles *projectiles);
void player_load_sprite(Player *player, Sprite sprite);


#endif /* _PLAYER_H */
