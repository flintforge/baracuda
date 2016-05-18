/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * The GPUModule class wraps
 * a CUmodule + name identifier, function signature
 * associated file and source code
 */

#ifndef GPUMODULE_H
#define GPUMODULE_H

#define CUDA_INIT_OPENGL
#include "cuda_drvapi_dynlink.h"
#include "helper_cuda_drvapi.h"
#include "bartypes.h"


class GPUModule
{

public:
    GPUModule(){}
    GPUModule( String name, StringList signature,
               FileInfo * fileInfo, CUfunction * func, CUmodule * module, String src );
    ~GPUModule();
private:
    String mName;
    StringList signature;
    QFileInfo * fileI;
    CUmodule * module;
    CUfunction * cufunc;
    String cusrc;
    String src;

    String error;// todo : set here if any error was detected during compilation

public:
    CUfunction * func() { return cufunc; }

    /** cpu arguments.
     * if width/height or time are declared,
     * retrieve them from the pipeline (W/H is defined by the pbo), time is time.
     */
    void * args[16];

    String name() { return mName; }
    String getSrc() { return src; }
    StringList getSignature() { return signature; }
    void setName(String name) {mName = name ; }
    FileInfo * fileInfo() { return fileI; }

};

//

#endif // GPUMODULE_H
