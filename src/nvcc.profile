
TOP              = /usr/local/cuda7.5

NVVMIR_LIBRARY_DIR = $(TOP)/nvvm/libdevice

LD_LIBRARY_PATH += $(TOP)/lib:
PATH            += $(TOP)/open64/bin:$(TOP)/nvvm/bin:$(_HERE_):

INCLUDES        +=  "-I$(TOP)/$(_TARGET_DIR_)/include" $(_SPACE_)

LIBRARIES        =+ $(_SPACE_) "-L$(TOP)/$(_TARGET_DIR_)/lib$(_TARGET_SIZE_)/stubs" "-L$(TOP)/$(_TARGET_DIR_)/lib$(_TARGET_SIZE_)"

CUDAFE_FLAGS    +=
PTXAS_FLAGS     +=
