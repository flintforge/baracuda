/*
 * from https://www.shadertoy.com/view/MdBGDK
 */

#define TAU 6.28318530718

#define _float2 make_float2
#define _float3 make_float3
#define _float4 make_float4

__device__ float mod(float x,float y) { return x - y * floorf(x/y); }
__device__ float dot(float2 v1, float2 v2) { return ( v1.x * v2.x + v1.y * v2.y ); }


/*__shared__ float gTime;
__shared__ float2 mou;*/
extern "C" __global__ 
void ribbon_assault (
        uchar4 *pixel,
        unsigned int width,
        unsigned int height,
        int time )
{
    // map from threadIdx/BlockIdx to pixel position
    float x = threadIdx.x + blockIdx.x * blockDim.x;
    float y = threadIdx.y + blockIdx.y * blockDim.y;
    uint offset = x + y * blockDim.x * gridDim.x;
    float W=width;
    float H=height;

    float gTime = time/1000.0f+11.0f;

    float f = 3.f, g = 3.f;

    //float2 mou = _float2(sin(gTime/10.0f)*W, cos(gTime/10.0f)*H);

    float2 mou = _float2(
                sin(gTime*.3)*sin(gTime*.17)*1.+sin(gTime*.3) ,
                (1.0-cos(gTime*.632f))*sin(gTime*.131f)*1.0f+cos(gTime*.3));
    mou.x=(mou.x+1.0f)*W;
    mou.y=(mou.y+1.0f)*H;


    float2 z = _float2((-W+2.0f*x)/H, (-H+2.0f*y)/H);
    float2 p = _float2((-W+2.0f+mou.x)/H, (-H+2.0f+mou.y)/H);


    for( int i = 0; i < 20; i++)
    {
        float d = dot(z,z);
        z = _float2( p.x+z.x/d, p.y/y-z.y/d );
        z.x =  abs(z.x);
        float2 ZP= _float2(z.x-p.x,z.y-p.y);
        float2 PZ= _float2(z.x+p.x,z.y+p.y);
        f = max( f, (dot(ZP,ZP) ));
        g = min( g, sin(mod(dot(PZ,PZ), TAU))+1.0f);
    }
    f = abs(-log(f) / 3.5f);
    g = abs(-log(g) / 8.0f);
    float3 V=_float3(min(g,1.0f), min(g*f,1.0f), min(f,1.0f) );

    pixel[offset].x = V.z*255;
    pixel[offset].y = V.y*255;
    pixel[offset].z = V.x*255;
    pixel[offset].w = 255;

}
