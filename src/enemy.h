#ifndef _ENEMY_H
#define _ENEMY_H

#include "vector.h"
#include "ngl.h"

#include "player.h"
#include "projectile.h"

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 speed;

    u16  hp;
    u16  damage;
    bool alive;

    u16  w, h;
} Enemy;

typedef struct {
    size_t size;
    size_t cap;
    Enemy *data;
} Enemies;

void enemy_new(Enemies *enemies, Vec2 pos);
void enemies_draw(screen_t *screen, Enemies enemies);
u32 enemies_update(Enemies *enemies, Projectiles *projectiles, Player player, f64 dt);




#endif /* _ENEMY_H */
