include(../../config.pri)

TEMPLATE = lib
TARGET = 
DEPENDPATH += src
INCLUDEPATH += ../../external/include ../../editor/src
CONFIG += qt staticlib

win32 { 
    INCLUDEPATH += QSci \
    CONFIG -= embed_manifest_exe
}

LIBS += -lqscintilla2
win32:LIBS += -lshell32

# Input
HEADERS += src/ExtraGUIToolBar.h \
           src/FunctionDialog.h \
           src/HelpWindow.h
           
SOURCES += src/ExtraGUIToolBar.cpp \
           src/FunctionDialog.cpp \
           src/HelpWindow.cpp
           
FORMS += ui/ExtraGUIToolBar.ui \
         ui/FunctionDialog.ui \
         ui/HelpWindow.ui
         
RESOURCES = rc/extra_gui.qrc

