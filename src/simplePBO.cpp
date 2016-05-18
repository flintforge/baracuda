/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
﻿#include "simplePBO.h"

#include <cuda_runtime.h>
//#include <helper_cuda.h>

#define checkCudaErrors(x) x

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SimplePBO::SimplePBO(void)
  : image_width(512),
    image_height(512),
    pixelBuffer(0),
    textureID(0),
    cudaResource(0)
{

}
//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
SimplePBO::~SimplePBO(void)
{
  cleanupCuda();
}
//-----------------------------------------------------------------------------
// initCuda
//-----------------------------------------------------------------------------
void SimplePBO::initCuda()
{
  checkCudaErrors( cudaSetDevice(0) );
  checkCudaErrors( cudaGLSetGLDevice(0) );
  //sdkCreateTimer(&hTimer);
}
//-----------------------------------------------------------------------------
// resize
//-----------------------------------------------------------------------------
void SimplePBO::resize(int w, int h)
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
}
//-----------------------------------------------------------------------------
// runCuda -  Run the Cuda part of the computation
//-----------------------------------------------------------------------------
void SimplePBO::runKernel(KERNEL_FUNC kernel, int time)
{
  Q_ASSERT(pixelBuffer);
  //checkCudaErrors( cudaGLMapBufferObject((void**)&dptr, pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0

  // execute the kernel
  kernel(cudaResource, image_width, image_height, time);

  // unmap buffer object
  //checkCudaErrors( cudaGLUnmapBufferObject(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
}

void SimplePBO::runCuda(int time)
{
  Q_ASSERT(pixelBuffer);
  //checkCudaErrors( cudaGLMapBufferObject((void**)&dptr, pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0  

  // execute the kernel
  launch_kernel(cudaResource, image_width, image_height, time);

  // unmap buffer object
  //checkCudaErrors( cudaGLUnmapBufferObject(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
}
//-----------------------------------------------------------------------------
// createPBO
//-----------------------------------------------------------------------------
void SimplePBO::createPBO()
{
  Q_ASSERT(pixelBuffer==0);
  // set up vertex data parameter
  int size_tex_data = sizeof(GLubyte) * image_width * image_height * 4;  

  pixelBuffer = new QGLBuffer(QGLBuffer::PixelUnpackBuffer);
  pixelBuffer->setUsagePattern(QGLBuffer::StreamDraw);
  pixelBuffer->create();

  pixelBuffer->bind();
  pixelBuffer->allocate(size_tex_data);
  pixelBuffer->release();
  //checkCudaErrors( cudaGLRegisterBufferObject( pixelBuffer->bufferId() ) ); // deprecated as of cuda 3.0

  checkCudaErrors( cudaGraphicsGLRegisterBuffer(&cudaResource, pixelBuffer->bufferId(), cudaGraphicsRegisterFlagsWriteDiscard) );

}
//-----------------------------------------------------------------------------
// deletePBO
//-----------------------------------------------------------------------------
void SimplePBO::deletePBO()
{
  if (pixelBuffer) {

    // unregister this buffer object with CUDA
    checkCudaErrors( cudaGLUnregisterBufferObject(pixelBuffer->bufferId()) ); // deprecated as of cuda 3.0
    pixelBuffer->destroy();
    delete pixelBuffer;
    pixelBuffer = 0;
  }
}
//-----------------------------------------------------------------------------
// createTexture
//-----------------------------------------------------------------------------
void SimplePBO::createTexture()
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
void SimplePBO::deleteTexture()
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
void SimplePBO::bind()
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
void SimplePBO::release()
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
void SimplePBO::cleanupCuda()
{

  if(cudaResource)
  {
    checkCudaErrors( cudaGraphicsUnregisterResource(cudaResource) );
    cudaResource = 0;
  }
  checkCudaErrors( cudaDeviceReset() );
  deleteTexture();
  deletePBO();
}
//-----------------------------------------------------------------------------
// initPBO
//-----------------------------------------------------------------------------
void SimplePBO::initPBO(int w, int h)
{
  //sdkResetTimer(&hTimer);
  //sdkStartTimer(&hTimer);
  image_width = w+16-(w%16);
  image_height = h+16-(h%16);
  createPBO();
  createTexture();

  //sdkStopTimer(&hTimer);
  //printf("Initialization time: %f msec\n", sdkGetTimerValue(&hTimer));
}
