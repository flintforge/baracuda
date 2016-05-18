/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


  This is the near complete real time per frame histogram

  inspired from the computing sdk example, there is
  nor more than 20 lines of codes missing to complete this.

  */

#include "histogram.h"
#include "cudacompil.h"
#include "baracuda.h"

#define LOG2_WARP_SIZE 5U
#define WARP_SIZE (1U << LOG2_WARP_SIZE)
//May change on future hardware, so better parametrize the code
#define SHARED_MEMORY_BANKS 16
//Threadblock size: must be a multiple of (4 * SHARED_MEMORY_BANKS)
//because of the bit permutation of threadIdx.x
#define HISTOGRAM64_THREADBLOCK_SIZE (4 * SHARED_MEMORY_BANKS)
//Warps ==subhistograms per threadblock
#define WARP_COUNT 6
//Threadblock size
#define HISTOGRAM256_THREADBLOCK_SIZE (WARP_COUNT * WARP_SIZE)
////////////////////////////////////////////////////////////////////////////////
// Merge histogram256() output
// Run one threadblock per bin; each threadblock adds up the same bin counter
// from every partial histogram. Reads are uncoalesced, but mergeHistogram256
// takes only a fraction of total processing time
////////////////////////////////////////////////////////////////////////////////
#define MERGE_THREADBLOCK_SIZE 256

Histogram::Histogram(QWidget *parent , const QGLWidget * shared)
    : GLRendererWidget( parent, shared )
   ,bins(256)
  ,channels(3)
{
}

Histogram::~Histogram() {
   // cuMemFree(dpartialHistograms);
   // cuMemFree(dhisto);
    //delete histoPBO;
}

void Histogram::init(GPU * gpu) {
    setGPU(gpu);
    CudaCompiler compil;
    partialHistoModule = compil.createGPUModuleFromFile("kernels/histogram256.cu",gpu);
    mergeHistModule = compil.createGPUModuleFromFile("kernels/histo256merge.cu",gpu);
    kernelModule = compil.createGPUModuleFromFile("kernels/invert24.cu",gpu);
    kernelHistoPartial = partialHistoModule->func();
    kernelHistoMerge = mergeHistModule->func();
    kernel = kernelModule->func();
}

void Histogram::map(GLCUPixelBuffer * pbo, GLCUPixelBuffer * hist)
{
    Q_ASSERT(pbo);
    Q_ASSERT(hist);
    // map the resource one an get the handle on the deviceptr
    CUgraphicsResource cugr1 = pbo->getCudaResource();
    CUgraphicsResource cugr2 = hist->getCudaResource();
    // is this mapping necessary on every frame ?
    if( ( cuGraphicsMapResources(1, &cugr1, 0)==CUDA_SUCCESS )
      && ( cuGraphicsMapResources(1, &cugr2, 0)==CUDA_SUCCESS )
            )
    {
        size_t  num_bytes; // check this
        CUDCHK ( cuGraphicsResourceGetMappedPointer( &pbo->devptr, &num_bytes, cugr1 ));
        CUDCHK ( cuGraphicsResourceGetMappedPointer( &hist->devptr, &num_bytes, cugr2 ));
        {
            run(&pbo->devptr , &hist->devptr, pbo->W(), pbo->H() );
        }
    }

    CUDCHK ( cuGraphicsUnmapResources(1, &cugr1, 0) ) ;
    CUDCHK ( cuGraphicsUnmapResources(1, &cugr2, 0) ) ;
}


void Histogram::run( CUdeviceptr * img , CUdeviceptr * hist, int W, int H )
{
    #if __CUDA_API_VERSION >= 4000
/*
    192*144/(16*16*16.0) = 6.75
    >>> 6*(256*16) =     24576
    >>> 192*144-24576=     3072
    >>> 3072/256.0 =     12.0
*/

    void * args1[4] = { img, &hist, &W, &H }; // TODO: automatic check against signature

    CUDCHK ( cuLaunchKernel( * kernel,
                                 16,1, 1, //grid
                                 256, 1, 1,  //block
                                 0, NULL,
                                 args1, NULL ));

    CUDCHK( cuCtxSynchronize());
    // and finally display

    /*CUDCHK ( cuLaunchKernel( *kernel,
                               W/16,H/16, 1, //grid
                               16, 16, 1,  //block
                               0,
                               NULL, args, NULL) );
    V1 ok */
    #else
    ;
    int offset = 0;

    CUDCHK(cuParamSetv(*kernel, offset, (void*) img, sizeof(img))); offset += sizeof(*img);
    CUDCHK(cuParamSetv(*kernel, offset, (void*) hist, sizeof(hist))); offset += sizeof(*hist);
    CUDCHK(cuParamSeti(*kernel, offset, W));    offset += sizeof(&W);
    CUDCHK(cuParamSeti(*kernel, offset, H));    offset += sizeof(&H);

    CUDCHK(cuParamSetSize(*kernel, offset));

    // 16384/(3*256) = 21
    CUDCHK(cuFuncSetBlockShape(*kernel, 256, 1,1));
    CUDCHK(cuFuncSetSharedSize(*kernel, 2*BLOCKSZ));
    // set execution configuration for the CUDA kernel
    CUDCHK(cuLaunchGrid(*kernel, W/16, H/16));
    CUDCHK( cuCtxSynchronize());
    #endif
  
}


void Histogram::initializeGL()
{
  histoPBO = new GLCUPixelBuffer(512,512);
  histoPBO->init(512,512);
  running = true;
  CUDCHK( cuMemAlloc( &dhisto, 256*sizeof(uint)));
  CUDCHK( cuMemAlloc( &dpartialHistograms, PARTIAL_HISTOGRAM256_COUNT * 256 /*bins*/ * sizeof(uint)));
  //startUpdateTimer();
}

void Histogram::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glEnable(GL_TEXTURE_2D);

  histoPBO->resize(w, h);

  glMatrixMode(GL_PROJECTION); //Select The Projection Matrix
  glLoadIdentity(); //Reset The Projection Matrix
  glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
  glLoadIdentity(); //Reset The Modelview Matrix
}


/*
 * the renderpass  **/
void Histogram::paintGL()
{
    map(pbo,histoPBO);

    // no need to clear, we repaint the entire buffer, splash
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    // bind buffer after cuda is done
    pbo->bind();
    // Draw a single Quad with texture coordinates for each vertex.
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);  glVertex3f(-1.0f,-1.0f,0.0f);
        glTexCoord2f(0.0f,0.0f);  glVertex3f(-1.0f, 1.0f,0.0f);
        glTexCoord2f(1.0f,0.0f);  glVertex3f( 1.0f, 1.0f,0.0f);
        glTexCoord2f(1.0f,1.0f);  glVertex3f( 1.0f,-1.0f,0.0f);
    glEnd();
    pbo->release();

    glColor3f(0,1,0);
    renderText(10,10, strFrames);
    glColor3f(1,1,1);

    ++frames;

    glError = glGetError();
}
