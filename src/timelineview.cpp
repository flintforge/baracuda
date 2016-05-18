/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */


#include <QDebug>
#include "timelineview.h"


#define _TIMELINEDURATION 20000

TimelineView::TimelineView(QWidget *parent) :
    QGraphicsView(parent)
{
    setScene( timelinegraphic = new TimelineGraphic());
    show();
    setMouseTracking(true);
}

TimelineGraphic::TimelineGraphic() :
    duration(_TIMELINEDURATION), tickInterval(1000)
{
    timeLine = new QTimeLine(duration, this);
    timeLine->setFrameRange(0, duration);
    timeLine->setUpdateInterval(16); //60fps
    timeLine->start();
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(setTime(int)));
    connect(timeLine, SIGNAL(finished()), this, SLOT(expandTimeLine()));

    setSceneRect(0, 0, width(), 5);

    //timer = new QTimer( this );
    //connect ( timer, SIGNAL( timeout() ), this, SLOT ( updateTime() ) );
    refreshTimer = new QTimer( this );
    connect ( refreshTimer, SIGNAL( timeout() ), this, SLOT ( update() ) );
    refreshTimer->start(40);
    dragging=false;

    //timer->start();
    //time.start();

    // fps 50ms = 20fps
}

// Efficiently draws a grid in the background.
// For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
void TimelineGraphic::drawBackground(QPainter *painter, const QRectF &rect)
{
    float ratio=rect.width()/duration;
    //int x = time.elapsed()*ratio;
    //int x = Time::time.value*1000*ratio;
    int x = timeLine->currentTime()*ratio;

    cursorLine.setP1(QPoint(x,0));
    cursorLine.setP2(QPoint(x,rect.height()));

    //painter->fillRect(rect,QBrush(Qt::gray));
    painter->translate(rect.x(),0);

    painter->setPen(Qt::white);

    for(int i=0; i<=duration; i+=tickInterval) {
        painter->drawLine(i*ratio,rect.height()/2,
                          i*ratio,rect.height());
    }
    painter->setPen(Qt::red);
    painter->drawLine(cursorLine);

}


/**
 * double the timeline and keep running
 */
void TimelineGraphic::expandTimeLine()
{
    int t = timeLine->currentTime();
    if(t>0) {
        duration *=2;
        timeLine->setDuration( duration  );
        timeLine->setFrameRange(0, duration );
        timeLine->start();
        timeLine->setPaused(true);
        timeLine->setCurrentTime(t);
        timeLine->setPaused(false);
    }
}

void TimelineGraphic::setTime(int time) {
    Time::setTime(time);
}

void TimelineGraphic::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int w=views().at(0)->width();
    float ratio= (float)w/duration;
    float t = (mouseEvent->scenePos().x() +w/2) / ratio;

    switch(mouseEvent->button()) {

        case Qt::LeftButton :
            dragging=true;
            timeLine->setPaused(true);
            timeLine->setCurrentTime((int)t);
            timeLine->setPaused(false);
            break;
        case Qt::RightButton :
            if(timeLine->state() == QTimeLine::NotRunning)
                timeLine->start();
            else
                timeLine->toggleDirection();
        break;
        default:break;
    }
}


void TimelineGraphic::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    qDebug() << "double click";
    switch(mouseEvent->button()) {
        case Qt::LeftButton :
        //timeLine->setPaused(timeLine->state() != QTimeLine::Paused);
        timeLine->start();
        break;
    default:
        break;
    }
}


void TimelineGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(dragging==true) {
        int w=views().at(0)->width();
        float ratio= (float)w/duration;
        float t = (mouseEvent->scenePos().x() +w/2) / ratio;
        timeLine->setPaused(true);
        timeLine->setCurrentTime((int)t);
    }
}

void TimelineGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    dragging=false;
    timeLine->setPaused(false);
}




