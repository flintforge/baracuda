__global__ void histo_kernel( uchar4 *buffer,
                               uint4 *partialHisto )
{
     __shared__ uint4 temp[256]; // 256*8 = 4096 = 4 blocks allowed
	  //256*3 channel = 3 blocks allowed
     temp[threadIdx.x] = 0; //?
     __syncthreads();

     int i = threadIdx.x + blockIdx.x * blockDim.x;
     int offset = i + blockDim.x * gridDim.x;
     //i += offset;
	 atomicAdd( &temp[buffer[offset].x].x, 1);
	 atomicAdd( &temp[buffer[offset].y].y, 1);
	 atomicAdd( &temp[buffer[offset].z].z, 1);
	 atomicAdd( &temp[buffer[offset].w].w, 1);
     
     __syncthreads();

    atomicAdd( &(histo[threadIdx.x]), temp[threadIdx.x] );
}

