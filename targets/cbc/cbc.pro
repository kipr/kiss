include(../../config.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(cbc_plugin)
DEPENDPATH += src
INCLUDEPATH += src ../../editor/src ../../libraries/extraGUI/src
DESTDIR = .

win32:CONFIG -= embed_manifest_dll
win32:TARGET = $$qtLibraryTarget(libcbc_plugin)

CONFIG -= debug debug_and_release
CONFIG += release

LIBS += -lqscintilla2 -L../../libraries/extraGUI -lextraGUI

HEADERS += CBC.h SerialClient.h QSerialPort.h
SOURCES += CBC.cpp SerialClient.cpp QSerialPort.cpp

QMAKE_CLEAN += $(DESTDIR)/$(TARGET) src/*~

#################
# Extra targets #
#################
unix:  COPY = cp
!unix: COPY = copy

unix: {
lib_dir.target = lib
lib_dir.commands = mkdir -p lib
include_dir.target = include
include_dir.commands = mkdir -p include
}

QMAKE_EXTRA_TARGETS += lib_dir include_dir
POST_TARGETDEPS += $$lib_dir.target $$include_dir.target

##############################
# Install proper target file #
##############################

macx: CBC_TARGET_SOURCE = cbc.target.mac
unix:!macx: CBC_TARGET_SOURCE = cbc.target.linux
windows: CBC_TARGET_SOURCE = cbc.target.windows

cbc-target.target = cbc-target
cbc-target.depends = cbc-target-file

cbc-target-file.target = cbc.target
cbc-target-file.commands = $${COPY} $${CBC_TARGET_SOURCE} cbc.target
cbc-target-file.depends = cbc-target-source

cbc-target-source.target = $${CBC_TARGET_SOURCE}

QMAKE_EXTRA_TARGETS += cbc-target cbc-target-file cbc-target-source

POST_TARGETDEPS += cbc-target

QMAKE_CLEAN += cbc.target

######################
# Install Directives #
######################

unix: CBC_TARGET_FILE_INSTALL = ../../$${INSTALL_BASE}/targets/cbc/cbc.target
!unix: CBC_TARGET_FILE_INSTALL = ..\\..\\$${INSTALL_BASE}\\targets\\cbc\\cbc.target

target_base.files = cbc.api \
					template.c
target_base.path = ../../$${INSTALL_BASE}/targets/cbc
target_base.extra = $${COPY} cbc.target $${CBC_TARGET_FILE_INSTALL}

target_include.files = ../../libraries/kiss_sim/include/kiss-sim.h include/botball.c
target_include.path = ../../$${INSTALL_BASE}/targets/cbc/include

target_lib.files = ../../libraries/kiss_sim/libkiss_sim.a
target_lib.path = ../../$${INSTALL_BASE}/targets/cbc/lib

target.path = ../../$${INSTALL_BASE}/targets/cbc

target_manual.files = manual/cbcmanual.html \
                      manual/image002.gif \
                      manual/image003.gif \
                      manual/image004.gif \
                      manual/image005.gif \
                      manual/image006.gif \
                      manual/image009.gif \
                      manual/image010.gif \
                      manual/image012.gif \
                      manual/sonar.jpg
target_manual.path = ../../$${INSTALL_BASE}/targets/cbc/manual
                      

INSTALLS += target target_base target_include target_lib target_manual

##################
# kiss_sim stuff #
##################

# Way ugly but nicer in the end

unix: {
KISS_SIM_LIB = ../../libraries/kiss_sim/libkiss_sim.a
KISS_SIM_LIB_DEST = lib/libkiss_sim.a

KISS_SIM_HEADER = ../../libraries/kiss_sim/include/kiss-sim.h
KISS_SIM_HEADER_DEST = include/kiss-sim.h
}

!unix:{
KISS_SIM_LIB = ..\\..\\libraries\\kiss_sim\\libkiss_sim.a
KISS_SIM_LIB_DEST = lib\\libkiss_sim.a

KISS_SIM_HEADER = ..\\..\\libraries\\kiss_sim\\include\\kiss-sim.h
KISS_SIM_HEADER_DEST = include\\kiss-sim.h
}

kiss_sim.target = kiss_sim
kiss_sim.depends = kiss_sim-lib kiss_sim-headers

kiss_sim-headers.target = kiss_sim-headers
kiss_sim-headers.depends = kiss_sim-header

kiss_sim-lib.target = $${KISS_SIM_LIB_DEST}
kiss_sim-lib.commands = $${COPY} $${KISS_SIM_LIB} $${KISS_SIM_LIB_DEST}
kiss_sim-lib.depends = $${KISS_SIM_LIB}

kiss_sim-header.target = $${KISS_SIM_HEADER_DEST}
kiss_sim-header.commands = $${COPY} $${KISS_SIM_HEADER} $${KISS_SIM_HEADER_DEST}
kiss_sim-header.depends = $${KISS_SIM_HEADER}

QMAKE_EXTRA_TARGETS += kiss_sim kiss_sim-lib kiss_sim-headers kiss_sim-header

POST_TARGETDEPS += kiss_sim

QMAKE_CLEAN += $${KISS_SIM_LIB_DEST} \
				$${KISS_SIM_HEADER_DEST}

#############################
# OS X Specific dylib stuff #
#############################

macx:{
install_lib_names.extra = sh ../../scripts/osx_install_names.sh ../../$${INSTALL_BASE}/targets/cbc/libcbc_plugin.dylib QtCore QtGui
install_lib_names.path = ../../$${INSTALL_BASE}

INSTALLS += install_lib_names
}
