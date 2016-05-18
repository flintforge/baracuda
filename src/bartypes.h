/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef BARTYPES_H
#define BARTYPES_H

#include <QList>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QPolygonF>


/** @deprecated
now that the port from
our original C++ source is complete
TODO : remove
*/
#define List QList
typedef QString String;
typedef QFile File;
typedef QFileInfo FileInfo;
typedef QProcess Process;
typedef QByteArray ByteArray;
typedef QStringList StringList;
typedef QPixmap Pixmap;
typedef QPoint Point;
typedef QPointF PointF;
typedef QRect Rect;
typedef QPolygonF PolygonF;

#endif // BARTYPES_H
