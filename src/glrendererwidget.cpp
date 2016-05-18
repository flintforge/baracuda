/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


  The Renderer widget

*/

#include <QString>
#include <QMessageBox>
#include <QtOpenGL/QGLWidget>
#include "glrendererwidget.h"
#include "bartime.h"

GLRendererWidget::GLRendererWidget(QWidget *parent , const QGLWidget * shared)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shared)
    ,isCopy(false)
    ,op(0)
    ,copyRenderer(NULL)
    ,frames(0)
    ,opTimelaps(0)
    ,running(false)

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


GLRendererWidget::~GLRendererWidget()
{
  delete tiupdate;
  delete tifps;
  if(!isCopy)
    delete pbo;
    // and the op and the module please
}


void GLRendererWidget::initializeGL()
{
    qDebug() << this << " initgl";
    glClearColor(1, 0, 0, 1);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    if(!isCopy) {
        pbo = new GLCUPixelBuffer(512,512);
        pbo->init(512,512);
        //running = true;
        startUpdateTimer();
    }
    if(copyRenderer) {
     copyRenderer->setPixelBuffer(getPBO());
    }
}


void GLRendererWidget::setPixelBuffer(GLCUPixelBuffer * pbo) {
    qDebug() << this << " pbo set" ; this->pbo = pbo; }

void GLRendererWidget::setOp(OP * op){    this->op = op; }

void GLRendererWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glEnable(GL_TEXTURE_2D);

  if(!isCopy)
    pbo->resize(w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*
 * the renderpass  **/
void GLRendererWidget::paintGL()
{
    // could be after the gl commands. make a perf check about this
    if(op!=NULL) {
        /*if(gpu!=0) {
            CUDCHK( cuCtxPushCurrent(gpu->context) );
        }*/
        opTime.restart();
        //Q_ASSERT(pbo);
        CUgraphicsResource cudaResource = pbo->getCudaResource();
        //if(
        cuGraphicsMapResources(1, &cudaResource, 0); // )==CUDA_SUCCESS )
        size_t  num_bytes; // check this

        CUDCHK( cuGraphicsResourceGetMappedPointer(&pbo->devptr, &num_bytes, cudaResource ));

        op->launch(pbo, Time::time);

        CUDCHK( cuGraphicsUnmapResources(1, &cudaResource, 0) ) ;
        CUDCHK( cuCtxSynchronize() );
        //qDebug()<<timeElapse.elapsed() << " "<<Time::time;
        opTimelaps = opTime.elapsed();
    }

    // no matrix change or we render over the entire buffer
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW); //Select The Modelview Matrix
    //glLoadIdentity();

    // bind buffer after cuda is done
    pbo->bind();
    // Draw a single Quad with texture coordinates for each vertex.
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);  glVertex2f(-1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f);  glVertex2f(-1.0f, 1.0f);
        glTexCoord2f(1.0f,0.0f);  glVertex2f( 1.0f, 1.0f);
        glTexCoord2f(1.0f,1.0f);  glVertex2f( 1.0f,-1.0f);
    glEnd();
    pbo->release();

    glColor3f(0,1,0);
    renderText(10,10, strFrames);
    glColor3f(1,1,1);

    ++frames;

    glError = glGetError();

    if(copyRenderer!=NULL) {
        //copyRenderer->running=true; //dirty
        // works, but
        // TURNED OF BECAUSE of
        // primus: warning: glXSwapBuffers: drawable not current
        //   copyRenderer->updateGL();
    }
    //if(gpu!=0 && op!=NULL) CUDCHK( cuCtxPopCurrent(0) );
}

//-----------------------------------------------------------------------------
// startUpdateTimer
//-----------------------------------------------------------------------------
void GLRendererWidget::startUpdateTimer()
{
  if( tiupdate->isActive() == false )
  {
    //timeElapse.restart();
    tiupdate->start(0);
  }
}

//-----------------------------------------------------------------------------
// stopUpdateTimer
//-----------------------------------------------------------------------------
void GLRendererWidget::stopUpdateTimer()
{
  tiupdate->stop();
  //timeElapse.restart();
}

//-----------------------------------------------------------------------------
// framesCount
//-----------------------------------------------------------------------------
void GLRendererWidget::framesCount()
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
void GLRendererWidget::update()
{
  if( glError == GL_NO_ERROR ){
    if(running) {

        updateGL();
        if(isCopy) { qDebug()<< opTimelaps << " " << strOpTime; }
    }
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

QString GLRendererWidget::getGLError(){
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


void GLRendererWidget::mouseDoubleClickEvent(QMouseEvent * mouseEvent) {

    if(/*isFullScreen()*/ this->parent()==NULL) {
         this->setWindowState(Qt::WindowMaximized);
         this->setParent(dynamic_cast <QWidget*> (mParent)); // to restore the window
      } else {
         mParent = this->parent();
         this->setParent(0);
         this->setWindowState(Qt::WindowFullScreen);
      }
    this->show();
}
