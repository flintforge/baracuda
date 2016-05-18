/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


using nvcc, produces ptx and cubin files given kernel sources

*/

#include "cudacompil.h"

/*
 * TODO : option window + environnement settings
 *
 * THREAD
 */

#include <stdlib.h>

bool CudaCompiler::NVCC_Ready;

CudaCompiler::CudaCompiler()
{
   // bin and usr/bin are mandatory
    qputenv("PATH",__STR(PATH)); // path to nvcc, cudafe, cicc /usr/bin/:/usr/local/bin
    // qgetenv("HOME") is not as reliable as QDesktopServices::storageLocation(QDesktopServices::HomeLocation)
    nvcc.setProcessChannelMode(QProcess::MergedChannels);
}

int CudaCompiler::runProcess( QProcess * process, String *s, StringList * args, ByteArray * result  )
{
    process->start(*s, *args, QIODevice::ReadOnly);
    if (!process->waitForStarted(3000)) {
        qDebug()<< *s << " nvcc still not started!";
        result->append(*s);
        result->append(" not started");
        return false;
    }
    if (!process->waitForFinished(8000)) { //
        qDebug() << *s << " sleeping!";
        result->append(*s);
        result->append(" sleeping!");
        return false;
    }
    if(result==NULL) { result = new ByteArray(); }

    result->append( process-> readAll()   // or readAllStandardOutput()
                    );
    process->close();
    return process->exitCode();
}

bool CudaCompiler::checkNVCCPresence() {

    qDebug()<<" checking nvcc presence";
    ByteArray result ;
    StringList args;
    args <<  "--version";

    NVCC_Ready = (  runProcess(&nvcc, &NVCC, &args, &result)
                    ==NVCC_OK);
    qDebug() << result;

    result="";
    String GCC("gcc");
    Process gcc;
    runProcess(&gcc, &GCC, &args, &result);
    qDebug() << result;

    return NVCC_Ready;
}


/*
 * call nvcc
 */
bool CudaCompiler::compileCUtoPTX(const String * filename, int &result, String * log)
{
    ByteArray res;
    StringList args;
    //args << "-I"STR(CUDA_DIR)"/include -noprof -ldir "STR(CUDA_DIR)"/nvvm/libdevice/ --ptx" << *filename;
    args << "-I"__STR(CUDA_DIR)"/include";
    //args << "-noprof"; // following options starting from cuda 7.5 
    //args << "-ldir "STR(CUDA_DIR)"/nvvm/libdevice/"; or set options in nvcc.profile 
    args << "--ptx" << *filename;
    qDebug() << args;
    result = runProcess(&nvcc, &NVCC, &args, &res);
    qDebug() << res;
    log << res;
    return (result==NVCC_OK );
}


ByteArray * CudaCompiler::ptxDump(File * file) {

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can't open "<< file;
        return 0;
    }

    ByteArray * baf = new ByteArray(file->readAll());
    file->close();
    return baf;
}


#if defined(__x86_64) || defined(AMD64) || defined(_M_AMD64)
   // #pragma warn "requirement for 64bits ?"
    // if something is required to compile a 64 bits variant
    // status = cuModuleLoadDataEx(&cuModule, KernelPTXDump, jitNumOptions, jitOptions, (void **)jitOptVals);
#endif

/*
 * returns the cubin = gpu machine code  form the PTX ISA assembly
 * setup JIT compilation options and perform compilation
 */
CUmodule * CudaCompiler::compilePTX(uchar * KernelPTXDump, GPU * gpu=0)
{
    // consider the default context is in run if gpu==0
    // TODO : make it mandatory
    if(gpu!=0) CUDCHK( cuCtxPushCurrent(gpu->context) );

    //CUDCHK( cuCtxSynchronize());

    // in this branch we use compilation with parameters
    const unsigned int jitNumOptions = 3;
    int jitLogBufferSize = 1024;
    int jitRegCount = 32;

    CUjit_option *  jitOptions   = new CUjit_option[jitNumOptions];
    void **         jitOptVals   = new void*[jitNumOptions];
    char *          jitLogBuffer = new char[jitLogBufferSize];

    jitOptions[0] = CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES;// set up size of compilation log buffer
    jitOptions[1] = CU_JIT_INFO_LOG_BUFFER;// set up pointer to the compilation log buffer
    jitOptions[2] = CU_JIT_MAX_REGISTERS;  // set up pointer to set the Maximum # of registers for a particular kernel

    jitOptVals[0] = (void *)(size_t)jitLogBufferSize;
    jitOptVals[1] = jitLogBuffer;
    jitOptVals[2] = (void *)(size_t)jitRegCount;

    // compile with set parameters
    qDebug()<<"> Compiling PTX module";
    CUmodule * cuModule = new CUmodule();

    CUDCHK( cuModuleLoadDataEx( cuModule, KernelPTXDump, jitNumOptions, jitOptions, (void **)jitOptVals) );

    qDebug()<< "PTX JIT log: \n [" <<  jitLogBuffer <<"]" ;

    delete [] jitOptions;
    delete [] jitOptVals;
    delete [] jitLogBuffer;

    CUDCHK( cuCtxPopCurrent(0));

    return cuModule;
}

/** @deprecated : use createGPUmodulefromfile
 */
CUmodule * CudaCompiler::compileFromFile(const String fileName, GPU * gpu)
{
    if(fileName == NULL) { return 0; }
    QFileInfo cu( fileName );
    if (! cu.isReadable()) { return 0; }
    else
    {
        /* Get filename */
        int result;
        String res;
        compileCUtoPTX( &fileName, result, &res );
        if(result != NVCC_OK ) {
            qDebug() << "Error during compilation from file " << fileName << " [" << result <<"]";
            return 0;
        }
        else {
            qDebug()<< " cu -> ptx ok";
        }
        String ptxfile = cu.baseName() + ".ptx";
        QFile file(ptxfile);
        QFileInfo ptx(ptxfile);
        if (ptx.isReadable()) {

            ByteArray * bar = ptxDump(&file);
            return compilePTX((uchar*) bar->data(), gpu);
            delete bar;
        }
    }
    return NULL;
}


GPUModule * CudaCompiler::createGPUModuleFromFile(const String fileName, GPU * gpu)
{
    qDebug() << "createGPUModuleFromFile " << fileName;
    if(fileName == NULL) { return 0; }

    FileInfo kcu = FileInfo( fileName );

    if (! kcu.isReadable()){
        qDebug() << "can't read "<< fileName;
        return 0;
    }

    {
        int result;
        String res;
        compileCUtoPTX( &fileName, result, &res );
        if(result != NVCC_OK ) {
            qDebug() << "Error during module compilation of " << fileName << " \n [" << result <<"]";
            return 0;
        }

        // Cuda Kernel compiled.
        // qDebug()<< " cu -> ptx ok";
        // find the kernel main function
        // the first __global__ one is considered as the entry point
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "could not open "<< fileName;
            return NULL;
        }

        // extract the main kernel function
        QStringList list;
        QStringList signature;
        String content(file.readAll());

        // to strip the comments out :
        //QRegExp lcomments("//.*\n");
        //QRegExp ncomments("/\\*.*\\*/");
        //lcomments.setMinimal(true);
        //ncomments.setMinimal(true);
        //content.remove(lcomments);
        //content.remove(ncomments);

        list = content.split(QRegExp("(\\s+,?\\s*)|(,\\s+)|\\s*\\)\\s*|\\s*\\(\\s*|\n+"));

        // ensure the kernel is declared as [extern "C"]
        int i=0;
        i=list.indexOf("extern");
        if(i==-1 || list.at(i+1)!="\"C\"") {
            qDebug() << fileName << "\nThere is no \'extern \"C\"\' entry point in this kernel."
                      <<  "\nthey are mandatory to prevent gcc name mangling (ie keep the function name in PTX)";
            return NULL;
        }

        QString entryPoint;

        i=list.indexOf("__global__"); // return type
        entryPoint=list.at(i+2); // except if returning an unsigned variable
        //qDebug() << "=== " << entryPoint;
        i++;

        QString tok;
        while(list.at(i)!="{" && i<list.size()-1) {
            tok=list.at(i);
            if(tok=="unsigned") {
               tok="u"+list.at(i+1);
               i++;
            }
            if(list.at(i+1).startsWith("*"))
               tok+="*";
            signature << tok;
            i+=1;
        }

        qDebug() << entryPoint << ":" << signature;

        file.close();

        String ptxfilename = kcu.baseName() + ".ptx";
        QFile ptxfile(ptxfilename);
        QFileInfo ptx(ptxfilename);

        if (ptx.isReadable()) {
            ByteArray * bar = ptxDump(&ptxfile);
            CUmodule * module = compilePTX((uchar*) bar->data(), gpu);
            delete bar;
            CUfunction * func = new CUfunction();

            CUDCHK( cuModuleGetFunction( func , *module, entryPoint.toStdString().c_str()) );

            GPUModule  * gm = new GPUModule(entryPoint, signature, new FileInfo(kcu), func, module, content);
            return gm;
        }
        else {
            qDebug() << "generated ptx file is not readable : "<< fileName;
            return NULL;
        }
    }
}

