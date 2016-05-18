/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QTime>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QTimeLine>
#include "bartime.h"

class TimelineGraphic : public QGraphicsScene
{
    Q_OBJECT
public:
    TimelineGraphic();

public slots:
    void setTime(int time);
    void expandTimeLine();

signals:
    void timeSet(int time);

protected:

    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


private:
    QTime   time;
    QTimer*   timer;
    QTimer*   refreshTimer; // when to redraw the cursor
    QTimeLine* timeLine;
    //int time; // in millisecs
    int duration; // msec
    int tickInterval;
    // the size in pixel of one frame of time
    int frameX;
    //
    // a vertical line indicating the cursor position
    //QGraphicsItem * cursor;
    //int * cursor;// cursor pos?
    int cursorX;
    QLine cursorLine;//cursor
    //QMenu * ctxMenu;//
    bool dragging;
};



class TimelineView : public QGraphicsView
{
    Q_OBJECT
private:
    TimelineGraphic * timelinegraphic;
public:
    TimelineView(QWidget *parent = 0);

signals:
    
public slots:
    
};




#endif // TIMELINEVIEW_H
