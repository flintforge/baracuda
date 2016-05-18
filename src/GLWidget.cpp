/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */
﻿/**
  replaced with the glrendererwidget
  */
//#include <GL/glew.h>

#include <QString>
#include <QMessageBox>
#include "GLWidget.h"
#include <QtOpenGL/QGLWidget>
//-----------------------------------------------------------------------------
// GLWidget
//-----------------------------------------------------------------------------

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  strFrames = "Frames: 0";
  frames    = 0;
  tiupdate  = new QTimer( this );
  tifps     = new QTimer( this );
  connect ( tiupdate, SIGNAL( timeout() ), this, SLOT ( update() ) );
  connect ( tifps,    SIGNAL( timeout() ), this, SLOT ( framesCount() ) );
  tifps->start(1000);
  timeElapse.start();
  glError = GL_NO_ERROR;

  //spbo.kernel1=launch_kernel;
}
//-----------------------------------------------------------------------------
// GLWidget
//-----------------------------------------------------------------------------
GLWidget::~GLWidget()
{
  delete tiupdate;
  delete tifps;
}
//-----------------------------------------------------------------------------
// initializeGL
//-----------------------------------------------------------------------------
void GLWidget::initializeGL()
{
  /*glewInit();
  if (glewIsSupported("GL_VERSION_2_0"))
    printf("Ready for OpenGL 2.0\n");
  else {
    printf("OpenGL 2.0 not supported\n");
  }*/
  running = true;
  spbo.initCuda();
  spbo.initPBO(width(), height());
  glClearColor(0, 0, 0, 1);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  startUpdateTimer();
}
//-----------------------------------------------------------------------------
// resizeGL
//-----------------------------------------------------------------------------
void GLWidget::resizeGL(int w, int h){
  glViewport(0, 0, w, h);
  glEnable(GL_TEXTURE_2D);
  spbo.resize(w, h);
  glMatrixMode(GL_PROJECTION); //Select The Projection Matrix
  glLoadIdentity(); //Reset The Projection Matrix

  glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
  glLoadIdentity(); //Reset The Modelview Matrix
}
//-----------------------------------------------------------------------------
// paintGL
//-----------------------------------------------------------------------------
void GLWidget::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  // run CUDA kernel
  spbo.runCuda( timeElapse.elapsed() );

  //spbo.runKernel( spbo.kernel1, timeElapse.elapsed() );

  // now bind buffer after cuda is done
  spbo.bind();

  // Draw a single Quad with texture coordinates for each vertex.
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);  glVertex3f(-1.0f,-1.0f,-1.0f);
    glTexCoord2f(0.0f,0.0f);  glVertex3f(-1.0f,1.0f,-1.0f);
    glTexCoord2f(1.0f,0.0f);  glVertex3f(1.0f,1.0f,-1.0f);
    glTexCoord2f(1.0f,1.0f);  glVertex3f(1.0f,-1.0f,-1.0f);
  glEnd();

  spbo.release();

  glColor3f(0,1,0);
  renderText(10,10, strFrames);
  glColor3f(1,1,1);

  ++frames;
  
  glError = glGetError();
}
//-----------------------------------------------------------------------------
// startUpdateTimer
//-----------------------------------------------------------------------------
void GLWidget::startUpdateTimer()
{
  if( tiupdate->isActive() == false ){
    timeElapse.restart();
    tiupdate->start(0);
  }
}
//-----------------------------------------------------------------------------
// stopUpdateTimer
//-----------------------------------------------------------------------------
void GLWidget::stopUpdateTimer()
{
  tiupdate->stop();
  timeElapse.restart();
}
//-----------------------------------------------------------------------------
// framesCount
//-----------------------------------------------------------------------------
void GLWidget::framesCount(){
  strFrames.setNum(frames);
  strFrames.prepend(tr("Frames: "));
  frames=0;
}
//-----------------------------------------------------------------------------
// update - SLOT Timer tiupdate
//-----------------------------------------------------------------------------
void GLWidget::update(){

  if( glError != GL_NO_ERROR ){ // OpenGL ocurred
    stopUpdateTimer();
    
    QMessageBox::warning(this, "OpenGL Error: "+QString::number(glError), getGLError());
    glError=GL_NO_ERROR;
  }else{
    if(running)
        updateGL();
  }
}
//-----------------------------------------------------------------------------
// getGLError
//-----------------------------------------------------------------------------
QString GLWidget::getGLError(){
  QString exception = "No error";
  switch (glError)
  {
    // see macro on top
    GLERROR(GL_INVALID_ENUM)
    GLERROR(GL_INVALID_VALUE)
    GLERROR(GL_INVALID_OPERATION)
    GLERROR(GL_STACK_OVERFLOW)
    GLERROR(GL_STACK_UNDERFLOW)
    GLERROR(GL_OUT_OF_MEMORY)
#ifdef GL_INVALID_INDEX
    GLERROR(GL_INVALID_INDEX)
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION_EXT
    GLERROR(GL_INVALID_FRAMEBUFFER_OPERATION_EXT)
#endif
    default:
      exception.sprintf("Unknown GL error: %04x\n", glError);
      break;
  }
  return exception;
}
