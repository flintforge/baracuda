

// images are 16x16 chunks. dividing by 256 will is integral
// compute as many histograms as shared memory allows
// TODO : consider streaming with very big images.
extern "C" __global__ 
void subhistograms24 ( 
	uchar4 *img,
	uchar4 *hist,
	unsigned int width,
	unsigned int height
	) 
{
	__shared__ uchar4 subhist[256]; // 256*4 = 1024 => 16 blocs max
	float x = threadIdx.x + blockIdx.x * blockDim.x;
	subhist [ threadIdx.x ] = make_uchar4(0,0,0,0);
   __syncthreads(); 
	atomicAdd( &(subhist [ img[i].x ].x) , 1 );
	atomicAdd( &(subhist [ img[i].y ].y) , 1 );
	atomicAdd( &(subhist [ img[i].z ].z) , 1 );
	atomicAdd( &(subhist [ img[i].w ].w) , 1 );
	__syncthreads();
	atomicAdd( &(hist[threadIdx.x]), subhist[threadIdx.x] );
/* 
	hist[i].x = 255-img[i].x;
	hist[i].y = 255-img[i].y;
	hist[i].z = 255-img[i].z;
	 // */
}

