/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 a GPU module associate kernel source, on device function, signature, fileInfo

*/

#include "gpumodule.h"


GPUModule::GPUModule( String name, StringList signature, FileInfo * fileInfo, CUfunction * func, CUmodule * module, String src )
{
    this->mName = name;
    this->signature = signature;
    this->fileI = fileInfo;
    this->cufunc = func;
    this->module = module;
    this->src = src;
}

GPUModule::~GPUModule() {
    if(module!=NULL)
        cuModuleUnload(*module);
    // check if module is use, unload it
    //delete cuFunc;
    delete module;
}

/**
 * @brief GPUModule::signatureArgs
 * @return
 * the arguments expected for the signature (max 8)
 */
/*void * GPUModule::signatureArgs() {

    void * args = {
}*/
