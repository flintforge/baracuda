#include "baracuda.hcu"

extern "C" __global__
void aliasingCheck2 (
        rgba8 * pixels,
        uint width,
        uint height,
        int time
        )
{
    _GLSL_SAUCE_;

    float stime = time/1000.0f;

    /* vec2 RG = vec2(FragCoord.xy);

    vec3 V = vec3( 1, 1, sin(gTime) );
                //RG.x,RG.y);

    vec4 P= vec4(V,1.f);
    V*=255.0f;
    pixels << V;*/

    float sX = ((float)_px_-(float)width/2)/(float)width;
    float sY = ((float)_py_-(float)height/2)/(float)height;
    int D = 255.0f*abs(sqrtf((sX+1.0f)*sX+sY*sY)-stime);

    vec3 px = D;

    pixels[OFFSET].x = px.x ;
    pixels[OFFSET].y = D ;
    pixels[OFFSET].z = D ;
    pixels[OFFSET].w = 255;

}

