# SPDX-License-Identifier: GPL-3.0-only
# (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
# Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

DCAROOT   ?= ${HOME}/dca
PREREQ_PATH  = ${DCAROOT}/prerequisites

CXXSTDLVL ?= c++17
CXXWEXTRA ?=

CXXFLAGS  ?= -g -O3 -Wall -Wextra -Wpedantic
CXXFLAGS  += ${CXXWEXTRA} -std=${CXXSTDLVL}
CXXFLAGS  += -I../../include
CXXFLAGS  += -I${PREREQ_PATH}/include
#
LINK.o     = ${LINK.cpp}
LDFLAGS    = -g -rdynamic
LDLIBS     = -L ${PREREQ_PATH}/lib -lzmq -lfmt
LDLIBS    += -lpthread -latomic
# libs
LIB_NAME   = ../../lib/libframework.a
# lib input
LOBJ_all    = AuthRegistry.o
LOBJ_all   += Base.o
LOBJ_all   += ClassRegistry.o
LOBJ_all   += Context.o
LOBJ_all   += CoutMessage.o
LOBJ_all   += ChronoHelper.o
LOBJ_all   += Exception.o
LOBJ_all   += FormatBits.o
LOBJ_all   += FormatHelper.o
LOBJ_all   += Logger.o LoggerStream.o
LOBJ_all   += LoggerSink.o
LOBJ_all   += LoggerSinkFile.o  LoggerSinkMonitor.o LoggerSinkSyslog.o
LOBJ_all   += Master.o
LOBJ_all   += Monitor.o
LOBJ_all   += MonitorSink.o MonitorSinkFile.o
LOBJ_all   += MonitorSinkInflux1.o MonitorSinkInflux2.o
LOBJ_all   += ProxyBase.o
LOBJ_all   += ProxyRegistryBase.o
LOBJ_all   += PThreadHelper.o
LOBJ_all   += RpcConvert.o
LOBJ_all   += SysCallException.o
LOBJ_all   += SystemProxy.o
LOBJ_all   += TesterEnd.o TesterEndProxy.o
LOBJ_all   += TesterMain.o TesterMainProxy.o
LOBJ_all   += TesterMid.o TesterMidProxy.o
LOBJ_all   += TesterTop.o TesterTopProxy.o
LOBJ_all   += UintHelper.o
LOBJ_all   += ValueCheck.o
LOBJ_all   += Worker.o
LOBJ_all   += ZmqHelper.o
# executables
EOBJ_all   = dca_tester.o
#
OBJ_all    = $(LOBJ_all) $(EOBJ_all)
DEP_all    = $(OBJ_all:.o=.dep)
EXE_all    = $(EOBJ_all:.o=)
#
build : $(EXE_all)
#
${LIB_NAME} : $(LOBJ_all)
	$(AR) -scrUuv $@ $?
#
dca_tester : dca_tester.o BuildInfo.o ${LIB_NAME}
#
BuildInfo.cpp : dca_tester.o ${LIB_NAME}
	../../tools/bin/create_buildinfo.sh
#
include ../../tools/make/generic_dep.mk
include ../../tools/make/dontincdep.mk
#
# The magic auto-dependency include
#
ifndef DONTINCDEP
include $(DEP_all)
endif
#
# cleanup phonies:
#
.PHONY : clean cleangcov cleandep distclean
#
cleangcov :
	@ rm -f *.gcda *.gcno *.gcov
	@ echo "gcov files removed"
#
clean : cleangcov
	@ rm -f $(OBJ_all)
	@ rm -f core
	@ echo "Object files removed"
#
cleandep :
	@ rm -f $(DEP_all)
	@ echo "Dependency files removed"
#
distclean : clean cleandep
	@ rm -f ${LIB_NAME}
	@ rm -f $(EXE_all)
	@ echo "Executables and library files removed"
