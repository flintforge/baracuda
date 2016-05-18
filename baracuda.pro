#-------------------------------------------------
#
#  Baracuda 0.5 
#
#-------------------------------------------------

QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = baracuda
TEMPLATE = app

debug:QMAKE_CXXFLAGS += -g
CUDA_SOURCES = $$OTHER_FILES

CUDA_ARCH = sm_11

PATH = /usr/bin/:/usr/local/bin
# path to your gpu sdk (toolkit+samples)
CUDA_DIR = /usr/local/cuda-7.5 
CUDA_SDK = /usr/local/cuda-7.5/samples/
# include paths
INCLUDEPATH += $$CUDA_DIR/include
INCLUDEPATH += $$CUDA_SDK/common/inc

# lib dirs
QMAKE_LIBDIR += $$CUDA_DIR/lib64

LIBS += -lcudart -ldl #-lGLEW
# join the includes in a line
CUDA_INC = $$join(INCLUDEPATH,' -I',' ')


# for compilation going directly through the nvcc chain :
# (unused)
cuda.input    = CUDA_SOURCES
cuda.output   = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
cuda.commands = $$CUDA_DIR/bin/nvcc -arch=$$CUDA_ARCH $$NVCCFLAGS $$CUDA_INC $$LIBS -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cuda.dependency_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}

# Tell Qt that we add more to the Makefile
QMAKE_EXTRA_COMPILERS += cuda


unix:!macx: LIBS += -lcudart -L/opt/cuda/lib

RESOURCES += baracuda.qrc

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

# nvcc flags (ptxas option verbose can be useful)
debug:NVCCFLAGS = -g -G  --ptxas-options=-v  #-pg #-keep --keep-dir=obj
release:NVCCFLAGS = -O

debug: DEFINE += -D_DEBUG_



OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

VPATH += ./src
SOURCES =\
    cuda_drvapi_dynlink.c \
    main.cpp\
    mainwindow.cpp \
    op.cpp \
    opchain.cpp \
    opcomposer.cpp \
    gpu.cpp \
    cudacompil.cpp \
    brick.cpp \
    moduleslistmodel.cpp \
    gpumodule.cpp \
    modulelist.cpp \
    modulelistitem.cpp \
    cuglpixelbuffer.cpp \
    glrendererwidget.cpp \
    timelineview.cpp \
    histogram.cpp \
    textedit.cpp \
    glrenderer.cpp \
    highlighter.cpp \ 
    sourceedit.cpp \
    bartime.cpp

#SOURCES = src/*.cpp src/*.c

OTHER_FILES =

HEADERS  += mainwindow.h \
#    simplePBO.h \
#    helper_string.h \
     helper_cuda_drvapi.h \
#    cuda_drvapi_dynlink_cuda.h \
#    cuda_drvapi_dynlink_gl.h \
#    cuda_drvapi_dynlink.h \
    op.h \
    bartypes.h \
    opchain.h \
    opcomposer.h \
    gpu.h \
    cudacompil.h \
    brick.h \
    moduleslistmodel.h \
    gpumodule.h \
    modulelist.h \
    modulelistitem.h \
    compilation.h \
    cuglpixelbuffer.h \
    glrendererwidget.h \
    timelineview.h \
    histogram.h \
    textedit.h \
    glrenderer.h \
    highlighter.h \
    sourceedit.h \
    baracuda.h \
    bartime.h

FORMS    += mainwindow.ui about.ui

BUILDX = $$system(./buildnum.sh)
message("BUILD NUMBER:" $$BUILDX)
DEFINES += _BUILDNUM_=$$BUILDX CUDA_DIR=$$CUDA_DIR PATH=$$PATH NVCCBIN=$$CUDA_DIR/bin/nvcc

EVERYTHING = $$SOURCES $$HEADERS
message("Project $$TARGET contains the following files:")
message($$EVERYTHING)

DISTFILES += \
    readme.md \
    src/ui_mainwindow.h\
    kernels/channels.cu\
    kernels/test.cu\
    kernels/baracuda.hcu\
    kernels/vecmath.hcu\
    kernels/circle.cu\
    kernels/ribbons.cu\
    kernels/ribbons-glsl.cu

extraclean.commands = find . -name \"*.ptx\" -exec rm -v {} \\;
distclean.depends = extraclean
QMAKE_EXTRA_TARGETS += distclean extraclean


