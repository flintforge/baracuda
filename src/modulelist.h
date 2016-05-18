/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * the moduelist turns to be a bricklist
 */

#ifndef MODULELIST_H
#define MODULELIST_H

#include <QListWidget>
#include "gpumodule.h"
#include "brick.h"


class ModuleList : public QListWidget
{
    Q_OBJECT
public:
    explicit ModuleList(QWidget *parent = 0);
    void addBrick(Brick * b);
    Brick* getBrick(int i);

protected:
    void startDrag(Qt::DropActions supportedActions);

};

#endif // MODULELIST_H
