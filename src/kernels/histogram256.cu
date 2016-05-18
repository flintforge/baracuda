
////////////////////////////////////////////////////////////////////////////////
// GPU-specific common definitions
////////////////////////////////////////////////////////////////////////////////
#define uchar unsigned char
#define LOG2_WARP_SIZE 5U
#define WARP_SIZE (1U << LOG2_WARP_SIZE)
#define UINT_BITS 32
#define UCHAR_BITS 8
#define HISTO256_BINS 256
//Warps ==subhistograms per threadblock
#define WARP_COUNT 3
//Threadblock size
#define HISTOGRAM256_THREADBLOCK_SIZE (WARP_COUNT * WARP_SIZE)
//Shared memory per threadblock
#define HISTOGRAM256_THREADBLOCK_MEMORY (WARP_COUNT * HISTO256_BINS)

#define UMUL(a, b) ( (a) * (b) )
#define UMAD(a, b, c) ( UMUL((a), (b)) + (c) )

////////////////////////////////////////////////////////////////////////////
// Shortcut shared memory atomic addition functions
////////////////////////////////////////////////////////////////////////////////
#define USE_SMEM_ATOMICS 0

#if(!USE_SMEM_ATOMICS)
//#define TAG_MASK ( (1U << (UINT_BITS - LOG2_WARP_SIZE)) - 1U )
#define TAG_MASK ( (1U << (UCHAR_BITS - LOG2_WARP_SIZE)) - 1U )

inline __device__ void addByte(volatile uint *s_WarpHist, uchar data, uint threadTag)
{
    uint count;
    do
    {
        count = s_WarpHist[data] & TAG_MASK;
        count = threadTag | (count + 1);
        s_WarpHist[data] = count;
    }
    while (s_WarpHist[data] != count);
}
#else
#ifdef CUDA_NO_SM12_ATOMIC_INTRINSICS
#error Compilation target does not support shared-memory atomics
#endif

#define TAG_MASK 0xFFFFFFFFU
inline __device__ void addByte(uint4 *s_WarpHist, uint data, uint threadTag)
{
    atomicAdd(s_WarpHist + data, 1);
}
#endif

inline __device__ void addWord(uint *s_WarpHist, uchar data, uint tag)
{
    addByte(s_WarpHist, (data >>  0) & 0xFFU, tag);
    addByte(s_WarpHist, (data >>  8) & 0xFFU, tag);
    addByte(s_WarpHist, (data >> 16) & 0xFFU, tag);
    addByte(s_WarpHist, (data >> 24) & 0xFFU, tag);
}

extern "C"
__global__ void histogram256Kernel(uint *d_PartialHistograms, uchar4 *d_Data, uint dataCount)
{
    //Per-warp subhistogram storage
   __shared__ uint s_HistR[HISTOGRAM256_THREADBLOCK_MEMORY];
	__shared__ uint s_HistG[HISTOGRAM256_THREADBLOCK_MEMORY];
	__shared__ uint s_HistB[HISTOGRAM256_THREADBLOCK_MEMORY];
	__shared__ uint s_HistA[HISTOGRAM256_THREADBLOCK_MEMORY];
   uint *s_WarpHistR= s_HistR + (threadIdx.x >> LOG2_WARP_SIZE) * HISTO256_BINS;
	uint *s_WarpHistG= s_HistG + (threadIdx.x >> LOG2_WARP_SIZE) * HISTO256_BINS;
	uint *s_WarpHistB= s_HistB + (threadIdx.x >> LOG2_WARP_SIZE) * HISTO256_BINS;
	uint *s_WarpHistA= s_HistA + (threadIdx.x >> LOG2_WARP_SIZE) * HISTO256_BINS;

    //Clear shared memory storage for current threadblock before processing
#pragma unroll

    for (uint i = 0; i < (HISTOGRAM256_THREADBLOCK_MEMORY / HISTOGRAM256_THREADBLOCK_SIZE); i++)
    {
      s_HistR[threadIdx.x + i * HISTOGRAM256_THREADBLOCK_SIZE] = 0;
		s_HistG[threadIdx.x + i * HISTOGRAM256_THREADBLOCK_SIZE] = 0;
		s_HistB[threadIdx.x + i * HISTOGRAM256_THREADBLOCK_SIZE] = 0;
		s_HistA[threadIdx.x + i * HISTOGRAM256_THREADBLOCK_SIZE] = 0;
    }

    //Cycle through the entire data set, update subhistograms for each warp
    const uint tag = threadIdx.x << (UCHAR_BITS - LOG2_WARP_SIZE);

    __syncthreads();

    for (uint pos = UMAD(blockIdx.x, blockDim.x, threadIdx.x); pos < dataCount; pos += UMUL(blockDim.x, gridDim.x))
    {
        uchar dataR = d_Data[pos].x;
		uchar dataG = d_Data[pos].y;
		uchar dataB = d_Data[pos].z;
		uchar dataA = d_Data[pos].w;
        addWord(s_WarpHistR, dataR, tag);
		addWord(s_WarpHistG, dataG, tag);
		addWord(s_WarpHistB, dataB, tag);
		addWord(s_WarpHistA, dataA, tag);
    }

    //Merge per-warp histograms into per-block and write to global memory
    __syncthreads();

    for (uint bin = threadIdx.x; bin < HISTO256_BINS; bin += HISTOGRAM256_THREADBLOCK_SIZE)
    {
        uint sumR = 0;
		uint sumG = 0;
		uint sumB = 0;
		uint sumA = 0;

        for (uint i = 0; i < WARP_COUNT; i++)
        {
			sumR += s_HistR[bin + i * HISTO256_BINS] & TAG_MASK;
			sumG += s_HistG[bin + i * HISTO256_BINS] & TAG_MASK;
			sumB += s_HistB[bin + i * HISTO256_BINS] & TAG_MASK;
			sumA += s_HistA[bin + i * HISTO256_BINS] & TAG_MASK;
        }

        d_PartialHistograms[blockIdx.x * HISTO256_BINS + bin] = sumR;
		d_PartialHistograms[blockIdx.x * HISTO256_BINS + bin] = sumG;
		d_PartialHistograms[blockIdx.x * HISTO256_BINS + bin] = sumB;
		d_PartialHistograms[blockIdx.x * HISTO256_BINS + bin] = sumA;
    }
}
