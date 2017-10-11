
include ../makefile.rc
MOD_NAME=osal
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/cmn/plat/osal

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/osal_common.c \
$(MOD_SRC_HOME)/ecos/osal_api_core.c

MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/include


include $(SWHOME)/tools/support/build/makefile.mod_start
