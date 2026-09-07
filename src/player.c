#include "vector.h"
#include "main.h"
#include "player.h"
#include "projectile.h"

void player_new(Player *player, Vec2 pos) {
    player->pos   = pos;
    player->vel   = vec2(1, 1);
    player->speed = vec2(50, 30);
    player->acceleration = vec2_scale(player->speed, 0.02);
    player->hp = 100;
    player->shoot_timer = 0;
    player->shoot_time  = 300;
}

void player_load_sprite(Player *player, Sprite sprite) {
    player->sprite = sprite;
}

void player_move(Player *player, DirFlag dir, f64 dt) {
    Vec2 direction = map_dir(dir);

    Vec2 target = vec2(
        direction.x * player->speed.x,
        direction.y * player->speed.y
    );

    /*
     * Exponential smoothing.
     */
    f64 ax = 1.0 - exp(-player->acceleration.x * dt);
    f64 ay = 1.0 - exp(-player->acceleration.y * dt);

    player->vel.x += (target.x - player->vel.x) * ax;
    player->vel.y += (target.y - player->vel.y) * ay;

    player->pos.x += player->vel.x * dt;
    player->pos.y += player->vel.y * dt;

    player->shoot_timer += dt * 1000;
}

void player_shoot(Player *player, Projectiles *projectiles) {
    if (player->shoot_timer >= player->shoot_time) {
        Vec2 projectile_pos = vec2(
            player->pos.x + (f64)player->sprite.w / 2,
            player->pos.y + (f64)player->sprite.h / 2);

        projectile_new(projectiles, projectile_pos,  vec2_rot90cw(vec2_normalize(player->vel)), 10);
        projectile_new(projectiles, projectile_pos, vec2_rot90ccw(vec2_normalize(player->vel)), 10);
        player->shoot_timer = 0;
    }
}

