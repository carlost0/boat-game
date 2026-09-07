#ifndef _VECTOR_H
#define _VECTOR_H
#include <math.h>
typedef struct {
	double x, y;
} Vec2;

#define vec2(x, y) ((Vec2){(double)(x), (double)(y)})

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, Vec2 b);
Vec2 vec2_sqrt(Vec2 vec);
Vec2 vec2_normalize(Vec2 vec);
Vec2 vec2_scale(Vec2 vec, double scalar);
Vec2 vec2_rot90cw(Vec2 vec);
Vec2 vec2_rot90ccw(Vec2 vec);


#endif /* _VECTOR_H */

#ifdef VECTOR_IMPLEMENTATION

Vec2 vec2_add(Vec2 a, Vec2 b) {
    return vec2(a.x + b.x, a.y + b.y);
}

Vec2 vec2_mul(Vec2 a, Vec2 b) {
    return vec2(a.x * b.x, a.y * b.y);
}

Vec2 vec2_sqrt(Vec2 vec) {
    return vec2(sqrt(vec.x), sqrt(vec.y));
}

Vec2 vec2_normalize(Vec2 vec) {
    double length = sqrt(vec.x * vec.x + vec.y * vec.y);

    if (length == 0.0) {
        return vec2(0.0, 0.0);
    }

    return vec2(vec.x / length, vec.y / length);
}

Vec2 vec2_scale(Vec2 vec, double scalar) {
    return vec2(vec.x * scalar, vec.y * scalar);
}

Vec2 vec2_rot90cw(Vec2 vec) {
    return vec2(-vec.y, vec.x);
}
Vec2 vec2_rot90ccw(Vec2 vec) {
    return vec2(vec.y, -vec.x);
}

#endif /* VECTOR_IMPLEMENTATION */
