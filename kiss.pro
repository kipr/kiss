!tt {
	include(config.pri)
	include(trademarks/branding.pri)

	TEMPLATE = app
	INSTALL_BASE = deploy

	TARGET = KISS
	DEPENDPATH += src
	INCLUDEPATH += include include/dialogs
	QT += webkit network phonon

	win32 {
		INCLUDEPATH += Qsci
		CONFIG -= embed_manifest_exe
		CONFIG += console
	}

	LIBS += -lqscintilla2
	win32: LIBS += -lshell32

	# Input
	HEADERS += 	include/MainWindow.h \
				include/dialogs/ChoosePortDialog.h \
				include/dialogs/EditorSettingsDialog.h \
				include/Target.h \
				include/SourceFile.h \
				include/Lexer.h \
				include/Singleton.h \
				include/PluginManager.h \
				include/Tab.h \
				include/TargetManager.h \
				include/WebTab.h \
				include/WelcomeTab.h \
				include/LexerManager.h \
				include/DebuggerInterface.h \
				include/Debugger.h \
				include/Repository.h \
				include/KissArchive.h \
				include/dialogs/TemplateDialog.h \
				include/dialogs/SourceDialog.h \
				include/dialogs/RequestFileDialog.h \
				include/VideoPlayerTab.h

	SOURCES += 	src/main.cpp \
				src/MainWindow.cpp \
				src/dialogs/ChoosePortDialog.cpp \
				src/dialogs/EditorSettingsDialog.cpp \
				src/Target.cpp \
				src/SourceFile.cpp \
				src/Lexer.cpp \
				src/TargetManager.cpp \
				src/WebTab.cpp \
				src/WelcomeTab.cpp \
				src/LexerManager.cpp \
				src/Debugger.cpp \
				src/Repository.cpp \
				src/KissArchive.cpp \
				src/dialogs/TemplateDialog.cpp \
				src/dialogs/SourceDialog.cpp \
				src/dialogs/RequestFileDialog.cpp \
				src/VideoPlayerTab.cpp

	FORMS += 	ui/MainWindow.ui \
				ui/ChoosePortDialog.ui \
				ui/FindDialog.ui \
				ui/EditorSettingsDialog.ui \
				ui/TargetSettingsDialog.ui \
				ui/SourceFile.ui \
				ui/WebTab.ui \
				ui/Debugger.ui \
				ui/Repository.ui \
				ui/TemplateDialog.ui \
				ui/SourceDialog.ui \
				ui/RequestFileDialog.ui \
				ui/VideoPlayerTab.ui

	RESOURCES += rc/rc.qrc

	QMAKE_INFO_PLIST = Info.plist

	#### Install Directives ####

	changelog.files = dog/ChangeLog.txt
	changelog.path = $${INSTALL_BASE}

	target.path += $${INSTALL_BASE}
	INSTALLS += target changelog

	macx: {
		install_libs.extra = macdeployqt $${INSTALL_BASE}/KISS.app
		install_libs.path = $${INSTALL_BASE}
	
		install_plist.extra = cp Info.plist $${INSTALL_BASE}/KISS.app/Contents
		install_plist.path = $${INSTALL_BASE}/..

		INSTALLS += install_libs install_plist
	}
}

tt {
	include(config.pri)

	INSTALL_BASE = deploy
	TARGET = TargetTool
	DEPENDPATH += src
	INCLUDEPATH += include
	CONFIG += console

	win32 {
		CONFIG -= embed_manifest_exe
	}
	win32: LIBS += -lshell32

	# Input
	HEADERS += 	include/Target.h \
				include/Singleton.h \
				include/PluginManager.h \
				include/Tab.h \
				include/TargetManager.h \
				include/KissArchive.h

	SOURCES += 	src/TargetTool.cpp \
				src/Target.cpp \
				src/TargetManager.cpp \
				src/KissArchive.cpp
}
