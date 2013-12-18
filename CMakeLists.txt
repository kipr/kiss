project(KISS)

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Declarative REQUIRED)


set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(RC ${CMAKE_SOURCE_DIR}/rc)
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(UI ${CMAKE_SOURCE_DIR}/ui)
set(TRADEMARKS ${CMAKE_SOURCE_DIR}/trademarks)
set(COMPAT ${CMAKE_SOURCE_DIR}/compat)

set(INCLUDE_DIRS ${INCLUDE} 
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
	${CMAKE_SOURCE_DIR} 
	${CMAKE_BINARY_DIR} 
	${INCLUDE_DIRS}
)

if(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/../prefix/include)
link_directories(${CMAKE_SOURCE_DIR}/../prefix/lib)
endif()

file(GLOB INCLUDES ${INCLUDE}/*.hpp
	${INCLUDE}/dialogs/*
	${INCLUDE}/widgets/*
	${INCLUDE}/menus/* 
	${INCLUDE}/js/*
	${INCLUDE}/project/*
	${INCLUDE}/compiler/*
	${INCLUDE}/interface/*)

file(GLOB SOURCES ${SRC}/*
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

set(SRCS_CXX ${SOURCES})
set(UI ${UIS})

set(QRC_FILES
	${RC}/rc.qrc
	${RC}/logos/logos.qrc
)

set(CMAKE_AUTOMOC ON)

qt5_wrap_ui(SRCS_CXX ${UI})
qt5_add_resources(SRCS_CXX ${QRC_FILES})

add_definitions(-Wall -O2)

if(APPLE)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	link_directories(/Library/Frameworks/ /usr/local/lib)
	include_directories(/usr/local/include /usr/include)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include")
	add_executable(KISS MACOSX_BUNDLE ${SRCS_CXX} ${INCLUDES})
  set(EXTRA_LIBS ${COCOA_LIBRARY})
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	add_executable(KISS ${SRCS_CXX} ${INCLUDES})
elseif(WIN32)
	set(COMPAT_SRCS ${COMPAT}/mingw.c)
	add_executable(KISS WIN32 ${SRCS_CXX} ${COMPAT_SRCS} ${INCLUDES})
endif()

set(EXECUTABLE_OUTPUT_PATH ${KISS_SOURCE_DIR}/deploy)

if(APPLE)
    set(KISS_EXTRAS_DIR ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents)
else()
    set(KISS_EXTRAS_DIR ${EXECUTABLE_OUTPUT_PATH})
endif()

qt5_use_modules(KISS Widgets Network PrintSupport Declarative)
target_link_libraries(KISS scintex pcompiler kar kovanserial ${EXTRA_LIBS})

if(WIN32)
	target_link_libraries(KISS ws2_32)
endif()

if(APPLE)
  # add_custom_target(MacDeployQt ALL
  #   COMMAND ${_qt5Core_install_prefix}/bin/macdeployqt ${KISS_SOURCE_DIR}/deploy/KISS.app -verbose=0
  #   WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  #   DEPENDS KISS)

	add_custom_target(Info.plist ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/Info.plist ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)
	
	add_custom_target(copy_scintex ALL
		COMMAND ${CMAKE_COMMAND} -E copy /usr/local/lib/libscintex.dylib
			${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Frameworks
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS)
	
	add_custom_target(Icons ALL
		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/logos/mac_icon.icns ${KISS_SOURCE_DIR}/deploy/KISS.app/Contents/Resources
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS KISS Info.plist)
		
	set(OSX_INSTALL_NAMES_SCRIPT "${KISS_SOURCE_DIR}/scripts/osx_install_names.sh")
endif()

install(DIRECTORY ${RC}/template_packs DESTINATION ${KISS_EXTRAS_DIR})

if(APPLE)
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
# add_custom_target(kiss_osx_install_names ALL 
#   COMMAND ${OSX_INSTALL_NAMES_SCRIPT} ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/MacOS/KISS
#     QtCore.framework/Versions/5/QtCore
#     QtScript.framework/Versions/5/QtScript
#     QtGui.framework/Versions/5/QtGui
#     QtWidgets.framework/Versions/5/QtWidgets
#     QtNetwork.framework/Versions/5/QtNetwork
#     QtXml.framework/Versions/5/QtXml
#     QtXmlPatterns.framework/Versions/5/QtXmlPatterns
#     QtSvg.framework/Versions/5/QtSvg
#     QtSql.framework/Versions/5/QtSql
#     QtWebKit.framework/Versions/5/QtWebKit
#     QtDeclarative.framework/Versions/5/QtDeclarative
#   WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
#   DEPENDS KISS Info.plist MacDeployQt)
# add_custom_target(qscintilla_install_names ALL 
#   COMMAND ${OSX_INSTALL_NAMES_SCRIPT} ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/Frameworks/libscintex.dylib
#     QtCore.framework/Versions/5/QtCore
#     QtScript.framework/Versions/5/QtScript
#     QtGui.framework/Versions/5/QtGui
#     QtWidgets.framework/Versions/5/QtWidgets
#     QtNetwork.framework/Versions/5/QtNetwork
#     QtXml.framework/Versions/5/QtXml
#     QtXmlPatterns.framework/Versions/5/QtXmlPatterns
#     QtSvg.framework/Versions/5/QtSvg
#     QtSql.framework/Versions/5/QtSql
#     QtWebKit.framework/Versions/5/QtWebKit
#     QtDeclarative.framework/Versions/5/QtDeclarative
#   WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
# DEPENDS KISS Info.plist MacDeployQt)
endif()

# Accessory Programs

link_directories(${KISS_SOURCE_DIR}/lib)

install(FILES ${INCLUDES} DESTINATION /usr/local/include/kiss/)

add_subdirectory(docs)
