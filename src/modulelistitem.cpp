/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016-05-17 16:24:14>
 * Source distributed under the terms of the GPL v2 license.
 */

/**

  bricks in the list

  */

#include "modulelistitem.h"
#include "brick.h"

ModuleListItem::ModuleListItem(Brick * b) : QListWidgetItem()
{
    this->brick = b;
    setText(brick->getOp()->getName());
    setIcon(QIcon(*brick->pix));
}

QVariant ModuleListItem::data(int role) {
    switch (role) {
    case Qt::DisplayRole : return brick->getOp()->getName(); break;
    case Qt::UserRole : return QIcon(*brick->pix); break;
    default : qDebug() << "wrong role"; return 0;
    }
}
