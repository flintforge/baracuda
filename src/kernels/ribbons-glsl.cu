
#include "baracuda.hcu"
#define TAU 6.28318530718

extern "C" __global__
void ribbon_glsl(
        uchar4 *frame,
        unsigned int width,
        unsigned int height,
        int time
         )
{
    _GLSL_SAUCE_

    float gTime = time/1000.0f+11.0f;

    float f = 3.f, g = 3.f;

    vec2 mu;
    mu.x = sin(gTime*.3) * sin(gTime*.17)*1.f + sin(gTime*.3);
    mu.y = (1.0-cos(gTime*.632f)) * sin(gTime*.131f)*1.f + cos(gTime*.3);
    mu = (mu+1) * FragSize;

    vec2 z = ((-FragSize + FragCoord.xy*2.0f) / FragSize.y );
    vec2 p = ((-FragSize + 2.0f + mu ) / FragSize.y );


    for( int i = 0; i < 20; i++)
    {
            float d = dot(z,z);
            z = (vec2(z.x,-z.y ) / d)+p;
            z.x =  abs(z.x);
            f = max( f, (dot(z-p,z-p) ));
            g = min( g, sin(mod(dot(z+p,z+p), TAU))+1.0f);
    }
    f = abs(-log(f) / 3.5f);
    g = abs(-log(g) / 8.0f);
    vec3 V = min(vec3(f,f*g,g),1.0f);

    V *=255;
    Output3(frame,V);
}
