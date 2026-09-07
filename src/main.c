/* 
 * Copyright (C) 2026 Carlos G. S.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: Instagram DMs @cgs.dev
*/

/* 
 * boat game: basic game with a boat.
 */

#include "main.h"

#include "player.h"
#include "projectile.h"
#include "enemy.h"


#define VECTOR_IMPLEMENTATION
#include "vector.h"

#include "ngl.h"

f64 get_dt(DeltaTime *dt) {
    dt->old  = dt->now;
    dt->now = get_ms();
    dt->time = ((f64)dt->now - (f64)dt->old) / 1000.0;
    return dt->time;
}

i32 random_range(i32 min, i32 max) {
    if (min > max) {
        i32 temp = min;
        min = max;
        max = temp;
    }

    return (u32)(min + rand() % (max - min + 1));
}

size_t dificulty_scale(size_t n) {
    if (n <= 2) return n;
    return dificulty_scale(n - 1) + dificulty_scale(n - 2);
}

Vec2 map_dir(DirFlag dir) {
    Vec2 res = {0};
    if (dir & DIR_UP) {
        res.y +=  -1;
    }
    if (dir & DIR_RIGHT) {
        res.x +=  1;
    }
    if (dir & DIR_DOWN) {
        res.y +=  1;
    }
    if (dir & DIR_LEFT) {
        res.x += -1;
    }
    return vec2_normalize(res);
}

bool is_coliding(Vec2 p1, u32 w1, u32 h1, Vec2 p2, u32 w2, u32 h2) {
    if (p1.x + w1   < p2.x) return 0;
    if (p1.x > p2.x + w2) return 0;
    if (p1.y + h1   < p2.y) return 0;
    if (p1.y > p2.y + h2) return 0;

    return 1;
}

int main(void) {
new_game:
    srand(time(NULL));
    get_term_size(&SCREEN_H, &SCREEN_W);

    SCREEN_H--;
    screen_t screen = {SCREEN_W, SCREEN_H, {0}, {0}};
    input_ctx_t input_ctx = {0};
	
    load_glyphs(&font, NULL);
    init_input(&input_ctx);
    init_screen(&screen);

    Player player = {0};
    player_new(&player, vec2(10, 20));

    char *heart_sprite = 
        "      "
        "/OvO\\ "
        "OOOOO "
       " \\O/  "
        "  V   ";
    char *wave_sprite =
       " /w= "
       "//   "
       "|O\\_/";

    /* 11x3 */ 
    Sprite player_horizontal = {
        .chars =
            "  -=####=-  "
            "<=#||||||#=>"
            "  -=####=-  ",
        .w = 12,
        .h = 3
    };

    /* 6x5 */
    Sprite player_vertical = {
        .chars =
            "  ^  "
            " |-| "
            "|-|>|"
            "|-^-|"
            " |-| "
            "  V  ",
        .w = 5,
        .h = 6
    };

    Projectiles projectiles = {0};
    player_load_sprite(&player, player_vertical);

    Enemies enemies = {0};

    DeltaTime delta_time_ = {
        .old = get_ms(),
        .now = get_ms(),
        .time = 0.0,
    };

    size_t wave = 0;
    bool running = true;
    clear_screen();
    printf("\x1b[48;2;0;0;0m");

    while (running) {
        get_keyboard_state(&input_ctx);
        f64 dt = get_dt(&delta_time_);

        if (is_key_down(input_ctx, KEY_ESC) || is_key_down(input_ctx, KEY_Q)) {
            running = false;
        }

        if (is_key_down(input_ctx, KEY_R)) {
            destroy_input(&input_ctx);
            destroy_screen(&screen);
            free(enemies.data);
            free(projectiles.data);
            goto new_game;
        }

        DirFlag dir = DIR_NONE;
        if (is_key_down(input_ctx, KEY_W)) {
            dir |= DIR_UP;
        } if (is_key_down(input_ctx, KEY_A)) {
            dir |= DIR_LEFT;
        } if (is_key_down(input_ctx, KEY_S)) {
            dir |= DIR_DOWN;
        } if (is_key_down(input_ctx, KEY_D)) {
            dir |= DIR_RIGHT;
        }

        player_move(&player, dir, dt);

        if (is_key_down(input_ctx, KEY_SPACE)) {
            player_shoot(&player, &projectiles);
        }

        if (fabs(player.vel.x) > fabs(player.vel.y))
            player_load_sprite(&player, player_horizontal);
        else 
            player_load_sprite(&player, player_vertical);

        player.hp -= enemies_update(&enemies, &projectiles, player, dt);
        projectiles_update(&projectiles, dt);


        clear_bg(&screen, 'l', (color_t){30,40,200});

        projectiles_draw(&screen, projectiles);
        enemies_draw(&screen, enemies);
        draw_sprite(&screen, player.pos.x, player.pos.y, player.sprite.w, player.sprite.h, player.sprite.chars, BROWN);

        draw_rect(&screen, 0, 0, 27, 14, 't', BROWN);
        draw_rect(&screen, 25, 12, 1, 1, 'o', (color_t){90, 50, 10});
        draw_sprite(&screen, 2, 1, 6, 5, heart_sprite, (color_t){255,30,30});
        draw_text_fmt(&screen, font, 3, 1, 'H', (color_t){255,30,30}, " %3d", player.hp);

        draw_sprite(&screen, 2, font.h + font.hpad + 2, 5, 3, wave_sprite, (color_t){30,30,255});
        draw_text_fmt(&screen, font, 3, font.h + font.hpad + 1, 'W', (color_t){30,30,255}, " %3u", wave);
        
        if (player.hp <= 0) {
            clear_bg(&screen, 'l', (color_t){30,40,200});
            draw_text(&screen, font, SCREEN_W / 2 - 5 * (font.w + font.vpad), SCREEN_H / 2 - 2, 'L', (color_t){255,30,30}, "You Lost :(");
            draw_text(&screen, font, SCREEN_W / 2 - 5 * (font.w + font.vpad), SCREEN_H / 2 - 2 + 6, 'R', (color_t){255,30,30}, "(r)eset");
            draw_text(&screen, font, SCREEN_W / 2 - 5 * (font.w + font.vpad), SCREEN_H / 2 - 2 + 12, 'Q', (color_t){255,30,30}, "(q)uit");
        }

        if (enemies.size == 0) {
            wave++;
            for (size_t i = 0; i < dificulty_scale(wave); ++i) {
                enemy_new(&enemies, vec2(random_range(SCREEN_W / 1.7, SCREEN_W - 1), random_range(2, SCREEN_H - 2)));
            }
        }

        draw_screen_borders(&screen, 0, BROWN);
        print_screen(&screen);
        delay(1000/60);
    }

    destroy_screen(&screen);
    destroy_input(&input_ctx);
    free(projectiles.data);
    free(enemies.data);
    return 0;
}

#define NGL_IMPLEMENTATION
#include "ngl.h"
