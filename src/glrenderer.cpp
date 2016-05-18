/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


alternative renderer

*/

#include <QString>
#include <QMessageBox>
#include <QtOpenGL/QGLWidget>
#include "glrenderer.h"


GLLitlRendererWidget::GLLitlRendererWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    ,op(0)
    ,frames(0)
    ,opTimelaps(0)
{
  strFrames = "Frames: 0";
  tiupdate  = new QTimer( this );
  tifps     = new QTimer( this );
  connect ( tiupdate, SIGNAL( timeout() ), this, SLOT ( update() ) );
  connect ( tifps,    SIGNAL( timeout() ), this, SLOT ( framesCount() ) );
  tifps->start(1000);
  timeElapse.start();
  glError = GL_NO_ERROR;
}


GLLitlRendererWidget::~GLLitlRendererWidget()
{
  delete tiupdate;
  delete tifps;
}


void GLLitlRendererWidget::initializeGL()
{
  glClearColor(0, 0, 0, 1);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  startUpdateTimer();

  pbo = new GLCUPixelBuffer(512,512);
  pbo->init(512,512);
  running = true;
}


void GLLitlRendererWidget::setPixelBuffer(GLCUPixelBuffer * pbo) {
    this->pbo = pbo;
}


void GLLitlRendererWidget::setOp(OP * op)
{
    this->op = op;
}


void GLLitlRendererWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glEnable(GL_TEXTURE_2D);

  pbo->resize(w, h);

  glMatrixMode(GL_PROJECTION); //Select The Projection Matrix
  glLoadIdentity(); //Reset The Projection Matrix
  glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
  glLoadIdentity(); //Reset The Modelview Matrix
}

/*
 * the renderpass  **/
void GLLitlRendererWidget::paintGL()
{
    if(!running) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    if(op!=NULL) {
        /*if(gpu!=0) {
            CUDCHK( cuCtxPushCurrent(gpu->context) );
        }*/
        opTime.restart();
        op->launch(pbo, timeElapse.elapsed());
        opTimelaps = opTime.elapsed();
    }
    // bind buffer after cuda is done
    pbo->bind();

    // Draw a single Quad with texture coordinates for each vertex.
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);  glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f);  glVertex3f(-1.0f, 1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f);  glVertex3f( 1.0f, 1.0f,-1.0f);
        glTexCoord2f(1.0f,1.0f);  glVertex3f( 1.0f,-1.0f,-1.0f);
    glEnd();

    pbo->release();

    glColor3f(0,1,0);
    renderText(10,10, strFrames);
    glColor3f(1,1,1);

    ++frames;

    glError = glGetError();

    //if(gpu!=0 && op!=NULL) CUDCHK( cuCtxPopCurrent(0) );
}

//-----------------------------------------------------------------------------
// startUpdateTimer
//-----------------------------------------------------------------------------
void GLLitlRendererWidget::startUpdateTimer()
{
  if( tiupdate->isActive() == false )
  {
    timeElapse.restart();
    tiupdate->start(0);
  }
}

//-----------------------------------------------------------------------------
// stopUpdateTimer
//-----------------------------------------------------------------------------
void GLLitlRendererWidget::stopUpdateTimer()
{
  tiupdate->stop();
  timeElapse.restart();
}

//-----------------------------------------------------------------------------
// framesCount
//-----------------------------------------------------------------------------
void GLLitlRendererWidget::framesCount()
{
  strFrames.setNum(frames);
  //  strFrames.prepend(tr("Frames: "));
  strFrames.prepend("Frames: ");
  strOpTime.setNum(opTimelaps);
  strOpTime.prepend(" Δop: ");
  strFrames.append(strOpTime);
  //strFrames.append(opTimelaps);
  frames=0;
}
//-----------------------------------------------------------------------------
// update - SLOT Timer tiupdate
//-----------------------------------------------------------------------------
void GLLitlRendererWidget::update(){

  if( glError == GL_NO_ERROR ){
    if(running) { updateGL(); }
  }
  else { // OpenGL ocurred
    stopUpdateTimer();
    QMessageBox::warning(this, "OpenGL Error: "+QString::number(glError), getGLError());
    glError=GL_NO_ERROR;
  }

}

//-----------------------------------------------------------------------------
// getGLError
//-----------------------------------------------------------------------------
#ifndef GLERROR
#define GLERROR(e) case e: exception=#e; break;
#endif

QString GLLitlRendererWidget::getGLError(){
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
