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

LIBS += -L../../libraries/extraGUI -lextraGUI -lqscintilla2 

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

exists(../../trademarks) {
target_include.files = ../../trademarks/simulator/cbc_sim/include/cbc-sim.h include/botball.c

target_lib.files = ../../trademarks/simulator/cbc_sim/libcbc_sim.a
}
!exists(../../trademarks) {
target_include.files = ../../libraries/cbc_sim/include/cbc-sim.h include/botball.c
target_lib.files = ../../libraries/cbc_sim/libcbc_sim.a
}

target_include.path = ../../$${INSTALL_BASE}/targets/cbc/include
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

#################
# cbc_sim stuff #
#################

# Way ugly but nicer in the end

exists(../../trademarks) {
unix: {
CBC_SIM_LIB = ../../trademarks/simulator/cbc_sim/libcbc_sim.a
CBC_SIM_LIB_DEST = lib/libcbc_sim.a

CBC_SIM_HEADER = ../../trademarks/simulator/cbc_sim/include/cbc-sim.h
CBC_SIM_HEADER_DEST = include/cbc-sim.h
}

!unix:{
CBC_SIM_LIB = ..\\..\\trademarks\\simulator\\cbc_sim\\libcbc_sim.a
CBC_SIM_LIB_DEST = lib\\libcbc_sim.a

CBC_SIM_HEADER = ..\\..\\trademarks\\simulator\\cbc_sim\\include\\cbc-sim.h
CBC_SIM_HEADER_DEST = include\\cbc-sim.h
}
}

!exists(../../trademarks) {
unix: {
CBC_SIM_LIB = ../../libraries/cbc_sim/libcbc_sim.a
CBC_SIM_LIB_DEST = lib/libcbc_sim.a

CBC_SIM_HEADER = ../../libraries/cbc_sim/include/cbc-sim.h
CBC_SIM_HEADER_DEST = include/cbc-sim.h
}

!unix:{
CBC_SIM_LIB = ..\\..\\libraries\\cbc_sim\\libcbc_sim.a
CBC_SIM_LIB_DEST = lib\\libcbc_sim.a

CBC_SIM_HEADER = ..\\..\\libraries\\cbc_sim\\include\\cbc-sim.h
CBC_SIM_HEADER_DEST = include\\cbc-sim.h
}

}

cbc_sim.target = cbc_sim
cbc_sim.depends = cbc_sim-lib cbc_sim-headers

cbc_sim-headers.target = cbc_sim-headers
cbc_sim-headers.depends = cbc_sim-header

cbc_sim-lib.target = $${CBC_SIM_LIB_DEST}
cbc_sim-lib.commands = $${COPY} $${CBC_SIM_LIB} $${CBC_SIM_LIB_DEST}
cbc_sim-lib.depends = $${CBC_SIM_LIB}

cbc_sim-header.target = $${CBC_SIM_HEADER_DEST}
cbc_sim-header.commands = $${COPY} $${CBC_SIM_HEADER} $${CBC_SIM_HEADER_DEST}
cbc_sim-header.depends = $${CBC_SIM_HEADER}

QMAKE_EXTRA_TARGETS += cbc_sim cbc_sim-lib cbc_sim-headers cbc_sim-header

POST_TARGETDEPS += cbc_sim

QMAKE_CLEAN += $${CBC_SIM_LIB_DEST} \
				$${CBC_SIM_HEADER_DEST}

#############################
# OS X Specific dylib stuff #
#############################

macx:{
install_lib_names.extra = sh ../../scripts/osx_install_names.sh ../../$${INSTALL_BASE}/targets/cbc/libcbc_plugin.dylib QtCore QtGui
install_lib_names.path = ../../$${INSTALL_BASE}

INSTALLS += install_lib_names
}
