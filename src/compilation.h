/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * A threaded wrapper for the compilation process
 * input : a file, a gpu
 * output : a signal with a module
  */


#ifndef COMPILATION_H
#define COMPILATION_H

#include <QThread>
#include "cudacompil.h"


class Compilation : public QThread
{
    Q_OBJECT

private :
    String file;
    GPU * gpu;
    GPUModule * module;

public:

    /*explicit*/ Compilation(String file, GPU * gpu=0, QObject *parent = 0) :
        QThread(parent)
    {
        this->file=file;
        this->gpu=gpu;
    }

    void run() /*Q_DECL_OVERRIDE*/ {
        CudaCompiler compil;
        GPUModule * module = compil.createGPUModuleFromFile(file,gpu);
        emit done(module);
    }


signals:
    void done(GPUModule * module);
};

#endif
