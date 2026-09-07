#ifndef _MAIN_H
#define _MAIN_H
#ifndef _NGL_H
#include "ngl.h"
#endif

#include "vector.h"
#define BROWN ((color_t) {139,69,19})
#define GREY  ((color_t) {50,40,40})

typedef enum {
    DIR_NONE  = 0,
    DIR_UP    = 1 << 0,
    DIR_RIGHT = 1 << 1,
    DIR_DOWN  = 1 << 2,
    DIR_LEFT  = 1 << 3,
} Dir;

typedef i32 DirFlag;

static u16 SCREEN_W, SCREEN_H;
static font_t font = {0};

typedef struct {
    clock_t old, now;
    f64 time;
} DeltaTime;


f64 get_dt(DeltaTime *dt);
i32 random_range(i32 min, i32 max);
bool is_coliding(Vec2 p1, u32 w1, u32 h1, Vec2 p2, u32 w2, u32 h2);
Vec2 map_dir(DirFlag dir);
#endif /* _MAIN_H */
