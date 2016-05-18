/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include "cudacompil.h"
#include "bartime.h"
#define DEBUG

int main(int argc, char *argv[])
{
    qDebug()<<  "╔══════════════════════════╗";
    qDebug()<<  "║  Baracuda 0.5 Generator  ║";
    qDebug()<<  "╚══════════════════════════╝";
    qDebug()<<  "     by Philippe Estival ";
    qDebug()<<  "     phil.estival@free.fr";
    qDebug()<<  "     build N#" << _BUILDNUM_;

    (new CudaCompiler())->checkNVCCPresence();

    QApplication a(argc, argv);
#ifdef DEBUG
    qSetMessagePattern("%{file}(%{line}): %{message}");
#endif
    GPU*gpu=new GPU();
    gpu->startup();
    GPU::gpu=gpu;

    MainWindow * w = new MainWindow(gpu);

    w->setWindowTitle("Baracuda 0.5");
    w->setWindowIcon(QIcon(":/baracuda.jpg"));
    w->show();
    
    return a.exec();
}
