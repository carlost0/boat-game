#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "ngl.h"
#include "vector.h"
#include "main.h"

typedef struct {
    Vec2 pos;
    Vec2 vel;
    Vec2 speed;

    f64  lifetime; // Seconds

    i32  damage;
    bool alive;
} Projectile;

typedef struct {
    size_t size;
    size_t cap;
    Projectile *data;
} Projectiles;

error_t projectiles_append(Projectiles *arr, Projectile projectile);
error_t projectiles_pop_back(Projectiles *arr, size_t i);
void projectile_new(Projectiles *arr, Vec2 pos, Vec2 dir, i32 damage);
void projectile_kill(Projectiles *projectiles, size_t i);
void projectiles_update(Projectiles *projectiles, f64 dt);
void projectiles_draw(screen_t *screen, Projectiles projectiles);


#endif /* _PROJECTILE_H */
