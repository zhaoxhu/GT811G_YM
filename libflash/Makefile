
include ../makefile.rc

MOD_NAME=flash
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/cmn/plat/flash

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -marm -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/flash_driver.c \
$(MOD_SRC_HOME)/multi_image_file.c \
$(MOD_SRC_HOME)/flash_hwr_init.c \
$(MOD_SRC_HOME)/iros_flash_oper.c \
$(SWHOME)/src/cmn/plat/flash/flash_cli.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_flash_oper.c

MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/cmn/plat \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn 

include $(SWHOME)/tools/support/build/makefile.mod_start

