/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
#ifndef GPU_H
#define GPU_H

#include <QtOpenGL/QGLBuffer>
#include "baracuda.h"
#define CUDA_INIT_OPENGL
#include "cuda_drvapi_dynlink.h"
#include "helper_cuda_drvapi.h"

class GPU
{
public:
    CUresult status;
    CUdevice device;
    CUcontext context;

    // default module/function check
    CUmodule module;
    CUfunction function;
    CUdeviceptr pos;
    int deviceID;
    static GPU * gpu;

public:
    GPU();
    void startup(int devID=0);//only one device supported for now
    ~GPU();
};

#endif // GPU_H
