/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


 * brick UI component

 */

#include "brick.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QPaintEngine>


const int Brick::WIDTH=70;
const int Brick::HEIGHT=22;
const int Brick::ANCHOR_WIDTH;
const QFont Brick::font = QFont("Monospace", 10);

QPixmap * Brick::initialPixmap ;
QPixmap * Brick::initialSelectedPix;

const QPen Brick::white = QPen( QBrush( Qt::white ), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
const QPen Brick::black = QPen( QBrush( Qt::black ), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
const QPen Brick::yellow = QPen( QBrush( Qt::yellow ), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
const QPen Brick::red = QPen( QBrush( Qt::red ), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);


Brick::Brick(OP * o )
{
    op=o;
    state = NORMAL;
    cnx = NONE;
    w = Brick::WIDTH;
    h = Brick::HEIGHT;

    rect = Rect(0,0,w,h);
    polygon << QPointF(0, 0)
            << QPointF(w, 0)
            << QPointF(w, h)
            << QPointF(0, h)
            ;

    if(initialPixmap==NULL) {
        initialPixmap = new Pixmap(w,h);
        drawPixmap( initialPixmap);
        drawFrame( initialPixmap );
    }

    // copy the initial pixmap in this own pixmap
    // (could wait further until the brick text or size gets modified)
    pix = new Pixmap(w,h);
    copyPixmap(initialPixmap,pix);
    drawText( pix, o->getName());
    setPixmap(*pix);

    mouseover = false;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
}
//
Brick::~Brick() { }


void Brick::setState(short state)
{
    this->state = state;
    updatePixmap();
}

void Brick::updatePixmap()
{
    drawPixmap( pix );
    drawText( pix ,  op->getName());

    switch(state) {
        case Brick::SELECTED   : drawFrame(pix, Brick::SELECTED);  break;
        case Brick::NORMAL     :
        default         : drawFrame(pix, Brick::NORMAL);    break;
    }
    if(cnx & DOWN) drawMarkDown();
    if(cnx & UP) drawMarkUp();

    setPixmap(*pix);
}

/*
 * connect this brick to the next one below
 * and vice versa
 */
void Brick::connectDown(Brick* d) {
    op->in << d->getOp();
    this->cnx = this->cnx | Brick::DOWN;
    d->cnx = d->cnx | Brick::UP;
}

void Brick::connectUp(Brick*d) {
    op->in << d->getOp();
    this->cnx = this->cnx | Brick::UP;
    d->cnx = d->cnx | Brick::DOWN;
}

// unused
void Brick::connect(List<Brick*> * cnx) {
    if(cnx!=NULL) {
        foreach (Brick*b, *cnx) {
            qDebug() << b->getOp()->getName();
            this->connectDown(b);
            b->updatePixmap();
        }
        delete cnx;
    }
}

//
/*
 * search the upper to remove that one from the list
 **/
void Brick::disconnect() {
    // consider only one unique output and no "dispatch"
    //if(hasDown) > disconnect down brick
    this->cnx = Brick::NONE;
    OP * op=getOp();

    foreach(OP * o, op->in) {
        o->out.removeAll(op);
            // pouac. this where you would like to say that an empty
            //outlist makes a bricks unmarked.
    }

    op->in.clear();
    op->out.clear();
}

//
Pixmap * Brick::drawPixmap( Pixmap * pixmap ) {

    pixmap->fill( Qt::transparent );

    QPainter painter(pixmap);
    QLinearGradient grad( 0, 0 , width(), height());
    grad.setColorAt( 0, QColor ( 255, 255, 255, 255 ) );
    grad.setColorAt( 1, QColor ( 192, 192, 192, 255 ) );

    painter.setBrush( QBrush( grad ) );
    painter.drawPolygon( polygon );

    return pixmap;
}
//

//
void Brick::drawText( Pixmap * pix, String text  ) {
    QPainter painter(pix);
    painter.setPen(  black );
    painter.setFont( font );
    painter.drawText( rect, Qt::AlignCenter, text );
}

void Brick::drawFrame( Pixmap * pix, STATE state ) {
    QPainter painter(pix);
    painter.setBrush( Qt::NoBrush);
    /*painter.setPen( state & Brick::NORMAL ? white : black );
    painter.drawPolyline(&polygon[2],3);
    painter.setPen( state & Brick::NORMAL ? black : white );
    painter.drawPolyline(&polygon[0],3);*/
    if(state & Brick::SELECTED) {
        painter.setPen( QPen( QBrush( Qt::red ), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter.drawPolyline(&polygon[3],2);
    }

}

void Brick::drawMarkDown() {
    QPainter painter(pix);
    painter.setBrush( Qt::NoBrush);
    painter.drawRect( 0,height()-5, 5,5 );
}

void Brick::drawMarkUp() {
    QPainter painter(pix);
    painter.setBrush( Qt::NoBrush);
    painter.drawRect( 0,0, 5,5 );
}


void Brick::copyPixmap( Pixmap * src, Pixmap * target ) {
    QPainter painter(target);
    painter.drawPixmap(rect,*src);
}



void Brick::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    /*
    if(update) {
       when update needed, redraw according to the new size of the brick
    }*/
    QGraphicsPixmapItem::paint(painter,option,widget);
}


void Brick::hoverEnterEvent(QGraphicsSceneHoverEvent *me)
{
    if(!mouseover) {
        mouseover=true;
    }
    // check if sides anchors are reached
    if( me->pos().x() <= ANCHOR_WIDTH
     || me->pos().x()>= width() - ANCHOR_WIDTH ) {
        setCursor(Qt::SizeHorCursor);
    }else {
        // TODO : why no set second time?
        if (cursor().shape() != Qt::OpenHandCursor)
            setCursor(Qt::OpenHandCursor);
    }

}


void Brick::hoverLeaveEvent(QGraphicsSceneHoverEvent *me)
{
    if (cursor().shape() != Qt::ArrowCursor)
        setCursor(Qt::ArrowCursor);
}
/*
bool DoBoxesIntersect(Brick * a, Brick * b) {
    return (abs(a->x() - b->x()) * 2 < (a->width + b->width)) &&
            (abs(a->y() - b->y()) * 2 < (a->height + b->height));
}*/

void Brick::hoverMoveEvent(QGraphicsSceneHoverEvent *me)
{
    if(state==MOVING) {
        if (cursor().shape() != Qt::ClosedHandCursor)
                setCursor(Qt::ClosedHandCursor);
    }
    else hoverEnterEvent(me);
    /*qDebug()<< "hover move";
    //discard event if dragging
    if( me->pos().x() <= ANCHOR_WIDTH
     || me->pos().x()>= width() - ANCHOR_WIDTH )
        this->setCursor(Qt::SizeHorCursor);
    else {
        if (cursor().shape() != Qt::OpenHandCursor)
                setCursor(Qt::OpenHandCursor);

        //scene()->mouseReleaseEvent(me);
    }*/

    //qDebug()<< "mouse over " << this;
}

