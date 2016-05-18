/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.


 CUDA GPU control

*/

#include "gpu.h"
#include <QDebug>

#define __WARNING(TEXT) __STRINGIFY(GCC warning TEXT)
#define INFO_CUDA_API_VERSION(VALUE) __WARNING(__STR(CUDA DYNLINK API VERSION = VALUE))
_Pragma (INFO_CUDA_API_VERSION(__CUDA_API_VERSION))


GPU * GPU::gpu = NULL;

GPU::GPU()
{
}

GPU::~GPU() {
    cuCtxDestroy(context);
}

void GPU::startup (int devID /*=0 first one*/) {
    int driverVer;// uint apiVer;
    int major, minor;
    char deviceName[256];

    // link to cuda driver dynamically
    status = cuInit(0, __CUDA_API_VERSION);

    if (CUDA_SUCCESS != status)
    {
     fprintf(stderr, "Fatal error while dynloading the CUDA driver\n");
     fprintf(stderr, "cuInit returned %d\n %s\n", status, getCudaDrvErrorString(status));
     exit(EXIT_FAILURE);
    }

    // get cuda-capable device count
    int deviceCount = 0;
    CUDCHK( cuDeviceGetCount(&deviceCount) );

    if (deviceCount == 0)
    {
        fprintf(stderr, "No devices supporting CUDA detected\n");
        exit(EXIT_SUCCESS);
    }

    if (devID < 0) devID = 0;

    if (devID > deviceCount -1)
    {
        fprintf(stderr, "initCUDA (Device=%d) invalid GPU device.  %d GPU device(s) detected.\n\n", devID, deviceCount);
        status = CUDA_ERROR_NOT_FOUND;

        cuCtxDestroy(context);
        exit(EXIT_FAILURE);
    }

    // pick up device with zero ordinal (default, or devID)
    CUDCHK( cuDeviceGet(&device, devID) );
    deviceID = devID;
    // get compute capabilities and the devicename
    CUDCHK( cuDeviceComputeCapability(&major, &minor, device));
    CUDCHK( cuDeviceGetName(deviceName, 256, device));
    CUDCHK( cuDriverGetVersion(&driverVer));
    //CUdevprop *props;
    //CUDCHK( cuDeviceGetProperties(&props,device));
    //CUDCHK( cuCtxGetApiVersion(context, &apiVer));
    // use a larger block size for Fermi and above
    qDebug() << "GPU Device "<< device <<":"<< deviceName
             << "\n Compute cap "   << major << "."<< minor
             << "\n CUDA API v."    << __CUDA_API_VERSION << " (compilation configuration)"
             << "\n CUDA Driver v." << driverVer
             << "\n block size advice : "<< ( (major < 2) ? 16 : 32 );

    CUDCHK( cuCtxCreate(&context, 0, device) );

    if (CUDA_SUCCESS != status)
    {
       cuCtxDestroy(context);
       exit(EXIT_SUCCESS);
    }

    CUDCHK( cuCtxPushCurrent(context) );
}



