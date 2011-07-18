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
INCLUDE(${QT_USE_FILE})

SET(KISS_SRCS_CXX
	${SRC}/Debugger.cpp
	${SRC}/KissArchive.cpp
	${SRC}/Lexer.cpp
	${SRC}/LexerManager.cpp
	${SRC}/MainWindow.cpp
	${SRC}/Repository.cpp
	${SRC}/SourceFile.cpp
	${SRC}/SourceFileShared.cpp
	${SRC}/Target.cpp
	${SRC}/TargetManager.cpp
	${SRC}/WebTab.cpp
	${SRC}/WelcomeTab.cpp
	${SRC}/dialogs/ChoosePortDialog.cpp
	${SRC}/dialogs/EditorSettingsDialog.cpp
	${SRC}/dialogs/FindDialog.cpp
	${SRC}/dialogs/SourceDialog.cpp
	${SRC}/dialogs/TargetSettingsDialog.cpp
	${SRC}/dialogs/TemplateDialog.cpp
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
	${INCLUDE}/dialogs/FindDialog.h
	${INCLUDE}/dialogs/SourceDialog.h
	${INCLUDE}/dialogs/TargetSettingsDialog.h
	${INCLUDE}/dialogs/TemplateDialog.h
)

SET(KISS_QRC_FILES
	${RC}/rc.qrc
	${TRADEMARKS}/logos/logos.qrc
)

QT4_WRAP_CPP(KISS_SRCS_CXX ${KISS_MOC_SRCS})
QT4_WRAP_UI(KISS_SRCS_CXX ${KISS_UI})
QT4_ADD_RESOURCES(KISS_SRCS_CXX ${KISS_QRC_FILES})

IF(APPLE)
	SET(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.6.sdk")
	SET(CMAKE_CXX_FLAGS "-arch i386")
	SET(LINK_FLAGS "-arch i386")
	LINK_DIRECTORIES(/Library/Frameworks/)
	ADD_EXECUTABLE(kiss MACOSX_BUNDLE ${KISS_SRCS_CXX})
ELSEIF(UNIX)
	ADD_EXECUTABLE(kiss ${KISS_SRCS_CXX})
ELSEIF(WIN32)
  ADD_EXECUTABLE(kiss WIN32 ${KISS_SRCS_CXX})
ENDIF()

TARGET_LINK_LIBRARIES(kiss ${QT_LIBRARIES} qscintilla2)

IF(APPLE)
#ADD_CUSTOM_TARGET(addframeworks ALL 
#	COMMAND ${QT_BINARY_DIR}/macdeployqt kiss.app
#	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ENDIF()
