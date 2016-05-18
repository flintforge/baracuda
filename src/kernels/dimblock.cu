/*
this reveal the block dimension
as adressed on the device.
blocks are according to the moduli
computed from the window size and the block size.

Change the window size to see it in effect
*/

extern "C" __global__ 
void dimblock (
        uchar4* ptr,
        unsigned int width,
        unsigned int height,
        int time
        )
{
    uint x = threadIdx.x + blockIdx.x * blockDim.x;
    uint y = threadIdx.y + blockIdx.y * blockDim.y;
    uint offset = x + y * blockDim.x * gridDim.x;

    float t= sin(time/1000.0f);
    float X = (float)x/(float)width;
    float Y = (float)y/(float)height;
    ptr[offset].x = (unsigned char) ( (sinf( X + t ) + threadIdx.x ) * 255 );
    ptr[offset].y = (unsigned char) ( (sinf( Y + t ) + threadIdx.y ) * 255 );
    ptr[offset].z = sin((float)time/1000.0f ) *255;
    ptr[offset].w = 255;

}

