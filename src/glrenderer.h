/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016-05-17 16:25:47>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef GLRENDERERWIDGET_H
#define GLRENDERERWIDGET_H

#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include "op.h"
#include "cuglpixelbuffer.h"
#include "gpu.h"
/**
 *
 */
class GLLitlRendererWidget : public QGLWidget
{
  Q_OBJECT

  private:
    GPU * gpu;

  public:
    GLLitlRendererWidget(QWidget *parent);
    ~GLLitlRendererWidget();

    // on which gpu is the rendering context activated
    void setGPU(GPU * gpu) { this->gpu = gpu; }

    void setPixelBuffer(GLCUPixelBuffer * pbo);
    void setOp(OP * op); // list of op to execute on a renderpass

    /**
     * Starts Update Timer.
     * frame rate may be capped by drivers around 60fps. */
    void startUpdateTimer();
    void stopUpdateTimer();

//-----------------------------------------------------------------------------
  public slots:
    /**
     * Update scene by calling updateGL().
     * If OpenGL Error ocurred, show it and don't update (stopUpdateTimer).
     */
    void update();

  private slots:
    /**
     * Count frames and create debug string (strFrames). Reset frame counter.
     */
    void framesCount();

//-----------------------------------------------------------------------------
  private:
    /**
     * Initialize context, check for OpenGL driver and start Update Timer.
     */
    void initializeGL();

    /**
     * Resize OpenGL frame.
     * @param w Window width
     * @param h Window height
     */
    void resizeGL(int w, int h);

    /**
     * Paint OpenGL Frame.
     */
    void paintGL();

    /**
     * @return OpenGL error enum as QString.
     */
    QString getGLError();

  private:
    GLCUPixelBuffer * pbo;
    OP * op;

    unsigned int frames;    /// frame counter
    GLenum  glError;        /// error opengl enum type
    QTime   timeElapse;     /// time object for measuring elapsed times
    QTime   opTime;         /// time object for measuring ops execution time
    int     opTimelaps;     // msec
    QTimer* tiupdate;       /// timer triggering frame updating (\see update())
    QTimer* tifps;          /// 1sec or 1/2 sec timer for counting frames
    QString strFrames;      /// string holding frames debug text
    QString strOpTime;      /// string holding ops execution time



public:
     // may need to lock rendering before swithing opcontext
     bool running;
};

#endif // GLRENDERERWIDGET_H
