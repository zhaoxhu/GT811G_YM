
include ../makefile.rc
MOD_NAME=loopdetect
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/onu/apps/lynx/loopdetect/loop_detect.c


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/loopdetect \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn


include $(SWHOME)/tools/support/build/makefile.mod_start

