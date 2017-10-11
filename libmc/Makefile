
include ../makefile.rc
MOD_NAME=mc
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/cmn/apps/mcast/src/mc_src.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_devm.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_init.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_grp.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_test.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_fdb.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_timer.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_parser.c \
$(SWHOME)/src/cmn/apps/mcast/src/igmp_pkt.c \
$(SWHOME)/src/cmn/apps/mcast/src/mld_pkt.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_mgmt.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_api.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_process.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_query.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_report.c \
$(SWHOME)/src/cmn/apps/mcast/src/mc_util.c \
$(SWHOME)/src/onu/apps/lynx/mcast/port_igmp.c \
$(SWHOME)/src/onu/apps/lynx/mcast/igmp_cmd.c \
$(SWHOME)/src/onu/apps/lynx/mcast/mc_ctc.c \
$(SWHOME)/src/onu/apps/lynx/mcast/mc_ctcctrl.c



MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/apps/mcast/inc \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/cmn/apps/oam/include \
-I$(SWHOME)/src/cmn/apps/core \
-I$(SWHOME)/src/cmn/apps/event \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/plat/lynxd \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/aal/lynxd/ \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/apps/lynx/oam \
-I$(SWHOME)/src/onu/cmd \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn	\
-I$(SWHOME)/src/onu/apps/gwd


include $(SWHOME)/tools/support/build/makefile.mod_start

