include(../../config.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(cbc2_plugin)
DEPENDPATH += ../cbc/src
INCLUDEPATH += ../cbc/src ../../editor/src ../../libraries/extraGUI/src
DESTDIR = .

win32:CONFIG -= embed_manifest_dll
win32:TARGET = $$qtLibraryTarget(libcbc2_plugin)

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

macx: CBC_TARGET_SOURCE = cbc2.target.mac
unix:!macx: CBC_TARGET_SOURCE = cbc2.target.linux
windows: CBC_TARGET_SOURCE = cbc2.target.windows

cbc-target.target = cbc-target
cbc-target.depends = cbc-target-file

cbc-target-file.target = cbc2.target
cbc-target-file.commands = $${COPY} $${CBC_TARGET_SOURCE} cbc2.target
cbc-target-file.depends = cbc-target-source

cbc-target-source.target = $${CBC_TARGET_SOURCE}

QMAKE_EXTRA_TARGETS += cbc-target cbc-target-file cbc-target-source

POST_TARGETDEPS += cbc-target

QMAKE_CLEAN += cbc2.target

######################
# Install Directives #
######################

unix: CBC_TARGET_FILE_INSTALL = ../../$${INSTALL_BASE}/targets/cbc2/cbc2.target
!unix: CBC_TARGET_FILE_INSTALL = ..\\..\\$${INSTALL_BASE}\\targets\\cbc2\\cbc2.target

target_base.files = cbc2.api \
					template.c
target_base.path = ../../$${INSTALL_BASE}/targets/cbc2
target_base.extra = $${COPY} cbc2.target $${CBC_TARGET_FILE_INSTALL}

exists(../../trademarks) {
target_include.files = ../../trademarks/simulator/cbc2_sim/include/cbc2-sim.h ../../trademarks/simulator/cbc2_sim/src/botball.c

target_lib.files = ../../trademarks/simulator/cbc2_sim/libcbc2_sim.a
}
!exists(../../trademarks) {
target_include.files = ../../libraries/cbc2_sim/include/cbc2-sim.h ../../libraries/cbc2_sim/src/botball.c
target_lib.files = ../../libraries/cbc2_sim/libcbc2_sim.a
}

target_include.path = ../../$${INSTALL_BASE}/targets/cbc2/include
target_lib.path = ../../$${INSTALL_BASE}/targets/cbc2/lib

target.path = ../../$${INSTALL_BASE}/targets/cbc2

target_manual.files = manual/CBCCSS.css \
											manual/CBCCSS_NOSCRIPT.css \
											manual/cbcmanual.html \
											manual/fdl-1.3.txt \
											manual/KISSCBCManualBody.htm \
											manual/KISSCBCManualTable.htm 
											
target_manual.path = ../../$${INSTALL_BASE}/targets/cbc2/manual

target_manual_images.files = manual/Images/box-minus.bmp \
														 manual/Images/box-null.GIF \
														 manual/Images/box-plus.bmp \
														 manual/Images/CBC_front_ports.jpg \
														 manual/Images/image002.gif \
														 manual/Images/image003.gif \
														 manual/Images/image004.gif \
														 manual/Images/image005.gif \
														 manual/Images/image006.gif \
														 manual/Images/image010.gif \
														 manual/Images/sonar.jpg

target_manual_images.path = ../../$${INSTALL_BASE}/targets/cbc2/manual/Images


INSTALLS += target target_base target_include target_lib target_manual target_manual_images

#################
# cbc2_sim stuff #
#################

# Way ugly but nicer in the end

exists(../../trademarks) {
unix: {
CBC2_SIM_LIB = ../../trademarks/simulator/cbc2_sim/libcbc2_sim.a
CBC2_SIM_LIB_DEST = lib/libcbc2_sim.a

CBC2_SIM_HEADER = ../../trademarks/simulator/cbc2_sim/include/cbc2-sim.h
CBC2_SIM_HEADER_DEST = include/cbc2-sim.h
}

!unix:{
CBC2_SIM_LIB = ..\\..\\trademarks\\simulator\\cbc2_sim\\libcbc2_sim.a
CBC2_SIM_LIB_DEST = lib\\libcbc2_sim.a

CBC2_SIM_HEADER = ..\\..\\trademarks\\simulator\\cbc2_sim\\include\\cbc2-sim.h
CBC2_SIM_HEADER_DEST = include\\cbc2-sim.h
}
}

!exists(../../trademarks) {
unix: {
CBC2_SIM_LIB = ../../libraries/cbc2_sim/libcbc2_sim.a
CBC2_SIM_LIB_DEST = lib/libcbc2_sim.a

CBC2_SIM_HEADER = ../../libraries/cbc2_sim/include/cbc2-sim.h
CBC2_SIM_HEADER_DEST = include/cbc2-sim.h
}

!unix:{
CBC2_SIM_LIB = ..\\..\\libraries\\cbc2_sim\\libcbc2_sim.a
CBC2_SIM_LIB_DEST = lib\\libcbc2_sim.a

CBC2_SIM_HEADER = ..\\..\\libraries\\cbc2_sim\\include\\cbc2-sim.h
CBC2_SIM_HEADER_DEST = include\\cbc2-sim.h
}

}

cbc2_sim.target = cbc2_sim
cbc2_sim.depends = cbc2_sim-lib cbc2_sim-headers

cbc2_sim-headers.target = cbc2_sim-headers
cbc2_sim-headers.depends = cbc2_sim-header

cbc2_sim-lib.target = $${CBC2_SIM_LIB_DEST}
cbc2_sim-lib.commands = $${COPY} $${CBC2_SIM_LIB} $${CBC2_SIM_LIB_DEST}
cbc2_sim-lib.depends = $${CBC2_SIM_LIB}

cbc2_sim-header.target = $${CBC2_SIM_HEADER_DEST}
cbc2_sim-header.commands = $${COPY} $${CBC2_SIM_HEADER} $${CBC2_SIM_HEADER_DEST}
cbc2_sim-header.depends = $${CBC2_SIM_HEADER}

QMAKE_EXTRA_TARGETS += cbc2_sim cbc2_sim-lib cbc2_sim-headers cbc2_sim-header

POST_TARGETDEPS += cbc2_sim

QMAKE_CLEAN += $${CBC2_SIM_LIB_DEST} $${CBC2_SIM_HEADER_DEST}

#############################
# OS X Specific dylib stuff #
#############################

macx:{
install_lib_names.extra = sh ../../scripts/osx_install_names.sh ../../$${INSTALL_BASE}/targets/cbc2/libcbc2_plugin.dylib QtCore.framework/Versions/4/QtCore QtGui.framework/Versions/4/QtGui
install_lib_names.path = ../../$${INSTALL_BASE}

INSTALLS += install_lib_names
}