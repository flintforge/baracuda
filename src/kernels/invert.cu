
extern "C" __global__ 
void invert ( uchar4 *frame ) 
{
    float x = threadIdx.x + blockIdx.x * blockDim.x;
    float y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;
    frame[offset].x = 255-frame[offset].x;
    frame[offset].y = 255-frame[offset].y; 
    frame[offset].z = 255-frame[offset].z;
}

