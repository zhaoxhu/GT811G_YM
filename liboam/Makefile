
include ../makefile.rc
MOD_NAME=oam
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/cmn/apps/oam/oam_sdl_if.c  \
$(SWHOME)/src/cmn/apps/oam/oam_core.c \
$(SWHOME)/src/cmn/apps/oam/oam_plat_if.c \
$(SWHOME)/src/cmn/apps/oam/oam_control.c \
$(SWHOME)/src/cmn/apps/oam/oam_mux_par.c \
$(SWHOME)/src/cmn/apps/oam/oam_client.c \
$(SWHOME)/src/cmn/apps/oam/oam_supp.c \
$(SWHOME)/src/cmn/apps/oam/oam_timer.c \
$(SWHOME)/src/cmn/apps/oam/oam_event.c \
$(SWHOME)/src/cmn/apps/oam/oam_oob_mgmt.c \
$(SWHOME)/src/cmn/apps/oam/oam_main.c \
$(SWHOME)/src/onu/apps/lynx/oam/ctc/oam_ctc_client.c \
$(SWHOME)/src/onu/apps/lynx/oam/ctc/app_alarm_ctc.c  \
$(SWHOME)/src/onu/apps/lynx/oam/ctc/app_stats_ctc.c  \
$(SWHOME)/src/onu/apps/lynx/oam/ctc/app_oam_ctc.c   \
$(SWHOME)/src/onu/apps/lynx/oam/ctc/app_oam_ctc_adapt.c   \
$(SWHOME)/src/onu/apps/lynx/oam/cs/app_oam_cs.c \
$(SWHOME)/src/onu/apps/lynx/pwrs/power_saving.c \
$(SWHOME)/src/onu/apps/gwd/app_oam_gwd.c	\
$(SWHOME)/src/onu/apps/gwd/timer.c	\
$(SWHOME)/src/onu/apps/gwd/oam_pty.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_eth_loop_detect.c \
$(SWHOME)/src/onu/apps/gwd/gwd_eth_loop_detect_realize.c \


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/apps/oam/include \
-I$(SWHOME)/src/cmn/apps/core \
-I$(SWHOME)/src/cmn/apps/event \
-I$(SWHOME)/src/cmn/apps/mcast/inc \
-I$(SWHOME)/src/onu/apps/lynx/mcast \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/plat/lynxd \
-I$(SWHOME)/src/onu/plat/lynxd/mports/ctc \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/sdl/cmn/cmn \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/aal/lynxd/ \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/apps/gwd \
-I$(SWHOME)/src/onu/apps/lynx/pppoe \
-I$(SWHOME)/src/onu/apps/lynx/ipintf \
-I$(SWHOME)/src/onu/apps/lynx/oam/cs \
-I$(SWHOME)/src/onu/apps/lynx/oam \
-I$(SWHOME)/src/onu/apps/lynx/pwrs \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/onu/apps/lynx/loopdetect \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn

include $(SWHOME)/tools/support/build/makefile.mod_start

