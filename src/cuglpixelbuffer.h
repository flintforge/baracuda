/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * a GL/CUDA pixel buffer
 * simply maps a QGLBuffer,
 * to a CudaResource
 * let the renderer calls the kernels
 */

#ifndef CUGLPIXELBUFFER_H
#define CUGLPIXELBUFFER_H

#include <QtOpenGL/QGLBuffer>
#define CUDA_INIT_OPENGL
#include "cuda_drvapi_dynlink.h"
#include "helper_cuda_drvapi.h"

class GLCUPixelBuffer
{
public:
    GLCUPixelBuffer(int,int);
    ~GLCUPixelBuffer();

    void init(int w, int h);
    void resize(int w, int h);
    void bind();
    void release();
    CUgraphicsResource getCudaResource() { return cudaResource; }

/*
 * no need for that.*/
 void registerBuffer() {
        CUDCHK ( cuGraphicsGLRegisterBuffer(&cudaResource, pixelBuffer->bufferId(),
                                        CU_GRAPHICS_REGISTER_FLAGS_NONE //CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD
                                        ) );
    }//*/

    int W() { return width; }
    int H() { return height; }

    CUdeviceptr devptr; // reserved device pointer for resource mapping

  private:
    void deleteTexture();

  private:
    int width;
    int height;
    int timing;
    QGLBuffer*   pixelBuffer;
    GLuint*      textureID;
    CUgraphicsResource cudaResource;

};

#endif // CUGLPIXELBUFFER_H
