PROJECT(computer)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

SET(RC ${computer_SOURCE_DIR}/rc)
SET(SRC ${computer_SOURCE_DIR}/src)
SET(UI ${computer_SOURCE_DIR}/ui)

INCLUDE_DIRECTORIES(${computer_BINARY_DIR} ${computer_SOURCE_DIR}/include ${DEVICES_SHARED}/include)

SET(QT_USE_QTNETWORK TRUE)

INCLUDE(${QT_USE_FILE})

FILE(GLOB INCLUDES ${computer_SOURCE_DIR}/include/*.h ${DEVICES_SHARED}/include/*)
FILE(GLOB SOURCES ${SRC}/* ${DEVICES_SHARED}/src/*)
FILE(GLOB UIS ${UI}/*)

SET(computer_SRCS_CXX ${SOURCES})
SET(computer_UI ${UIS})
SET(computer_MOC_SRCS ${INCLUDES})

# SET(computer_QRC_FILES ${RC}/rc.qrc ${RC}/logos/logos.qrc)

QT4_WRAP_CPP(computer_SRCS_CXX ${computer_MOC_SRCS})
QT4_WRAP_UI(computer_SRCS_CXX ${computer_UI})
# QT4_ADD_RESOURCES(computer_SRCS_CXX ${computer_QRC_FILES})

SET(CMAKE_CXX_FLAGS "-Wall")

IF(APPLE)
	SET(CMAKE_OSX_SYSROOT "${OSX_DEVELOPER_ROOT}/SDKs/MacOSX10.6.sdk")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
	LINK_DIRECTORIES(/Library/Frameworks/)
	ADD_EXECUTABLE(computer MACOSX_BUNDLE ${computer_SRCS_CXX})
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	ADD_EXECUTABLE(computer ${computer_SRCS_CXX})
ELSEIF(WIN32)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
	ADD_EXECUTABLE(computer WIN32 ${computer_SRCS_CXX} rc/logos/kiss.rc)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${computer_SOURCE_DIR}/deploy)
SET(LIBRARY_OUTPUT_PATH ${computer_SOURCE_DIR}/lib)
TARGET_LINK_LIBRARIES(computer ${QT_LIBRARIES} qscintilla2 kiss-compiler tinyarchive z)

IF(APPLE)
	ADD_CUSTOM_TARGET(computer_MacDeployQt ALL 
		COMMAND ${QT_BINARY_DIR}/macdeployqt ${computer_SOURCE_DIR}/deploy/computer.app -verbose=0
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS computer)
	
#	ADD_CUSTOM_TARGET(Info.plist ALL 
#		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/Info.plist ${computer_SOURCE_DIR}/deploy/computer.app/Contents
#		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#		DEPENDS computer)
#	
#	ADD_CUSTOM_TARGET(Icons ALL 
#		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/logos/mac_icon.icns ${computer_SOURCE_DIR}/deploy/computer.app/Contents/Resources
#		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#		DEPENDS computer Info.plist MacDeployQt)
ENDIF()