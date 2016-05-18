/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.

 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/10 14:30:09>
 * Source distributed under the terms of the GPL v2 license.

 *
 * using nvcc, produces ptx and cubin files given kernel sources
 */


#ifndef CUDACOMPILER_H
#define CUDACOMPILER_H

#include "bartypes.h"
#include "gpumodule.h"
#include "gpu.h"
#define CUDCHK checkCudaErrors

#define NVCC_OK                 0x0000
#define NVCC_FILE_NOT_FOUND     0x1000
#define NVCC_COMPILATION_ERROR  0x0002
#define NVCC_NO_INPUT_FILE      0xFF00
#define NVCC_COMMAND_NOT_FOUND  0x7F00

static String NVCC = __STR(NVCCBIN);

class CudaCompiler : public QObject
{

private:

    Process nvcc;
public:
    CudaCompiler();
    bool checkNVCCPresence();
    static bool NVCC_Ready; // in case of versioning spaghetti

    int runProcess( QProcess * process, String *s, StringList * args, ByteArray * result  );
    bool compileCUtoPTX(const String * fileName, int &result, String * log);
    ByteArray * ptxDump(File * file);
    CUmodule * compilePTX(uchar * KernelPTXDump, GPU * gpu);
    CUmodule * compileFromFile(const String fileName, GPU * gpu) ;
    // GPUModule * compileFromSrc(const String src) ;  using of a tmp dir
    GPUModule * createGPUModuleFromFile(const String fileName, GPU * gpu=0);
};

#endif // CUDACOMPILER_H
