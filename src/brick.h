/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 *
 * "when you'll see it..."
 *  the graphical view of an OP
 *
 *  holds the connexion marks
 */


#ifndef BRICK_H
#define BRICK_H
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include "bartypes.h"
#include "op.h"


class Brick : public QGraphicsPixmapItem
{
public:
    typedef enum {NORMAL   =0x100,
                  SELECTED =0x010,
                  MOVING   =0x001
                 } STATE;
    typedef enum {NONE =0x100,
                    UP =0x010,
                  DOWN =0x001} CNX;
private:
    OP * op;
    int w, h;

    List<Brick*> up;
    List<Brick*> below;

    bool hasUp;
    bool hasDown;
    void markUp(bool has) { hasUp = has; }
    void markDown(bool has) { hasDown = has; }

    void drawMarkDown();
    void drawMarkUp();
    // drawings
    QRect rect;
    QPolygonF polygon; // the polygon of that brick

    short state;
    short cnx;

    static const QFont font;
    static const QPen white;// = QPen( QBrush( Qt::white ), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    static const QPen black;// = QPen( QBrush( Qt::black ), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    static const QPen yellow,red;

protected:
    static const int ANCHOR_WIDTH=2; //px
    bool mouseover;

public:

    Brick( OP * op );
    Brick( Brick * b ); // new brick from same shared op
    ~Brick();
    //
    OP * getOp() { return op; }
    //
    void connectUp(Brick*);
    void connectDown(Brick*);
    void connect(List<Brick*> * cnx);
    void disconnect();
    void updatePixmap();
    //
    Brick * dup(); //TODO // duplicate this brick and the op along with its parameters
    // the brick must be initialized and on stage

    int width() { return w; }
    int height() { return h; }

    void setState(short state);
    //
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *);
    void paint(QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget);

    // draw the inner shape of that pixmap
    Pixmap * drawPixmap(Pixmap * pix );
    void drawText( Pixmap * pix, String text ); // the text on it
    void drawFrame( Pixmap * pix, STATE state=NORMAL ); // the frame (selected or other state)
    void copyPixmap( Pixmap * src, Pixmap * target );

    static Pixmap * initialPixmap;
    static Pixmap * initialSelectedPix;

    Pixmap * pix;
    Pixmap * brickSelectedPix;
    static const int WIDTH,HEIGHT;

};

#endif // BRICK_H
