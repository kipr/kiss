project(KISS)

cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

set(INCLUDE ${KISS_SOURCE_DIR}/include)
set(RC ${KISS_SOURCE_DIR}/rc)
set(SRC ${KISS_SOURCE_DIR}/src)
set(UI ${KISS_SOURCE_DIR}/ui)
set(TRADEMARKS ${KISS_SOURCE_DIR}/trademarks)
set(COMPAT ${KISS_SOURCE_DIR}/compat)

set(KISS_INCLUDE_DIRS ${INCLUDE} 
	${INCLUDE}/dialogs
	${INCLUDE}/lexer
	${INCLUDE}/widgets
	${INCLUDE}/menus
	${INCLUDE}/js
	${INCLUDE}/project
	${INCLUDE}/compiler
	${INCLUDE}/interface)

include_directories(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/src 
	${KISS_SOURCE_DIR} 
	${KISS_BINARY_DIR} 
	${KISS_INCLUDE_DIRS}
)

if(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/../prefix/include)
link_directories(
	${CMAKE_SOURCE_DIR}/../prefix/lib
	${QT_LIBRARY_DIR})
endif()

set(QT_USE_QTDECLARATIVE TRUE)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTSCRIPT TRUE)

include(${QT_USE_FILE})

file(GLOB INCLUDES ${INCLUDE}/*.hpp
	${INCLUDE}/lexer/*
	${INCLUDE}/dialogs/*
	${INCLUDE}/widgets/*
	${INCLUDE}/menus/* 
	${INCLUDE}/js/*
	${INCLUDE}/project/*
	${INCLUDE}/compiler/*
	${INCLUDE}/interface/*)

file(GLOB SOURCES ${SRC}/*
	${SRC}/lexer/*
	${SRC}/dialogs/*
	${SRC}/widgets/*
	${SRC}/menus/*
	${SRC}/js/*
	${SRC}/project/*
	${SRC}/compiler/*
	${SRC}/interface/*)
  
if(APPLE)
  file(GLOB PLATFORM_SOURCES ${SRC}/platform_osx/*)
elseif(UNIX)
  file(GLOB PLATFORM_SOURCES ${SRC}/platform_nix/*)
elseif(WIN32)
  file(GLOB PLATFORM_SOURCES ${SRC}/platform_win32/*)
endif()

set(SOURCES ${SOURCES} ${SRC} ${PLATFORM_SOURCES})

file(GLOB UIS ${UI}/*)

set(KISS_SRCS_CXX ${SOURCES})
set(KISS_UI ${UIS})
set(KISS_MOC_SRCS ${INCLUDES})

set(KISS_QRC_FILES
	${RC}/rc.qrc
	${RC}/logos/logos.qrc
)

qt4_wrap_cpp(KISS_SRCS_CXX ${KISS_MOC_SRCS})
qt4_wrap_ui(KISS_SRCS_CXX ${KISS_UI})
qt4_add_resources(KISS_SRCS_CXX ${KISS_QRC_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_definitions(-O2)

if(APPLE)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	link_directories(/Library/Frameworks/ /usr/local/lib)
	include_directories(/usr/local/include /usr/include)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include")
	add_executable(KISS MACOSX_BUNDLE ${KISS_SRCS_CXX} ${INCLUDES})
  set(EXTRA_LIBS ${COCOA_LIBRARY})
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	add_executable(KISS ${KISS_SRCS_CXX} ${INCLUDES})
elseif(WIN32)
	set(COMPAT_SRCS ${COMPAT}/mingw.c)
	add_executable(KISS WIN32 ${KISS_SRCS_CXX} ${COMPAT_SRCS} ${INCLUDES})
endif()

add_library(kisside STATIC ${KISS_SRCS_CXX} ${INCLUDES})

set(EXECUTABLE_OUTPUT_PATH ${KISS_SOURCE_DIR}/deploy)
if(WIN32)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../prefix/lib)
else()
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
endif()

if(APPLE)
    set(KISS_EXTRAS_DIR ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents)
else()
    set(KISS_EXTRAS_DIR ${EXECUTABLE_OUTPUT_PATH})
endif()

target_link_libraries(KISS ${QT_LIBRARIES} qscintilla2 pcompiler kar ${EXTRA_LIBS})

if(WIN32)
	target_link_libraries(KISS ws2_32)
endif()

if(APPLE)
	add_custom_target(MacDeployQt ALL
		COMMAND ${QT_BINARY_DIR}/macdeployqt ${KISS_SOURCE_DIR}/deploy/KISS.app -verbose=0
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)

	add_custom_target(Info.plist ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/Info.plist ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)
	
	add_custom_target(qscintilla ALL
		COMMAND ${CMAKE_COMMAND} -E copy /Library/Frameworks/libqscintilla2.9.dylib
			${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Frameworks
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS MacDeployQt)
	
	add_custom_target(Icons ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/logos/mac_icon.icns ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Resources
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS Info.plist MacDeployQt)
		
	set(OSX_INSTALL_NAMES_SCRIPT "${KISS_SOURCE_DIR}/scripts/osx_install_names.sh")
endif()

install(DIRECTORY ${RC}/template_packs DESTINATION ${KISS_EXTRAS_DIR})

if(APPLE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(kiss_osx_install_names ALL 
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
add_custom_target(qscintilla_install_names ALL 
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
endif()

# Accessory Programs

link_directories(${KISS_SOURCE_DIR}/lib)

install(FILES ${INCLUDES} DESTINATION /usr/local/include/kiss/)

add_subdirectory(plugins)
add_subdirectory(docs)
