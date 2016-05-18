/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
﻿/******************************************************************************
** A cuda pixel buffer object holding
** every cuda driver api aspect
*  device initialization, glbuffer bindings, kernel launch
*
*******************************************************************************/
#ifndef __CUDA_PBO_H
#define __CUDA_BO_H

#include <QtOpenGL/QGLBuffer>
#define CUDA_INIT_OPENGL
#include "cuda_drvapi_dynlink.h"
#include "helper_cuda_drvapi.h"
#include "tiTimer.h"

extern "C" bool cudaLock;
typedef void (*CU_KERNEL_FUNC) (CUgraphicsResource *, uint, uint, int);

// Implementation by kernelPBO.cu (compiled with nvcc)
extern "C" void cu_launch_kernel(CUgraphicsResource*, unsigned int, unsigned int, int);

/**
 * CudaPBO encapsulates pixel buffer object and 2d texturing.
 * It runs cuda and launches the kernel as well.
 * Reference: http://drdobbs.com/cpp/222600097
 * (took source and adapted it to this class)
 */
class CudaPBO
{
  public:
    CudaPBO(void);
    ~CudaPBO(void);

    cudaError_enum initCuda();
    void initPBO(int w, int h);
    void resize(int w, int h);
    void bind();
    void release();
    void runCuda(int time);
    void cleanupCuda();
    void loadKernel(char * kernel);

  private:
    void createPBO();
    void deletePBO();
    void createTexture();
    void deleteTexture();

  private:
    TiTimer * timer;
    unsigned int image_width;
    unsigned int image_height;
    int timing;
    QGLBuffer*   pixelBuffer;
    GLuint*      textureID;
    CUgraphicsResource cudaResource;

    CUfunction kernel1;
    CUresult status;
    CUdevice cuDevice;
    CUmodule cuModule;
    CUfunction cuFunction;
    CUdeviceptr pos;

    CUcontext g_cuContext;
    int block_size;
};

#endif
