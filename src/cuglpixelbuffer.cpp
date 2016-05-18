/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


A CUDA/GL pixel buffer
bound to a 'CudaResource'
working on moduli BLOCKSZ images
(16 or 32 depending on the hardware and processed tiles)
this is our main and only PBO for now
multi/concurent window shall switch the cucontext

*/

#include "cuglpixelbuffer.h"
#include "baracuda.h"
#include <QDebug>

GLCUPixelBuffer::GLCUPixelBuffer(int w, int h)
  : width(w),
    height(h),
    pixelBuffer(0),
    textureID(0),
    cudaResource(0)
{}


GLCUPixelBuffer::~GLCUPixelBuffer(void) {
    // unmap the resource
    if(cudaResource)
    {
      //CUDCHK( cuGraphicsUnregisterResource( cudaResource) );
      cudaResource = 0;
    }
    // take care to destroy the context AFTER this, not before
    deleteTexture();

    if (pixelBuffer) {
      pixelBuffer->destroy();
      delete pixelBuffer;
    }
}


void GLCUPixelBuffer::resize(int w, int h) {
/**
 * called when the window is resized
 * the image is reallocated
 */
  Q_ASSERT(pixelBuffer);
  Q_ASSERT(textureID[0]);
  // sizes are multiple of BLOCKSZ
  width = w+BLOCKSZ-(w%BLOCKSZ);
  height = h+BLOCKSZ-(h%BLOCKSZ);

  int size_tex_data = sizeof(GLubyte) * width * height * 4;
  pixelBuffer->bind();
  pixelBuffer->allocate(size_tex_data); // previous content is removed
  pixelBuffer->release();
  glBindTexture( GL_TEXTURE_2D, textureID[0]);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
      GL_BGRA,GL_UNSIGNED_BYTE, NULL); // notice the bgra has no effect here.
  glBindTexture( GL_TEXTURE_2D, 0);
  qDebug() << "pbo " << this << " resized to " << width << "x"<<height;
}

//-----------------------------------------------------------------------------
// deleteTexture
//-----------------------------------------------------------------------------
void GLCUPixelBuffer::deleteTexture()
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
void GLCUPixelBuffer::bind()
{
  Q_ASSERT(pixelBuffer);
  // Create a texture from the buffer
  pixelBuffer->bind();
  // bind texture from PBO
  glBindTexture(GL_TEXTURE_2D, textureID[0]);

  // Note: glTexSubImage2D will perform a format conversion if the
  // buffer is a different format from the texture.
  // Note: NULL indicates the data resides in device memory
  // hence data is coming from PBO
  //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
}
//-----------------------------------------------------------------------------
// release
//-----------------------------------------------------------------------------
void GLCUPixelBuffer::release()
{
  Q_ASSERT(pixelBuffer);
  // deactivate pixelbuffer object
  pixelBuffer->release();
  // deactivate texture object
  glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * initialization after the GL context is set
 */
void GLCUPixelBuffer::init(int w, int h)
{
  width = w+BLOCKSZ-(w%BLOCKSZ);
  height = h+BLOCKSZ-(h%BLOCKSZ);

  Q_ASSERT(pixelBuffer==0);
  int size_tex_data = sizeof(GLubyte) * width * height * 4;

  /*
   * there will be in and out here,
   * take care to the usage pattern.
   * the blockop we will ll be pointing at
   * will show the desired level of op on the framebuffer
   * side note : working on ultra high resolution,
   * will require a downsampling before rendering
   */
  size_t free, total;
  CUDCHK(cuMemGetInfo(&free,&total)) 	;
  qDebug() << "GPU memory : free="<<free<< " /"<< total;
  pixelBuffer = new QGLBuffer(QGLBuffer::PixelUnpackBuffer);
  pixelBuffer->setUsagePattern(QGLBuffer::StreamDraw);
  pixelBuffer->create();

  pixelBuffer->bind();
  pixelBuffer->allocate(size_tex_data);
  pixelBuffer->release();

  // cudaGLRegisterBufferObject( pixelBuffer->bufferId() ) ); // deprecated as of cuda 3.0
  CUDCHK ( cuGraphicsGLRegisterBuffer(&cudaResource, pixelBuffer->bufferId(),
                                      CU_GRAPHICS_REGISTER_FLAGS_NONE
                                      //CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD
                                      ) );
                              //cudaGraphicsRegisterFlagsWriteDiscard) );

  /*
   * create textures
   * delete texture object if necessary
   * for reallocating tex mem, e.g. at different size
   */
  deleteTexture();
  textureID = new GLuint[1]; // Generate a texture id
  glGenTextures(1, textureID);
  glBindTexture( GL_TEXTURE_2D, textureID[0]);
  // Allocate the texture memory. The last parameter is NULL since we only
  // want to allocate memory, not initialize it
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
            GL_BGRA,GL_UNSIGNED_BYTE, NULL);

  // Must set the filter mode, GL_LINEAR enables interpolation when scaling
  /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

  /* Note: GL_TEXTURE_RECTANGLE_ARB may be used instead of
   GL_TEXTURE_2D for improved performance if linear interpolation is
   not desired. Replace GL_LINEAR with GL_NEAREST in the
   glTexParameteri() call
  */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  //CUDCHK(cuMemGetInfo(&free,&total)) 	;
  //qDebug() << "memory : free="<<free<< " /"<< total;
}
