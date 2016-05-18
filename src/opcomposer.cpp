/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**

the composition logic + view

 */


#include "opcomposer.h"
#include <QMimeData>
#include "modulelistitem.h"
OpComposer::OpComposer() :
        mCellSize(Brick::WIDTH, Brick::HEIGHT),
        mcellMoveIncrement(Brick::WIDTH/4,Brick::HEIGHT)
      ,selection(0), dragSelection(0), prevSelection(0)
{
    //renderAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    renderAct = new QAction("&Render", this);
    renderAct->setShortcuts(QKeySequence::Open);
    renderAct->setStatusTip(tr("Render this brick"));
    renderAct->setEnabled(false);
    connect(renderAct, SIGNAL(triggered()), this, SLOT(renderBrick()));

    ctxMenu = new QMenu();
    //ctxMenu->addAction("Generator",this, SLOT(addGenerator()), tr("g") );
    //ctxMenu->addAction("Op",this, SLOT(addOp()), tr("o") );
    ctxMenu->addAction(renderAct);

    pointer = new QPoint(0,0);
    int cursorW=Brick::WIDTH/4;
    int cursorH=Brick::HEIGHT;
    cursorPixmap = new QPixmap( cursorW,cursorH );
    cursorPixmap->fill( Qt::transparent );
    QPainter painter(cursorPixmap);
    //painter.setRenderHints( ); //QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
    QPainterPath path;
    QPolygonF polygon;
    polygon
    << QPointF(0,  0)
    << QPointF(cursorW, cursorH/2)
    << QPointF(0,  cursorH)
    << QPointF(0,  0);

    path.addPolygon(polygon);

    QLinearGradient grad( 0, 216 , 216, 376 );
    grad.setColorAt( 0, QColor ( 255, 237, 8, 255 ) );
    grad.setColorAt( 1, QColor ( 255, 0, 8, 255 ) );
    painter.setPen( QPen( QBrush( Qt::black ), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin) );
    painter.setBrush( QBrush( grad ) );
    painter.drawPath( path );
}


void OpComposer::addBrick(Brick * b) {
    // duplicate that brick
    Brick * brick = new Brick ( b->getOp() );
    brick->setPos(pointer->x(),pointer->y());
    this->addItem(brick);
    bricks << brick;
}

/*
 * will this brick connect to an other one */
void OpComposer::connectBrick(Brick *a) {

    List<Brick*> cnx;// = new List<Brick*>;

    foreach (Brick * b, bricks) {
        if(a==b) continue;
        int bx = b->pos().x();
        int ax = a->pos().x();

        // check if there is a brick up to this one
        int y = a->pos().y() - mCellSize.height();
        if( b->pos().y() == y
        && (  ( (bx >= ax) && ( bx < ax + a->width() ))
           || ( (bx <= ax) && (bx + b->width() > ax) )
           )
        )
        {
            a->connectUp(b);
            b->updatePixmap();
            cnx<<b;
            //qDebug()<< "##connect above to this";
            //return true;
        }
/*        else {
            y = a->pos().y() + mCellSize.height();
            if( b->pos().y() == y
             && ( bx >= ax
               && bx < ax + a->width() )
            || (  bx <= ax
               && bx + b->width() > ax)
            )
            {
                a->connectDown(b);
                qDebug()<< "##connect this to below";
                return true;
            }
       // }*/
    }
    a->updatePixmap();
    /*if(cnx->empty()) { return NULL; }
    else return cnx;*/
}

/*
 * activate this brick */
void OpComposer::renderBrick() {
    emit RenderOp(selection);
    qDebug() << "renderop emited";
}

QPoint OpComposer::gridCoord( QPointF p) {
    return QPoint(floor(p.x() / mcellMoveIncrement.width()) * mcellMoveIncrement.width(),
                  floor(p.y()/ mcellMoveIncrement.height()) * mcellMoveIncrement.height());
}


// unused so far
const QRect OpComposer::targetSquare(const QPoint &position) const
{
    return QRect( (position.x() / mcellMoveIncrement.width()) * mcellMoveIncrement.width(),
                  (position.y()/ mcellMoveIncrement.height()) * mcellMoveIncrement.height(),
            Brick::WIDTH, Brick::HEIGHT );
}


// Efficiently draws a grid in the background.
void OpComposer::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,QBrush(Qt::gray));
    qreal left = int(rect.left()) - (int(rect.left()) % mCellSize.width());
    qreal top = int(rect.top()) - (int(rect.top()) % mCellSize.height());

    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = left; x < rect.right(); x += mCellSize.width())
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += mCellSize.height())
        lines.append(QLineF(rect.left(), y, rect.right(), y));
    painter->setPen(Qt::darkGray);
    painter->drawLines(lines.data(), lines.size());
    painter->drawPixmap(pointer->x(),pointer->y(), *cursorPixmap);
    //qDebug()<<"drawing BG..";
}

void OpComposer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent) {
    selection = qgraphicsitem_cast<Brick*>( itemAt(mouseEvent->scenePos(), QTransform()) );
    if (selection) {
        qDebug()<< selection;
        renderBrick();
    }
}

void OpComposer::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch(mouseEvent->button()) {
    case Qt::RightButton :
        ctxMenu->move(mouseEvent->screenPos());
        ctxMenu->show();
        selection = qgraphicsitem_cast<Brick*>( itemAt(mouseEvent->scenePos(), QTransform()) );
        if (selection) {
            renderAct->setEnabled(true);
            selection->setState(Brick::SELECTED);
        }else
            renderAct->setEnabled(false);
        break;

    case Qt::LeftButton :
        ctxMenu->hide();
        if(selection) selection->setState(Brick::NORMAL);
        if(prevSelection) prevSelection->setState(Brick::NORMAL);
        dragSelection = qgraphicsitem_cast<Brick*>( itemAt(mouseEvent->scenePos(), QTransform()) );
        if (dragSelection) {
            qDebug()<< "selection";
            mDragOffset = gridCoord( mouseEvent->scenePos().toPoint() - dragSelection->pos().toPoint() );
            dragSelection->setState(Brick::SELECTED);
            emit BrickSelected(dragSelection);
        } else {
            QPoint p = gridCoord(mouseEvent->scenePos());
            pointer->setX(p.x()); pointer->setY(p.y());
            QGraphicsScene::mousePressEvent(mouseEvent);
            this->update();
        }
        break;
    default: break;
    }
}


void OpComposer::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (dragSelection) {
        // Ensure that the item's offset from the mouse cursor stays the same.
        // mapToScenemSelection->setPos(mouseEvent->scenePos() - mDragOffset);
        // int x = floor((mouseEvent->scenePos().x() - mDragOffset.x()) / mcellMoveIncrement.width()) * mcellMoveIncrement.width();
        // int y = floor((mouseEvent->scenePos().y() - mDragOffset.y()) / mcellMoveIncrement.height()) * mcellMoveIncrement.height();
        QPoint p1 = gridCoord(dragSelection->pos().toPoint()); // previous pos
        Point p = gridCoord(mouseEvent->scenePos() - mDragOffset );
        // check here if the block doesnt collide with an other one
        dragSelection->setPos(p);
        // dragSelection->setState(Brick::MOVING);
        bool over = overlap(dragSelection);
        if(over) {
           dragSelection->setPos(p1);  // revert back
        }
    } else
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void OpComposer::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch(mouseEvent->button()) {
    case Qt::LeftButton :
        if (dragSelection) {
            dragSelection->disconnect();
            connectBrick(dragSelection);
            prevSelection = dragSelection;
            dragSelection = 0;
        } else
            QGraphicsScene::mouseReleaseEvent(mouseEvent);

        qDebug() << "release";

    default : break;
    }

}


void OpComposer::dragModule(PointF p)
{
    QPoint pg = gridCoord(p);
    if( ! pointInBlock(p.toPoint())) {
        pointer->setX(pg.x());
        pointer->setY(pg.y());
    }

    update();
}


/** check if this point doesn't en inside a brick
    TODO : the more compact way to express this
*/
bool OpComposer::pointInBlock( Point p ) {
    //if(bricks.size()==1) return false;
    foreach ( Brick * b, bricks ) {
        if((p.x() >= b->pos().x() )
        &&( p.x() < b->pos().x() + b->width())
        &&( p.y() >= b->pos().y() )
        &&( p.y() < b->pos().y() + b->height()))
        {
            return true;
        }
    }
    return false;

}

/** check if the current brick doesn't overlap one on the list */
bool OpComposer::overlap( Brick * a ) {
    foreach ( Brick * b, bricks ) {
        if(b!=a)
        if(( abs(a->x() - b->x()) < (abs(a->width() + b->width()) / 2) )
        &&(  abs(a->y() - b->y()) < (abs(a->height() + b->height()) / 2))) {
            return true;
        }
    }
    return false;
}


/**********************/
/**      View        **/
/**********************/


OpComposerView::OpComposerView(QWidget *parent) : QGraphicsView(parent) {
    setScene( composer = new OpComposer());
    setSceneRect(0, 0, size().width(), size().height());
    show();
}


void OpComposerView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-baracuda"))
        event->accept();
    else
        event->ignore();
}


void OpComposerView::dragMoveEvent(QDragMoveEvent *event)
{
    composer->dragModule( mapToScene( event->pos()) );
    event->accept();
}


void OpComposerView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-baracuda") )
    {
        QByteArray moduleData = event->mimeData()->data("image/x-baracuda");
        QDataStream dataStream(&moduleData, QIODevice::ReadOnly);
        QRect square = composer->targetSquare(event->pos());
        QPixmap pixmap;
        QPoint location;
        int row;
        dataStream >> pixmap >> location >> row;
        Brick * b = moduleList->getBrick(row);
        composer->addBrick( b );
        composer->connectBrick( b );
        update(square);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}



