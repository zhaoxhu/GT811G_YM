
include ../makefile.rc
MOD_NAME=cache
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/cmn/plat

ECOS_INSTALL_HOME=$(MOD_HOME)/../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -nostdlib -fno-inline -Wno-strict-aliasing \
-mthumb-interwork -marm -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/cache.c

MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/include \
-I$(IROS_CONFIG_HOME)

include $(SWHOME)/tools/support/build/makefile.mod_start
