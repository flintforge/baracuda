/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "cuglpixelbuffer.h"
#include "glrendererwidget.h"

class Histogram : public GLRendererWidget
{
Q_OBJECT

private:
    int bins;
    int channels;

    GPUModule * partialHistoModule;
    GPUModule * mergeHistModule;
    GPUModule * kernelModule;
    CUfunction * kernelHistoPartial;
    CUfunction * kernelHistoMerge;
    CUfunction * kernel;
    GLCUPixelBuffer * histoPBO; // output image
    CUdeviceptr dhisto;
    CUdeviceptr dpartialHistograms;
    // parent pbo is the input image, shared with an other rendering context
    static const uint PARTIAL_HISTOGRAM256_COUNT = 240;


public:
    Histogram(QWidget *parent , const QGLWidget * shared);
    ~Histogram();

    void init(GPU * gpu);
    // map the input image to device pointers
    void map ( GLCUPixelBuffer * pbo, GLCUPixelBuffer * hist );
    // run the histogram kernel
    void run ( CUdeviceptr * img , CUdeviceptr * hist, int W, int H );

    virtual void resizeGL(int w, int h);
    virtual void initializeGL();
    virtual void paintGL();

};


// compute histogram
// histkern(data,histo) /on device
// render hist onto the pixelbuffer
// alias on a height of 256 (get maxbin)
// return scale ratio (maxbin/256)
// > use it to put the legend on side
//
// draw the <256*256> image with one block of thread


#endif // HISTOGRAM_H
