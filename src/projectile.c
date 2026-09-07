#include "vector.h"
#include "main.h"

#include "projectile.h"

#include "ngl.h"

error_t projectiles_append(Projectiles *arr, Projectile projectile) {
    if (arr->cap == arr->size) {
        if (arr->cap >= 0) arr->cap = 256;
        arr->cap *= 2;
        arr->data = realloc(arr->data, arr->cap * sizeof(Projectile));
        if (!arr->data) return ERR_FAILED_MALLOC;
    }

    arr->data[arr->size++] = projectile;
    return ERR_SUCCESS;
}

error_t projectiles_pop_back(Projectiles *arr, size_t i) {
    if (i >= arr->size) return ERR_INVALID_SIZE;
    
    arr->data[i] = arr->data[arr->size-1];
    arr->size--;
    
    return ERR_SUCCESS;
}

void projectile_new(Projectiles *arr, Vec2 pos, Vec2 dir, i32 damage) {
    Projectile projectile = {0};
    projectile.damage   = damage;
    projectile.pos      = pos;
    projectile.alive    = true;
    projectile.speed    = vec2(.4, .2);
    projectile.vel      = vec2_mul(projectile.speed, dir);
    projectile.lifetime = 3;

    projectiles_append(arr, projectile);
}

void projectile_kill(Projectiles *projectiles, size_t i) {
    projectiles_pop_back(projectiles, i);
}

void projectiles_clean(Projectiles *projectiles) {
    for (size_t i = 0; i < projectiles->size; ++i) {
        if (!projectiles->data[i].alive) projectile_kill(projectiles, i);
    }
}
void projectiles_update(Projectiles *projectiles, f64 dt) {
    for (size_t i = 0; i < projectiles->size; ++i) {
        if (!projectiles->data[i].alive) continue;
        Vec2 new_vel = projectiles->data[i].vel;
        projectiles->data[i].pos = vec2_add(projectiles->data[i].pos, new_vel);
        projectiles->data[i].lifetime -= dt;
        if (projectiles->data[i].lifetime <= 0) {
            projectiles->data[i].alive = false;
        } 
    }
    projectiles_clean(projectiles);
}

void projectiles_draw(screen_t *screen, Projectiles projectiles) {
    for (size_t i = 0; i < projectiles.size; ++i) {
        draw_rect(screen, projectiles.data[i].pos.x, projectiles.data[i].pos.y, 1, 1, 'o', GREY);
    }
}
