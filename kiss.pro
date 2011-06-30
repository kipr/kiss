include(config.pri)
include(trademarks/branding.pri)

TEMPLATE = app
INSTALL_BASE = deploy

TARGET = KISS
DEPENDPATH += src
INCLUDEPATH += include
QT += webkit network

win32 {
	INCLUDEPATH += Qsci
	CONFIG -= embed_manifest_exe
}

LIBS += -lqscintilla2
win32: LIBS += -lshell32

# Input
HEADERS += 	include/MainWindow.h \
			include/ChooseTargetDialog.h \
			include/ChoosePortDialog.h \
			include/EditorSettingsDialog.h \
			include/TargetSettingsDialog.h \
			include/FindDialog.h \
			include/Target.h \
			include/SourceFile.h \
			include/Lexer.h \
			include/LexerSpec.h \
			include/Version.h \
			include/Tab.h \
			include/TargetManager.h \
			include/WebTab.h \
			include/WelcomeTab.h \
			include/LexerSpecManager.h \
			include/DebuggerInterface.h \
			include/Debugger.h \
			include/SourceFileShared.h \
			include/Repository.h \
			include/KissArchive.h

SOURCES += 	src/main.cpp \
			src/MainWindow.cpp \
			src/ChooseTargetDialog.cpp \
			src/ChoosePortDialog.cpp \
			src/EditorSettingsDialog.cpp \
			src/TargetSettingsDialog.cpp \
			src/FindDialog.cpp \
			src/Target.cpp \
			src/SourceFile.cpp \
			src/Lexer.cpp \
			src/TargetManager.cpp \
			src/WebTab.cpp \
			src/WelcomeTab.cpp \
			src/LexerSpecManager.cpp \
			src/Debugger.cpp \
			src/SourceFileShared.cpp \
			src/Repository.cpp \
			src/KissArchive.cpp

FORMS += 	ui/MainWindow.ui \
			ui/ChooseTargetDialog.ui \
			ui/ChoosePortDialog.ui \
			ui/FindDialog.ui \
			ui/EditorSettingsDialog.ui \
			ui/TargetSettingsDialog.ui \
			ui/SourceFile.ui \
			ui/WebTab.ui \
			ui/Debugger.ui \
			ui/Repository.ui

RESOURCES += rc/rc.qrc

#### Install Directives ####

changelog.files = dog/ChangeLog.txt
changelog.path = $${INSTALL_BASE}

target.path += $${INSTALL_BASE}
INSTALLS += target changelog

macx: {
	install_libs.extra = macdeployqt $${INSTALL_BASE}/KISS.app
	install_libs.path = $${INSTALL_BASE}

	INSTALLS += install_libs
}
