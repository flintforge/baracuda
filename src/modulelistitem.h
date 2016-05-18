/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * a list of brick as drag source
 */
#ifndef MODULELISTITEM_H
#define MODULELISTITEM_H
#include <QListWidgetItem>
#include "modulelist.h"
#include "brick.h"

class ModuleListItem : public QListWidgetItem
{
private :
    Brick * brick;
public:
    ModuleListItem(Brick * brick);
    Brick * getBrick() {return brick;}
    QVariant data(int role);

};

#endif // MODULELISTITEM_H
