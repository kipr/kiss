include(../../config.pri)

TEMPLATE = lib
TARGET = glfw
DEPENDPATH += glfw/lib
INCLUDEPATH += glfw/lib

DESTDIR = ../lib

CONFIG -= qt
CONFIG += staticlib warn_off

unix: DEFINES += _GLFW_HAS_PTHREAD _GLFW_HAS_SCHED_YIELD

linux-g++: DEFINES += _GLFW_HAS_XRANDR

windows: DEFINES -= UNICODE

macx: DEFINES += DISABLE_UNBUNDLED_MESSAGE

SOURCES += enable.c \
	   fullscreen.c \
	   glext.c \
	   image.c \
	   init.c \
	   input.c \
	   joystick.c \
	   stream.c \
	   tga.c \
	   thread.c \
	   time.c \
	   window.c

HEADERS += internal.h

macx: {
DEPENDPATH += glfw/lib/macosx
INCLUDEPATH += glfw/lib/macosx
SOURCES += macosx_enable.c \
	   macosx_fullscreen.c \
	   macosx_glext.c \
	   macosx_init.c \
	   macosx_joystick.c \
	   macosx_thread.c \
	   macosx_time.c \
	   macosx_window.c
HEADERS += platform.h
}

unix:!macx {
DEPENDPATH += glfw/lib/x11
INCLUDEPATH += glfw/lib/x11
SOURCES += x11_enable.c \
	   x11_fullscreen.c \
	   x11_glext.c \
	   x11_init.c \
	   x11_joystick.c \
	   x11_keysym2unicode.c \
	   x11_thread.c \
	   x11_time.c \
	   x11_window.c
HEADERS += platform.h

}

win32: {
DEPENDPATH += glfw/lib/win32
INCLUDEPATH += glfw/lib/win32
SOURCES += win32_enable.c \
	   win32_fullscreen.c \
	   win32_glext.c \
	   win32_init.c \
	   win32_joystick.c \
	   win32_thread.c \
	   win32_time.c \
	   win32_window.c
HEADERS += platform.h
}

copy_header.target = ../include/GL/glfw.h

unix: {
copy_header.commands = cp glfw/include/GL/glfw.h ../include/GL
}

win32: {
copy_header.commands = copy glfw\\include\\GL\\glfw.h ..\\include\\GL
}

QMAKE_EXTRA_TARGETS = copy_header
POST_TARGETDEPS += $$copy_header.target

QMAKE_CLEAN += $${DESTDIR}/lib$${TARGET}.a ../include/GL/glfw.h
