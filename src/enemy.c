#include "main.h"
#include "vector.h"
#include "ngl.h"

#include "enemy.h"
#include "player.h"
#include "projectile.h"

error_t enemies_append(Enemies *arr, Enemy enemy) {
    if (arr->cap == arr->size) {
        if (arr->cap >= 0) arr->cap = 256;
        arr->cap *= 2;
        arr->data = realloc(arr->data, arr->cap * sizeof(Enemy));
        if (!arr->data) return ERR_FAILED_MALLOC;
    }

    arr->data[arr->size++] = enemy;
    return ERR_SUCCESS;
}

error_t enemies_pop_back(Enemies *arr, size_t i) {
    if (i >= arr->size) return ERR_INVALID_SIZE;
    
    arr->data[i] = arr->data[arr->size-1];
    arr->size--;
    
    return ERR_SUCCESS;
}

/*
 * v = <12, 32>
 * c = <20, 20>
 * 
 * nv = <v.x - c.x, v.y - c.y> -> <-8, 12>
 * nv = c - v -> <8, -12>
 */

void enemy_new(Enemies *enemies, Vec2 pos) {
    Enemy enemy = {0};
    enemy.pos    = pos;
    enemy.w      = random_range(2, 4);
    enemy.h      = enemy.w / 2;

    enemy.speed  = vec2_scale(vec2(enemy.w, enemy.h), 2);
    //enemy.vel    = vec2_mul(vec2_normalize(vec2_add(center, vec2_scale(enemy.pos, -1.0f))), enemy.speed);
    enemy.vel    = vec2_mul(vec2(-1, 0), enemy.speed);
    
    enemy.hp     = enemy.w * enemy.h * 10;
    enemy.damage = enemy.hp / 5;
    enemy.alive  = true;

    enemies_append(enemies, enemy);
}

void enemy_kill(Enemies *enemies, size_t i) {
    enemies_pop_back(enemies, i);
}

void enemies_draw(screen_t *screen, Enemies enemies) {
    for (size_t i = 0; i < enemies.size; ++i) {
        if (enemies.data[i].alive) {
            draw_rect(screen, enemies.data[i].pos.x, enemies.data[i].pos.y, enemies.data[i].w, enemies.data[i].h, '*', (color_t){70, 60, 50});
            draw_text_fmt(screen, font, 1, i*font.h, 'o', (color_t) {255,255,255}, "%03f,%03f", enemies.data[i].pos.x, enemies.data[i].pos.y);
        }
    }
}

void enemies_clean(Enemies *enemies) {
    size_t i = 0;

    while (i < enemies->size) {
        if (!enemies->data[i].alive) {
            enemies_pop_back(enemies, i);
        } else {
            ++i;
        }
    }
}

u32 enemies_update_projectile(Enemies *enemies, Projectiles *projectiles, f64 dt) {
    u32 lost_hp = 0;

    for (size_t i = 0; i < enemies->size; ++i) {
        Enemy *enemy = &enemies->data[i];
        enemy->pos = vec2_add(enemy->pos, vec2_scale(enemy->vel, dt));


        if (enemy->pos.x < 0) {
            enemy->alive = false;
            lost_hp += enemy->w * enemy->h;
            continue;
        }


        for (size_t j = 0; j < projectiles->size; ++j) {
            Projectile *projectile = &projectiles->data[j];

            if (is_coliding(enemy->pos, enemy->w, enemy->h, projectile->pos, 1, 1) && enemy->alive && projectile->alive) {
                enemy->alive = false;
                projectile->alive = false;
                break;
            }
        }
    }

    enemies_clean(enemies);

    return lost_hp;
}

u32 enemies_update_player(Enemies *enemies, Player player) {
    u32 lost_hp = 0;
    for (size_t i = 0; i < enemies->size; ++i) {
        Enemy *enemy = &enemies->data[i];
        if (is_coliding(enemy->pos, enemy->w, enemy->h, player.pos, player.sprite.w, player.sprite.h) && enemy->alive) {
            lost_hp += enemy->w * enemy->h * 3;
            enemy->alive = false;
        }
    }
    return lost_hp;
}

u32 enemies_update(Enemies *enemies, Projectiles *projectiles, Player player, f64 dt) {
    return enemies_update_projectile(enemies, projectiles, dt) + enemies_update_player(enemies, player);
}
