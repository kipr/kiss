include(../../config.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(gcc_plugin)
DEPENDPATH += src
INCLUDEPATH += src ../../editor/src
DESTDIR = .

win32:CONFIG -= embed_manifest_dll
win32:TARGET = $$qtLibraryTarget(libgcc_plugin)

CONFIG -= debug debug_and_release
CONFIG += release

LIBS += -lqscintilla2

HEADERS += Gcc.h
SOURCES += Gcc.cpp

QMAKE_CLEAN += $(DESTDIR)/$(TARGET) src/*~

#################
# Extra targets #
#################
unix:  COPY = cp
!unix: COPY = copy

##############################
# Install proper target file #
##############################

macx: GCC_TARGET_SOURCE = gcc.target.mac
unix:!macx: GCC_TARGET_SOURCE = gcc.target.linux
windows: GCC_TARGET_SOURCE = gcc.target.windows

gcc-target.target = gcc-target
gcc-target.depends = gcc-target-file

gcc-target-file.target = gcc.target
gcc-target-file.commands = $${COPY} $${GCC_TARGET_SOURCE} gcc.target
gcc-target-file.depends = gcc-target-source

gcc-target-source.target = $${GCC_TARGET_SOURCE}

QMAKE_EXTRA_TARGETS += gcc-target gcc-target-file gcc-target-source

POST_TARGETDEPS += gcc-target

QMAKE_CLEAN += gcc.target

########
# glfw #
########

unix:{
GLFW_LIB = ../../external/lib/libglfw.a
GLFW_LIB_DEST = lib/libglfw.a
GLFW_HEADER = ../../external/include/GL/glfw.h
GLFW_HEADER_DEST = include/GL/glfw.h
}
!unix:{
GLFW_LIB = ..\\..\\external\\lib\\libglfw.a
GLFW_LIB_DEST = lib\\libglfw.a
GLFW_HEADER = ..\\..\\external\\include\\GL\\glfw.h
GLFW_HEADER_DEST = include\\GL\\glfw.h
}

glfw.target = glfw
glfw.depends = glfw-lib glfw-header

glfw-lib.target = $${GLFW_LIB_DEST}
glfw-lib.commands = $${COPY} $${GLFW_LIB} $${GLFW_LIB_DEST}
glfw-lib.depends = $${GLFW_LIB}

glfw-header.target = $${GLFW_HEADER_DEST}
glfw-header.commands = $${COPY} $${GLFW_HEADER} $${GLFW_HEADER_DEST}
glfw-header.depends = $${GLFW_HEADER}

QMAKE_EXTRA_TARGETS += glfw glfw-lib glfw-header

POST_TARGETDEPS += glfw

QMAKE_CLEAN += $${GLFW_LIB_DEST} $${GLFW_HEADER_DEST}


########
# GLee #
########

unix:{
GLEE_LIB = ../../external/lib/libGLee.a
GLEE_LIB_DEST = lib/libGLee.a
GLEE_HEADER = ../../external/include/GL/GLee.h
GLEE_HEADER_DEST = include/GL/GLee.h
}
!unix:{
GLEE_LIB = ..\\..\\external\\lib\\libGLee.a
GLEE_LIB_DEST = lib\\libGLee.a
GLEE_HEADER = ..\\..\\external\\include\\GL\\GLee.h
GLEE_HEADER_DEST = include\\GL\\GLee.h
}

glee.target = glee
glee.depends = glee-lib glee-header

glee-lib.target = $${GLEE_LIB_DEST}
glee-lib.commands = $${COPY} $${GLEE_LIB} $${GLEE_LIB_DEST}
glee-lib.depends = $${GLEE_LIB}

glee-header.target = $${GLEE_HEADER_DEST}
glee-header.commands = $${COPY} $${GLEE_HEADER} $${GLEE_HEADER_DEST}
glee-header.depends = $${GLEE_HEADER}

QMAKE_EXTRA_TARGETS += glee glee-lib glee-header

POST_TARGETDEPS += glee

QMAKE_CLEAN += $${GLEE_LIB_DEST} $${GLEE_HEADER_DEST}

###########
# libkiss #
###########

# Way ugly but nicer in the end

unix: {
KISS_LIB = ../../libraries/kiss/libkiss.a
KISS_LIB_DEST = lib/libkiss.a

KISS_HEADER_COMPAT = ../../libraries/kiss/include/kiss-compat.h
KISS_HEADER_COMPAT_DEST = include/kiss-compat.h

KISS_HEADER_SERIAL = ../../libraries/kiss/include/kiss-serial.h
KISS_HEADER_SERIAL_DEST = include/kiss-serial.h

KISS_HEADER_GRAPHICS = ../../libraries/kiss/include/kiss-graphics.h
KISS_HEADER_GRAPHICS_DEST = include/kiss-graphics.h

KISS_HEADER_INPUT = ../../libraries/kiss/include/kiss-input.h
KISS_HEADER_INPUT_DEST = include/kiss-input.h
}
!unix:{
KISS_LIB = ..\\..\\libraries\\kiss\\libkiss.a
KISS_LIB_DEST = lib\\libkiss.a

KISS_HEADER_COMPAT = ..\\..\\libraries\\kiss\\include\\kiss-compat.h
KISS_HEADER_COMPAT_DEST = include\\kiss-compat.h

KISS_HEADER_SERIAL = ..\\..\\libraries\\kiss\\include\\kiss-serial.h
KISS_HEADER_SERIAL_DEST = include\\kiss-serial.h

KISS_HEADER_GRAPHICS = ..\\..\\libraries\\kiss\\include\\kiss-graphics.h
KISS_HEADER_GRAPHICS_DEST = include\\kiss-graphics.h

KISS_HEADER_INPUT = ..\\..\\libraries\\kiss\\include\\kiss-input.h
KISS_HEADER_INPUT_DEST = include\\kiss-input.h
}

kiss.target = kiss
kiss.depends = kiss-lib kiss-headers

kiss-headers.target = kiss-headers
kiss-headers.depends = kiss-header-compat \
						kiss-header-serial \
						kiss-header-graphics \
						kiss-header-input

kiss-lib.target = $${KISS_LIB_DEST}
kiss-lib.commands = $${COPY} $${KISS_LIB} $${KISS_LIB_DEST}
kiss-lib.depends = $${KISS_LIB}

kiss-header-compat.target = $${KISS_HEADER_COMPAT_DEST}
kiss-header-compat.commands = $${COPY} $${KISS_HEADER_COMPAT} $${KISS_HEADER_COMPAT_DEST}
kiss-header-compat.depends = $${KISS_HEADER_COMPAT}

kiss-header-serial.target = $${KISS_HEADER_SERIAL_DEST}
kiss-header-serial.commands = $${COPY} $${KISS_HEADER_SERIAL} $${KISS_HEADER_SERIAL_DEST}
kiss-header-serial.depends = $${KISS_HEADER_SERIAL}

kiss-header-graphics.target = $${KISS_HEADER_GRAPHICS_DEST}
kiss-header-graphics.commands = $${COPY} $${KISS_HEADER_GRAPHICS} $${KISS_HEADER_GRAPHICS_DEST}
kiss-header-graphics.depends = $${KISS_HEADER_GRAPHICS}

kiss-header-input.target = $${KISS_HEADER_INPUT_DEST}
kiss-header-input.commands = $${COPY} $${KISS_HEADER_INPUT} $${KISS_HEADER_INPUT_DEST}
kiss-header-input.depends = $${KISS_HEADER_INPUT}

QMAKE_EXTRA_TARGETS += kiss kiss-lib kiss-headers kiss-header-compat kiss-header-serial kiss-header-graphics kiss-header-input

POST_TARGETDEPS += kiss

QMAKE_CLEAN += $${KISS_LIB_DEST} \
				$${KISS_HEADER_COMPAT_DEST} \
				$${KISS_HEADER_SERIAL_DEST} \
				$${KISS_HEADER_GRAPHICS_DEST} \
				$${KISS_HEADER_INPUT_DEST}

##############
# Create Lib #
##############

unix:{
CREATE_HEADER = ../../libraries/create/kiss-create.h
CREATE_HEADER_DEST = include/kiss-create.h
}
!unix:{
CREATE_HEADER = ..\\..\\libraries\\create\\kiss-create.h
CREATE_HEADER_DEST = include\\kiss-create.h
}

create.target = create
create.depends = create-header

create-header.target = $${CREATE_HEADER_DEST}
create-header.commands = $${COPY} $${CREATE_HEADER} $${CREATE_HEADER_DEST}
create-header.depends = $${CREATE_HEADER}

QMAKE_EXTRA_TARGETS += create create-header

POST_TARGETDEPS += create

QMAKE_CLEAN += $${CREATE_HEADER_DEST}

######################
# Install Directives #
######################

unix: GCC_TARGET_FILE_INSTALL = ../../$${INSTALL_BASE}/targets/gcc/gcc.target
!unix: GCC_TARGET_FILE_INSTALL = ..\\..\\$${INSTALL_BASE}\\targets\\gcc\\gcc.target

target_base.files = gcc.api \
					manual.html \
					template.c
target_base.path = ../../$${INSTALL_BASE}/targets/gcc
target_base.extra = $${COPY} gcc.target $${GCC_TARGET_FILE_INSTALL}

target_include.files = ../../libraries/kiss/include/kiss-compat.h \
					   ../../libraries/kiss/include/kiss-input.h \
					   ../../libraries/kiss/include/kiss-graphics.h \
					   ../../libraries/kiss/include/kiss-serial.h \
					   ../../libraries/create/kiss-create.h
target_include.path = ../../$${INSTALL_BASE}/targets/gcc/include

target_gl.files = ../../external/include/GL/glfw.h \
				  ../../external/include/GL/GLee.h
target_gl.path = ../../$${INSTALL_BASE}/targets/gcc/include/GL

target_lib.files = ../../external/lib/libglfw.a \
				   ../../external/lib/libGLee.a \
				   ../../libraries/kiss/libkiss.a
target_lib.path = ../../$${INSTALL_BASE}/targets/gcc/lib

target.path = ../../$${INSTALL_BASE}/targets/gcc

INSTALLS += target target_base target_include target_gl target_lib

#################################
# Windows specifing MinGW stuff #
#################################

win32: {
mingw.path = ../../$${INSTALL_BASE}/targets/gcc
mingw.extra = xcopy /e /i mingw ..\\..\\$${INSTALL_BASE}\\targets\\gcc\\mingw

INSTALLS += mingw
}

#############################
# OS X Specific dylib stuff #
#############################

macx:{
install_lib_names.extra = sh ../../scripts/osx_install_names.sh ../../$${INSTALL_BASE}/targets/gcc/libgcc_plugin.dylib QtCore QtGui
install_lib_names.path = ../../$${INSTALL_BASE}

INSTALLS += install_lib_names
}
