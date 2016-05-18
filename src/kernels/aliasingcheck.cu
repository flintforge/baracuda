#include "baracuda.hcu"

extern "C" __global__
void aliasingcheck (
        rgba8 * pixels,
        uint width,
        uint height,
        int time
        )
{
    _GLSL_SAUCE_;

    float D = abs(sqrtf(
                      (1.f+FragCoord.x) * FragCoord.x
                      + FragCoord.y*FragCoord.y
                      )-time/10.0f);

    vec3 px = D;

    /*
    this crash at src/op.cpp>, line 114. cuCtxSynchronize
    vec3 R;
    R = px; // pb passing ref ptr ? vec3(px) is ok.
    pixels << R;
    // */
    Output3(pixels, px);

}
