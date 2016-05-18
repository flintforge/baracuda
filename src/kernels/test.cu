#include "baracuda2.hcu"

extern "C" __global__
void test (
        rgba8 * pixels,
        uint width,
        uint height,
        int time
        )
{
    _GLSL_SAUCE_;

    vec2 m=vec2(128,0);
    vec2 n = m;
    n.x = 0;
    n.y = 128;
    vec2 o = m + n;

    pixels[OFFSET].x = o.x;
    pixels[OFFSET].y = o.y;
    pixels[OFFSET].z = 128;
    pixels[OFFSET].w = 255;
}
