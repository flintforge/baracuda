/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
﻿/******************************************************************************
** Class SimplePBO for encapsulating pixel buffer and texturing.
*******************************************************************************/
#ifndef __SIMPLEPBO_H
#define __SIMPLEPBO_H

#include <QtOpenGL/QGLBuffer>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

//#include <helper_timer.h>


typedef void (*KERNEL_FUNC) (cudaGraphicsResource *, uint, uint, int);

// Implementation by kernelPBO.cu (compiled with nvcc)
extern "C" void launch_kernel(cudaGraphicsResource*, unsigned int, unsigned int, int);

/**
 * SimplePBO encapsulates pixel buffer object and 2d texturing.
 * It runs cuda and launches the kernel as well.
 * Reference: http://drdobbs.com/cpp/222600097
 * (took source and adapted it to this class)
 */
class SimplePBO
{
  public:
    SimplePBO(void);
    ~SimplePBO(void);

    void initCuda();
    void initPBO(int w, int h);
    void resize(int w, int h);
    void bind();
    void release();
    void runCuda(int time);
    void cleanupCuda();

    void runKernel(KERNEL_FUNC kernel, int time);
    KERNEL_FUNC kernel1;

  private:
    void createPBO();
    void deletePBO();
    void createTexture();
    void deleteTexture();

  private:

    unsigned int image_width;
    unsigned int image_height;
    QGLBuffer*   pixelBuffer;
    GLuint*      textureID;
    cudaGraphicsResource* cudaResource;
    //StopWatchInterface *hTimer;
};

#endif
