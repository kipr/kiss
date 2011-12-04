PROJECT(KISS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

SET(INCLUDE ${KISS_SOURCE_DIR}/include)
SET(RC ${KISS_SOURCE_DIR}/rc)
SET(SRC ${KISS_SOURCE_DIR}/src)
SET(UI ${KISS_SOURCE_DIR}/ui)
SET(TRADEMARKS ${KISS_SOURCE_DIR}/trademarks)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/src 
	${KISS_SOURCE_DIR} 
	${KISS_BINARY_DIR} 
	${INCLUDE} 
	${INCLUDE}/dialogs
)

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_PHONON TRUE)
INCLUDE(${QT_USE_FILE})

SET(KISS_SRCS_CXX
	${SRC}/Debugger.cpp
	${SRC}/KissArchive.cpp
	${SRC}/Lexer.cpp
	${SRC}/LexerManager.cpp
	${SRC}/MainWindow.cpp
	${SRC}/Repository.cpp
	${SRC}/SourceFile.cpp
	${SRC}/Target.cpp
	${SRC}/TargetManager.cpp
	${SRC}/WebTab.cpp
	${SRC}/WelcomeTab.cpp
	${SRC}/dialogs/ChoosePortDialog.cpp
	${SRC}/dialogs/EditorSettingsDialog.cpp
	${SRC}/dialogs/SourceDialog.cpp
	${SRC}/dialogs/TemplateDialog.cpp
	${SRC}/dialogs/RequestFileDialog.cpp
	${SRC}/VideoPlayerTab.cpp
	${SRC}/dialogs/ErrorDialog.cpp
	${SRC}/JavaScript.cpp
	${SRC}/main.cpp
)

SET(KISS_UI
	${UI}/ChoosePortDialog.ui
	${UI}/Debugger.ui
	${UI}/EditorSettingsDialog.ui
	${UI}/FindDialog.ui
	${UI}/Interaction.ui
	${UI}/MainWindow.ui
	${UI}/Repository.ui
	${UI}/RepositoryDialog.ui
	${UI}/SourceDialog.ui
	${UI}/SourceFile.ui
	${UI}/TargetSettingsDialog.ui
	${UI}/TemplateDialog.ui
	${UI}/WebTab.ui
	${UI}/SourceDialog.ui
	${UI}/RequestFileDialog.ui
	${UI}/VideoPlayerTab.ui
	${UI}/ErrorDialog.ui
)

SET(KISS_MOC_SRCS
	${INCLUDE}/Debugger.h
	${INCLUDE}/Lexer.h
	${INCLUDE}/MainWindow.h
	${INCLUDE}/Repository.h
	${INCLUDE}/SourceFile.h
	${INCLUDE}/Target.h
	${INCLUDE}/WebTab.h
	${INCLUDE}/WelcomeTab.h
	${INCLUDE}/dialogs/ChoosePortDialog.h
	${INCLUDE}/dialogs/EditorSettingsDialog.h
	${INCLUDE}/dialogs/SourceDialog.h
	${INCLUDE}/dialogs/TemplateDialog.h
	${INCLUDE}/dialogs/RequestFileDialog.h
	${INCLUDE}/VideoPlayerTab.h
	${INCLUDE}/dialogs/ErrorDialog.h
	${INCLUDE}/MacroString.h
	${INCLUDE}/JavaScript.h
)

SET(KISS_QRC_FILES
	${RC}/rc.qrc
	${RC}/logos/logos.qrc
)

QT4_WRAP_CPP(KISS_SRCS_CXX ${KISS_MOC_SRCS})
QT4_WRAP_UI(KISS_SRCS_CXX ${KISS_UI})
QT4_ADD_RESOURCES(KISS_SRCS_CXX ${KISS_QRC_FILES})

IF(APPLE)
	SET(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.7.sdk")
	SET(CMAKE_CXX_FLAGS "-arch i386")
	SET(LINK_FLAGS "-arch i386")
	LINK_DIRECTORIES(/Library/Frameworks/)
	ADD_EXECUTABLE(KISS MACOSX_BUNDLE ${KISS_SRCS_CXX})
ELSEIF(UNIX)
	ADD_EXECUTABLE(KISS ${KISS_SRCS_CXX})
ELSEIF(WIN32)
  ADD_EXECUTABLE(KISS WIN32 ${KISS_SRCS_CXX})
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${KISS_SOURCE_DIR}/deploy)
TARGET_LINK_LIBRARIES(KISS ${QT_LIBRARIES} qscintilla2)

IF(APPLE)
ADD_CUSTOM_TARGET(MacDeployQt ALL 
	COMMAND ${QT_BINARY_DIR}/macdeployqt ${KISS_SOURCE_DIR}/deploy/KISS.app -verbose=0
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ENDIF()