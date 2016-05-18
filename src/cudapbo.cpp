/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
﻿
#include "cudapbo.h"
#include "neutral_ptxdump.h"
#include "timedtest_ptxdump.h"
#include <QDebug>

#define CUDCHK checkCudaErrors

// replace this mess with a mutex
bool cudaLock=0;
#define cuda_lock(X) cudaLock = true;
#define cuda_unlock(X) cudaLock = false;
// ok stop this mess :
/*
write an other class to lock
mutex.lock( module, 3 )
// wait for 3 second or surrender and get out of the function
*/
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CudaPBO::CudaPBO(void)
  : image_width(512),
    image_height(512),
    pixelBuffer(0),
    textureID(0),
    cudaResource(0)
{
    timer = new TiTimer();
}
//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CudaPBO::~CudaPBO(void)
{
  cleanupCuda();
}
//-----------------------------------------------------------------------------
// initCuda
//-----------------------------------------------------------------------------
cudaError_enum CudaPBO::initCuda()
{
    int major, minor, devID = 0; //select first and only device
    char deviceName[256];

    // link to cuda driver dynamically
    status = cuInit(0, __CUDA_API_VERSION);

    if (CUDA_SUCCESS != status)
    {
     fprintf(stderr, "Fatal error dynamically loading the CUDA driver\n");
     fprintf(stderr, "cuInit(0), returned %d\n-> %s\n", status, getCudaDrvErrorString(status));
     exit(EXIT_FAILURE);
    }

    // get cuda-capable device count
    int deviceCount = 0;
    CUDCHK( cuDeviceGetCount(&deviceCount) );

    if (deviceCount == 0)
    {
        fprintf(stderr, "No devices supporting CUDA detected, exiting...\n");
        exit(EXIT_SUCCESS);
    }

    if (devID < 0) devID = 0;

    if (devID > deviceCount -1)
    {
        fprintf(stderr, "initCUDA (Device=%d) invalid GPU device.  %d GPU device(s) detected.\n\n", devID, deviceCount);
        status = CUDA_ERROR_NOT_FOUND;

        cuCtxDestroy(g_cuContext);
        exit(EXIT_FAILURE);
    }

    // pick up device with zero ordinal (default, or devID)
    CUDCHK( cuDeviceGet(&cuDevice, devID) );

    // get compute capabilities and the devicename
    CUDCHK( cuDeviceComputeCapability(&major, &minor, cuDevice) );
    CUDCHK( cuDeviceGetName(deviceName, 256, cuDevice) );
    // use a larger block size for Fermi and above
    block_size = ( (major < 2) ? 16 : 32 );
    printf("> Device %d: \"%s\" with Compute %d.%d capability. CUDA v%i (bs=%i)\n",
           cuDevice, deviceName, major, minor,__CUDA_API_VERSION, block_size);


    // create context for picked device
    status = cuCtxCreate(&g_cuContext, 0, cuDevice);

    if (CUDA_SUCCESS != status)
    {
       cuCtxDestroy(g_cuContext);
       exit(EXIT_SUCCESS);
    }


    // setup JIT compilation options and perform compilation
    {
       // in this branch we use compilation with parameters
       const unsigned int jitNumOptions = 3;
       CUjit_option *jitOptions = new CUjit_option[jitNumOptions];
       void **jitOptVals = new void*[jitNumOptions];

       // set up size of compilation log buffer
       jitOptions[0] = CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES;
       int jitLogBufferSize = 1024;
       jitOptVals[0] = (void *)(size_t)jitLogBufferSize;

       // set up pointer to the compilation log buffer
       jitOptions[1] = CU_JIT_INFO_LOG_BUFFER;
       char *jitLogBuffer = new char[jitLogBufferSize];
       jitOptVals[1] = jitLogBuffer;

       // set up pointer to set the Maximum # of registers for a particular kernel
       jitOptions[2] = CU_JIT_MAX_REGISTERS;
       int jitRegCount = 32;
       jitOptVals[2] = (void *)(size_t)jitRegCount;

       // compile with set parameters
       printf("> Compiling PTX module\n");

    #if defined(__x86_64) || defined(AMD64) || defined(_M_AMD64)
       CUDCHK( cuModuleLoadDataEx(&cuModule, timedtest_ptxdump, jitNumOptions, jitOptions, (void **)jitOptVals) );
       
   // matrixmul 64 ?
      // status = cuModuleLoadDataEx(&cuModule, matrixMul_kernel_64_ptxdump, jitNumOptions, jitOptions, (void **)jitOptVals);
    #else
       //status = cuModuleLoadDataEx(&cuModule, neutral_ptxdump, jitNumOptions, jitOptions, (void **)jitOptVals);
       CUDCHK( cuModuleLoadDataEx(&cuModule, timedtest_ptxdump, jitNumOptions, jitOptions, (void **)jitOptVals) );
       // this does not compile the module
       //CUDCHK( cuModuleLoad(&cuModule, "blue.ptx"));
    #endif

       printf("> PTX JIT log:\n%s\n", jitLogBuffer);

       delete [] jitOptions;
       delete [] jitOptVals;
       delete [] jitLogBuffer;
    }

    if (CUDA_SUCCESS != status)
    {
       printf("Error while compiling PTX\n");
       cuCtxDestroy(g_cuContext);
       exit(EXIT_FAILURE);
    }

    // retrieve CUDA function from the compiled module
    status = cuModuleGetFunction( &kernel1 , cuModule,"timedtest");
                                //( block_size == 16) ? "_Z3redP6uchar4jj" : "mandelbrot_32bit" );

    // now how the hell do i keep a proper .entry name for my function ?
    // answer : __extern C to avoid name mangling

    if (CUDA_SUCCESS != status)
    {
       printf("The lookup failed ! \n");
       cuCtxDestroy(g_cuContext);
       exit(EXIT_FAILURE);
    }

    return CUDA_SUCCESS;
}


//-----------------------------------------------------------------------------
// load kernel
//-----------------------------------------------------------------------------
void CudaPBO::loadKernel(char * kernel) {
    while(cudaLock) {} // for safety, just set a timer of a few seconds before give up

}

//-----------------------------------------------------------------------------
// resize
//-----------------------------------------------------------------------------
void CudaPBO::resize(int w, int h)
{
  Q_ASSERT(pixelBuffer);
  Q_ASSERT(textureID[0]);
  // sizes must be a multiple of 16
  image_width = w+16-(w%16);
  image_height = h+16-(h%16);

  int size_tex_data = sizeof(GLubyte) * image_width * image_height * 4;
  pixelBuffer->bind();
  pixelBuffer->allocate(size_tex_data);
  pixelBuffer->release();
  glBindTexture( GL_TEXTURE_2D, textureID[0]);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0,
      GL_BGRA,GL_UNSIGNED_BYTE, NULL);
  glBindTexture( GL_TEXTURE_2D, 0);
  qDebug()<< "resized " << image_width << " x " << image_height << "   :"<< w << "// "<<w%16;
}
//-----------------------------------------------------------------------------
// runCuda -  Run the Cuda part of the computation
//-----------------------------------------------------------------------------
/*
void CudaPBO::runKernel(KERNEL_FUNC kernel, int time)
{
  Q_ASSERT(pixelBuffer);
  //checkCudaErrors( cudaGLMapBufferObject((void**)&dptr, pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0

  // execute the kernel
  kernel(cudaResource, image_width, image_height, time);

  // unmap buffer object
  //checkCudaErrors( cudaGLUnmapBufferObject(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
}
*/

void CudaPBO::runCuda(int time) // -> make it long and msec
{
    timing=time;
    Q_ASSERT(pixelBuffer);

    cuda_lock();
    //CUDCHK( cuGLMapBufferObject(&pos, &num_bytes, pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
  //  CUDCHK ( cuGraphicsGLRegisterBuffer(&cudaResource, pixelBuffer->bufferId(), CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD  ) );
    // execute the kernel
    if( cuGraphicsMapResources(1, &cudaResource, 0)==CUDA_SUCCESS)
    {
        size_t  num_bytes;
        CUDCHK ( cuGraphicsResourceGetMappedPointer(&pos, &num_bytes, cudaResource) );
        //@Opt eventually no need to recollect the pointers on every frame...
        void *args[3] = { &pos, &image_width, &image_height };
        #if __CUDA_API_VERSION >= 6000 // sigsev here...why?
        {
                CUDCHK ( cuLaunchKernel(kernel1,
                                           image_width/16, image_width/16, 1, //grid
                                           16, 16, 1,  //block
                                           0,
                                           NULL, args, NULL));
        //*/
        }
        #else
        {
               // This is the older CUDA Driver API for Kernel Parameter passing and Kernel Launching
               int offset = 0;
               checkCudaErrors(cuParamSetv(kernel1, offset, &pos, sizeof(pos)));
               offset += sizeof(pos);
               checkCudaErrors(cuParamSeti(kernel1, offset, image_width));
               offset += sizeof(&image_width);
               checkCudaErrors(cuParamSeti(kernel1, offset, image_height));
               offset += sizeof(&image_height);
               checkCudaErrors(cuParamSeti(kernel1, offset, time));
               offset += sizeof(&(time));

               checkCudaErrors(cuParamSetSize(kernel1, offset));

               checkCudaErrors(cuFuncSetBlockShape(kernel1, 16, 16, 1));
               checkCudaErrors(cuFuncSetSharedSize(kernel1, 2*block_size*block_size*sizeof(uint)));

               // set execution configuration for the CUDA kernel
               checkCudaErrors(cuLaunchGrid(kernel1, image_width / 16, image_height / 16));
        }
        #endif
    }

    checkCudaErrors(cuCtxSynchronize());
    CUDCHK ( cuGraphicsUnmapResources(1, &cudaResource, 0) ) ;

    cuda_unlock();
    // unmap buffer object
    // checkCudaErrors( cudaGLUnmapBufferObject(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
}
//-----------------------------------------------------------------------------
// createPBO
//-----------------------------------------------------------------------------
void CudaPBO::createPBO()
{
  Q_ASSERT(pixelBuffer==0);
  // set up vertex data parameter
  int size_tex_data = sizeof(GLubyte) * image_width * image_height * 4;

  /*
   * there will be in and out here, so take care to the usage pattern
   * the blockop we'll be pointing will show desired level of op on the framebuffer
   * side note : should we be working on big data, we'll have to downsample this
   * before restitution
   */
  pixelBuffer = new QGLBuffer(QGLBuffer::PixelUnpackBuffer);
  pixelBuffer->setUsagePattern(QGLBuffer::StreamDraw);
  pixelBuffer->create();

  pixelBuffer->bind();
  pixelBuffer->allocate(size_tex_data);
  pixelBuffer->release();
  //checkCudaErrors( cudaGLRegisterBufferObject( pixelBuffer->bufferId() ) ); // deprecated as of cuda 3.0

  CUDCHK ( cuGraphicsGLRegisterBuffer(&cudaResource, pixelBuffer->bufferId(),
                                      CU_GRAPHICS_REGISTER_FLAGS_NONE
                                      //CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD
                                      ) );
                                     //cudaGraphicsRegisterFlagsWriteDiscard) );
}

//-----------------------------------------------------------------------------
// deletePBO
//-----------------------------------------------------------------------------
void CudaPBO::deletePBO()
{
  if (pixelBuffer) {

    // unregister this buffer object with CUDA
    //checkCudaErrors( cuGraphicsUnregisterResource(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
    // checkCudaErrors( cuGraphicsUnregisterResource( cudaResource ) );
    pixelBuffer->destroy();
    delete pixelBuffer;
    pixelBuffer = 0;
  }
}
//-----------------------------------------------------------------------------
// createTexture
//-----------------------------------------------------------------------------
void CudaPBO::createTexture()
{
  // delete texture object if necessary
  //  for reallocating tex mem, e.g. at different size
  deleteTexture();

  // Generate a texture identifier
  textureID = new GLuint[1]; // increase if u need more
  glGenTextures(1, textureID);

  // Make this the current texture (remember that GL is state-based)
  glBindTexture( GL_TEXTURE_2D, textureID[0]);

  // Allocate the texture memory. The last parameter is NULL since we only
  // want to allocate memory, not initialize it
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0,
            GL_BGRA,GL_UNSIGNED_BYTE, NULL);

  // Must set the filter mode, GL_LINEAR enables interpolation when scaling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Note: GL_TEXTURE_RECTANGLE_ARB may be used instead of
  // GL_TEXTURE_2D for improved performance if linear interpolation is
  // not desired. Replace GL_LINEAR with GL_NEAREST in the
  // glTexParameteri() call
  glBindTexture(GL_TEXTURE_2D, 0);
}
//-----------------------------------------------------------------------------
// deleteTexture
//-----------------------------------------------------------------------------
void CudaPBO::deleteTexture()
{
  if(textureID){
    glDeleteTextures(1, textureID);
    delete[] textureID;
    textureID = 0;
  }
}
//-----------------------------------------------------------------------------
// bind
//-----------------------------------------------------------------------------
void CudaPBO::bind()
{
  Q_ASSERT(pixelBuffer);
  // Create a texture from the buffer
  pixelBuffer->bind();
  // bind texture from PBO
  glBindTexture(GL_TEXTURE_2D, textureID[0]);

  // Note: glTexSubImage2D will perform a format conversion if the
  // buffer is a different format from the texture. We created the
  // texture with format GL_RGBA8. In glTexSubImage2D we specified
  // GL_BGRA and GL_UNSIGNED_BYTE. This is a fast-path combination

  // Note: NULL indicates the data resides in device memory
  // hence data is coming from PBO
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height,
          GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
//-----------------------------------------------------------------------------
// release
//-----------------------------------------------------------------------------
void CudaPBO::release()
{
  Q_ASSERT(pixelBuffer);
  // deactivate pixelbuffer object
  pixelBuffer->release();
  // deactivate texture object
  glBindTexture(GL_TEXTURE_2D, 0);
}
//-----------------------------------------------------------------------------
// cleanupCuda
//-----------------------------------------------------------------------------
void CudaPBO::cleanupCuda()
{

  if(cudaResource)
  {
    CUDCHK( cuGraphicsUnregisterResource( cudaResource) );
    cudaResource = 0;
  }

  if(g_cuContext)
    checkCudaErrors(cuCtxDestroy(g_cuContext));

  deleteTexture();
  deletePBO();
}
//-----------------------------------------------------------------------------
// initPBO
//-----------------------------------------------------------------------------
void CudaPBO::initPBO(int w, int h)
{
  timer->init();

  image_width = w+16-(w%16);
  image_height = h+16-(h%16);
  createPBO();
  createTexture();

  printf("W=%i H=%i", image_width, image_height);
  printf("Initialization time: %f msec\n", timer->get());
}
