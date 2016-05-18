/*
 * Copyright 1993-2013 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#ifndef __cuda_drvapi_dynlink_cuda_gl_h__
#define __cuda_drvapi_dynlink_cuda_gl_h__

// force it
#define CUDA_INIT_OPENGL

#ifdef CUDA_INIT_OPENGL

#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
#ifdef FORCE_USE_GLEW
#include <GL/glew.h>
#endif
*/

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

/************************************
 **
 **    OpenGL Graphics/Interop
 **
 ***********************************/

// OpenGL/CUDA interop (CUDA 2.0+)
typedef CUresult CUDAAPI tcuGLCtxCreate(CUcontext *pCtx, unsigned int Flags, CUdevice device);
typedef CUresult CUDAAPI tcuGraphicsGLRegisterBuffer(CUgraphicsResource *pCudaResource, GLuint buffer, unsigned int Flags);
typedef CUresult CUDAAPI tcuGraphicsGLRegisterImage(CUgraphicsResource *pCudaResource, GLuint image, GLenum target, unsigned int Flags);


extern tcuGraphicsGLRegisterBuffer *cuGraphicsGLRegisterBuffer;
/*
CUresult cuGraphicsGLRegisterBuffer (CUgraphicsResource *pCudaResource, GLuint buffer, unsigned int Flags)
Registers an OpenGL buffer object.
Parameters
pCudaResource - Pointer to the returned object handle
buffer - name of buffer object to be registered
Flags - Register flags

Returns : CUDA_SUCCESS, CUDA_ERROR_INVALID_HANDLE, CUDA_ERROR_ALREADY_MAPPED, CUDA_ERROR_INVALID_CONTEXT,
Registers the buffer object specified by buffer for access by CUDA. A handle to the
registered object is returned as pCudaResource. The register flags Flags specify the
intended usage, as follows:

CU_GRAPHICS_REGISTER_FLAGS_NONE: Specifies no hints about how this
resource will be used. It is therefore assumed that this resource will be read from
and written to by CUDA. This is the default value.
CU_GRAPHICS_REGISTER_FLAGS_READ_ONLY: Specifies that CUDA will not
write to this resource.
CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD: Specifies that CUDA will
not read from this resource and will write over the entire contents of the resource, so
none of the data previously stored in the resource will be preserved.
Note that this function may also return error codes from previous, asynchronous
launches.
*/

#ifdef _WIN32
#include <GL/wglext.h>
// WIN32
typedef CUresult CUDAAPI tcuWGLGetDevice(CUdevice *pDevice, HGPUNV hGpu);
#endif

#endif // CUDA_INIT_OPENGL

#endif // __cuda_drvapi_dynlink_cuda_gl_h__

