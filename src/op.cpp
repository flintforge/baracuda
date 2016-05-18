/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**
 * a warning : the kernel signature will change
 * if there are multiple input
 * in1, in2, in3...
 *
 * TODO : keep an eye on num_bytes
 */

#include "op.h"
#include "baracuda.h"


OP::OP(GPUModule * module /*, bool isG*/)
{
    //generator(isG);
    //in = new List<OP*>;
    name = module->name();
    this->module = module;
    in.empty();
}


/*
void setBasicParameters( CUfunction * f, CUdeviceptr devptr, int W, int H, int &offset ) {
    checkCudaErrors(cuParamSeti(*kernel, offset, value));    offset += sizeof(value);
}
void setExtendedParameter( CUfunction * f, int value, int &offset ) {
    checkCudaErrors(cuParamSeti(*kernel, offset, value));    offset += sizeof(value);
}*/

/**
 * @brief OP::execute
 * @return status of execution
 *
 * run one module on the given pixelbuffer
 * calling the parent op first if any
 * with time parameter t
 *
 * todo : don't map/unmap on every frame ?
 */

STATUS OP::launch(GLCUPixelBuffer * pbo, int time) {

    //Q_ASSERT(pbo);
    // map the resource one an get the handle on the deviceptr
    //CUgraphicsResource cudaResource = pbo->getCudaResource();
    // is the mapping necessary on every frame ?


    //if( cuGraphicsMapResources(1, &cudaResource, 0)==CUDA_SUCCESS )
    //{

        //size_t  num_bytes; // check this
        //CUDCHK ( cuGraphicsResourceGetMappedPointer(&pbo->devptr, &num_bytes, cudaResource ));

        run(&pbo->devptr,pbo->W(),pbo->H(),time);


    /*}else{
        qDebug()<< "OP launch mapping failed!!";
    }*/

    //CUDCHK ( cuGraphicsUnmapResources(1, &cudaResource, 0) ) ;
    return RUNNING;
}


/**
 * width and height are needed
 * only if they are required by the kernel's computation
 */
STATUS OP::run( CUdeviceptr * devptr, int W, int H, int time ) {

    // backward the op chain to the first one
    if(!in.empty()) in.first()->run(devptr,W,H,time);

    // this is threaded... no problem so far but take care with the
    // module->func context switching
    CUfunction * kernel = this->module->func();

     #if __CUDA_API_VERSION >= 4000
        void * args[4] = { devptr, &W, &H, &time };
        // TODO: automatic check against signature
        // should the pbo be bound to a specific device :
        //void *args[4] = { &pbo->devptr, &w, &h, &time};
        CUDCHK ( cuLaunchKernel( *kernel,
                                  W/BLOCKSZ,H/BLOCKSZ, 1, //grid
                                   BLOCKSZ, BLOCKSZ, 1,  //block
                                   0,
                                   NULL, args, NULL) );

        /*
        void * args[8] = this->module->args;
        args[0] = devptr;

        CUDCHK ( cuLaunchKernel( * this->module->func(),
                                   W/BLOCKSZ,H/BLOCKSZ, 1, //grid
                                   BLOCKSZ, BLOCKSZ, 1,  //block
                                   0,
                                   NULL, args, NULL) );
        */
     #else
    /*
    // Funny thing here : if the signature is lesser than the passed arguments,
    // they are simply discared ( ex : with a kernel that takes only (devptr,w,h) , not time

    int offset = 0;
    CUDCHK(cuParamSetv(*kernel, offset, (void*) devptr, sizeof(devptr)));
    offset += sizeof(*devptr);
    ulong *w=(ulong*) args[1]; // to see how to handle array params
    ulong *t=(ulong*) args[3];

    CUDCHK(cuParamSeti(*kernel, offset, (int)*w));    offset += sizeof(w);
    CUDCHK(cuParamSeti(*kernel, offset, H));    offset += sizeof(&H);
    CUDCHK(cuParamSeti(*kernel, offset, (int)*t)); offset += sizeof(t);
    CUDCHK(cuParamSetSize(*kernel, offset));

    CUDCHK(cuFuncSetBlockShape(*kernel, BLOCKSZ, BLOCKSZ, 1)); //16x16 = 256
    CUDCHK(cuFuncSetSharedSize(*kernel, 2*BLOCKSZ*BLOCKSZ*sizeof(uint)));//512/set it to maxsharde 16384
    // set execution configuration for the CUDA kernel
    CUDCHK(cuLaunchGrid(*kernel, W/BLOCKSZ, H/BLOCKSZ));
    // compare with
    // CUDCHK(cuLaunchGrid(*kernel, W*H/16, 16));
    // CUDCHK(cuLaunchGrid(*kernel, W*H/32, 32));
    // CUDCHK(cuLaunchGrid(*kernel, W*H/64, 64));
     */
    #endif


    return RUNNING;
}
