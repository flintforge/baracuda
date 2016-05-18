

// turn 
extern "C" __global__ 
void grayscale ( uchar4 *img ) 
{
    float x = threadIdx.x + blockIdx.x * blockDim.x;
    float y = threadIdx.y + blockIdx.y * blockDim.y;
    int i = x + y * blockDim.x * gridDim.x;
    unsigned char mean = (img[i].x + img[i].y + img[i].z)/3;
    img[i].x = img[i].y = img[i].z = mean;
}

