
include ../makefile.rc
MOD_NAME=aal
MOD_SRC_HOME=$(SWHOME)/src/onu/aal/lynxd
MOD_TARGET=library

ECOS_INSTALL_HOME=../iros/install

MOD_CFILES= \
$(MOD_SRC_HOME)/aal_sys.c \
$(MOD_SRC_HOME)/aal_enc.c \
$(MOD_SRC_HOME)/aal_bm.c \
$(MOD_SRC_HOME)/aal_flow.c \
$(MOD_SRC_HOME)/aal_mpcp.c \
$(MOD_SRC_HOME)/aal_pon.c \
$(MOD_SRC_HOME)/aal_l2.c \
$(MOD_SRC_HOME)/aal_cls.c \
$(MOD_SRC_HOME)/aal_cls_util.c \
$(MOD_SRC_HOME)/aal_ma.c \
$(MOD_SRC_HOME)/aal_uni.c \
$(MOD_SRC_HOME)/aal_ptp.c \
$(MOD_SRC_HOME)/aal_fec.c \
$(MOD_SRC_HOME)/aal_serdes.c 

CFILES=$(MOD_CFILES)

MOD_INCLUDES= -I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/plat/lynxd \
-I$(SWHOME)/src/onu/aal/lynxd \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/3rd/driver/realtek \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn



IROS_CONFIG_H=$(shell pwd)/../iros_config.h
MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -D_LITTLE_ENDIAN\
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

include $(SWHOME)/tools/support/build/makefile.mod_start

