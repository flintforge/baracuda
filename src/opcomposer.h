/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef OPCOMPOSER_H
#define OPCOMPOSER_H

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMainWindow>
#include <QVarLengthArray>
#include <math.h>
#include <QMenu>
#include <QDebug>
#include <QDragEnterEvent>
#include "modulelist.h"
#include "brick.h"

class OpComposer : public QGraphicsScene
{
    Q_OBJECT
public:
    OpComposer();
    void setList(ModuleList * moduleList) { modList = moduleList; }
    const QRect targetSquare(const QPoint &position) const;

   // void dragMoveEvent(QDragMoveEvent * event);
    void dragModule(PointF p);
    void connectBrick(Brick * a); // see if the dragged brick connect

public slots:
    //void addGenerator();
    //void addOp(GPUModule *, bool generator=false); //@deprec
    void renderBrick();// render the selected op
    void addBrick(Brick * b);
    //void removeBrick(Brick * b);

signals:
    void BrickSelected(Brick *);
    void RenderOp(Brick*b);// render the selected op

protected:

    // Efficiently draws a grid in the background.
    // For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    /* goes in the view
    virtual void drawForeGround(QPainter *painter, const QRectF &rect);
    void drawItems(QPainter *painter, int numItems,
                   QGraphicsItem *items[],
                   const QStyleOptionGraphicsItem options[],
                   QWidget *widget);
                   */
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    bool overlap(Brick * b);
    bool pointInBlock(Point P);

private:
    ModuleList * modList;

    // The size of the cells in the grid.
    const QSize mCellSize;
    const QSize mcellMoveIncrement;

    // The item being dragged.
    Brick * selection;
    Brick * dragSelection;
    Brick * prevSelection;
    // a multiselect
    //QGraphicsItem *mSelection;

    // The distance from the top left of the item to the mouse position.
    QPoint mDragOffset;

    Pixmap * cursorPixmap; // will go into its own class

    QPoint * pointer;//cursor
    QMenu * ctxMenu;
    QAction * renderAct;
    List<Brick * > bricks;

    Point gridCoord(PointF p);
    // Tree<Brick * > bricks // intrisically defined by the brick
};



class OpComposerView : public QGraphicsView
{
private:
    ModuleList * moduleList;
public:
    OpComposerView(QWidget *parent);
    OpComposer * composer;
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void setList(ModuleList* list) { this->moduleList = list ;}
};


//nclude "main.moc"


#endif // OPCOMPOSER_H
