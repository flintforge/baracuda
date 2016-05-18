/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**

  the module list widget

  @note : There is a bug in libqxcb when working with a virtual frame,
  wich happens when optimus fires the gpu, related to the mouse events
  This crash the app when dragging an item from the the list to composer.
  It works fine if the gpu isn't a separated 3dcontroller but the actual
  display device.

  for now, the workaround is to insert the module when's it's clicked in the list

  */

#include "modulelist.h"
#include <QDragEnterEvent>
#include <QDebug>
#include "modulelistitem.h"
#include <QMimeData>
#include <QDrag>
#include "brick.h"
#include "modulelistitem.h"

ModuleList::ModuleList(QWidget *parent) :
    QListWidget(parent)
{
    //setViewMode(QListView::IconMode);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

Brick * ModuleList::getBrick(int i) {
    ModuleListItem * it = dynamic_cast <ModuleListItem*> (item(i));
    return it->getBrick();
}

void ModuleList::addBrick(Brick * brick)
{
    ModuleListItem *item = new ModuleListItem(brick);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    item->setData(Qt::UserRole, QVariant(*brick->pix));
    addItem(item);
}

void ModuleList::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QPoint location = item->data(Qt::UserRole).toPoint();

    dataStream << pixmap << location << currentRow();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("image/x-baracuda", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
    qDebug()<<drag;
    drag->exec(Qt::MoveAction);

}

