PROJECT(KISS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

FIND_PACKAGE(Qt4 REQUIRED)

SET(INCLUDE ${KISS_SOURCE_DIR}/include)
SET(RC ${KISS_SOURCE_DIR}/rc)
SET(SRC ${KISS_SOURCE_DIR}/src)
SET(UI ${KISS_SOURCE_DIR}/ui)
SET(TRADEMARKS ${KISS_SOURCE_DIR}/trademarks)
SET(COMPAT ${KISS_SOURCE_DIR}/compat)

SET(KISS_INCLUDE_DIRS ${INCLUDE} 
	${INCLUDE}/dialogs
	${INCLUDE}/lexer
	${INCLUDE}/widgets
	${INCLUDE}/menus
	${INCLUDE}/js
	${INCLUDE}/project
	${INCLUDE}/compiler
	${INCLUDE}/interface)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/src 
	${KISS_SOURCE_DIR} 
	${KISS_BINARY_DIR} 
	${KISS_INCLUDE_DIRS}
)

IF(WIN32)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../prefix/include)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../prefix/lib)
ENDIF(WIN32)

SET(QT_USE_QTDECLARATIVE TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSCRIPT TRUE)

INCLUDE(${QT_USE_FILE})

FILE(GLOB INCLUDES ${INCLUDE}/*.hpp
	${INCLUDE}/lexer/*
	${INCLUDE}/dialogs/*
	${INCLUDE}/widgets/*
	${INCLUDE}/menus/* 
	${INCLUDE}/js/*
	${INCLUDE}/project/*
	${INCLUDE}/compiler/*
	${INCLUDE}/interface/*)

FILE(GLOB SOURCES ${SRC}/*
	${SRC}/lexer/*
	${SRC}/dialogs/*
	${SRC}/widgets/*
	${SRC}/menus/*
	${SRC}/js/*
	${SRC}/project/*
	${SRC}/compiler/*
	${SRC}/interface/*)

FILE(GLOB UIS ${UI}/*)

SET(KISS_SRCS_CXX ${SOURCES})
SET(KISS_UI ${UIS})
SET(KISS_MOC_SRCS ${INCLUDES})

SET(KISS_QRC_FILES
	${RC}/rc.qrc
	${RC}/logos/logos.qrc
)

QT4_WRAP_CPP(KISS_SRCS_CXX ${KISS_MOC_SRCS})
QT4_WRAP_UI(KISS_SRCS_CXX ${KISS_UI})
QT4_ADD_RESOURCES(KISS_SRCS_CXX ${KISS_QRC_FILES})

SET(CMAKE_CXX_FLAGS "-Wall")

ADD_DEFINITIONS(-O2)

IF(APPLE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
	LINK_DIRECTORIES(/Library/Frameworks/)
	ADD_EXECUTABLE(KISS MACOSX_BUNDLE ${KISS_SRCS_CXX})
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	ADD_EXECUTABLE(KISS ${KISS_SRCS_CXX})
ELSEIF(WIN32)
	SET(COMPAT_SRCS ${COMPAT}/mingw.c)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
	ADD_EXECUTABLE(KISS WIN32 ${KISS_SRCS_CXX} ${COMPAT_SRCS} ${CMAKE_SOURCE_DIR}/rc/logos/kiss.rc)
ENDIF()

ADD_LIBRARY(kisside STATIC ${KISS_SRCS_CXX})

SET(EXECUTABLE_OUTPUT_PATH ${KISS_SOURCE_DIR}/deploy)
IF(WIN32)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../prefix/lib)
ELSE(WIN32)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(KISS ${QT_LIBRARIES} qscintilla2 pcompiler kar)

IF(WIN32)
	TARGET_LINK_LIBRARIES(KISS ws2_32)
ENDIF(WIN32)

IF(APPLE)
	ADD_CUSTOM_TARGET(MacDeployQt ALL
		COMMAND ${QT_BINARY_DIR}/macdeployqt ${KISS_SOURCE_DIR}/deploy/KISS.app -verbose=0
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)

	ADD_CUSTOM_TARGET(Info.plist ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/Info.plist ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)
	
	ADD_CUSTOM_TARGET(qscintilla ALL
		COMMAND ${CMAKE_COMMAND} -E copy /Library/Frameworks/libqscintilla2.9.dylib
			${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Frameworks
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS MacDeployQt)
	
	ADD_CUSTOM_TARGET(Icons ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/logos/mac_icon.icns ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Resources
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS Info.plist MacDeployQt)
		
	SET(OSX_INSTALL_NAMES_SCRIPT "${KISS_SOURCE_DIR}/scripts/osx_install_names.sh")
	
	install(DIRECTORY ${RC}/template_packs DESTINATION ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents)
ELSE(APPLE)
	install(DIRECTORY ${RC}/template_packs DESTINATION ${KISS_SOURCE_DIR}/deploy)
ENDIF(APPLE)

IF(APPLE)
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
ADD_CUSTOM_TARGET(kiss_osx_install_names ALL 
	COMMAND ${OSX_INSTALL_NAMES_SCRIPT} ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/MacOS/KISS
		QtCore.framework/Versions/4/QtCore
		QtScript.framework/Versions/4/QtScript
		QtDBus.framework/Versions/4/QtDBus
		QtGui.framework/Versions/4/QtGui
		QtNetwork.framework/Versions/4/QtNetwork
		QtXml.framework/Versions/4/QtXml
		QtXmlPatterns.framework/Versions/4/QtXmlPatterns
		QtSvg.framework/Versions/4/QtSvg
		QtSql.framework/Versions/4/QtSql
		QtWebKit.framework/Versions/4/QtWebKit
		phonon.framework/Versions/4/phonon
		QtDeclarative.framework/Versions/4/QtDeclarative
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	DEPENDS KISS Info.plist MacDeployQt)
ADD_CUSTOM_TARGET(qscintilla_install_names ALL 
	COMMAND ${OSX_INSTALL_NAMES_SCRIPT} ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/Frameworks/libqscintilla2.9.dylib
		QtCore.framework/Versions/4/QtCore
		QtScript.framework/Versions/4/QtScript
		QtDBus.framework/Versions/4/QtDBus
		QtGui.framework/Versions/4/QtGui
		QtNetwork.framework/Versions/4/QtNetwork
		QtXml.framework/Versions/4/QtXml
		QtXmlPatterns.framework/Versions/4/QtXmlPatterns
		QtSvg.framework/Versions/4/QtSvg
		QtSql.framework/Versions/4/QtSql
		QtWebKit.framework/Versions/4/QtWebKit
		phonon.framework/Versions/4/phonon
		QtDeclarative.framework/Versions/4/QtDeclarative
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	DEPENDS KISS Info.plist MacDeployQt)
ENDIF(APPLE)

# Accessory Programs

LINK_DIRECTORIES(${KISS_SOURCE_DIR}/lib)

install(FILES ${INCLUDES} DESTINATION /usr/local/include/kiss/)

add_subdirectory(plugins)
add_subdirectory(tests)