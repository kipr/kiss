PROJECT(kovansim)

FIND_PACKAGE(Qt4 REQUIRED)

SET(INCLUDE ${kovansim_SOURCE_DIR}/include)
SET(RC ${kovansim_SOURCE_DIR}/rc)
SET(SRC ${kovansim_SOURCE_DIR}/src)
SET(UI ${kovansim_SOURCE_DIR}/ui)

SET(kovansim_INCLUDE_DIRS ${INCLUDE})

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/src 
	${kovansim_SOURCE_DIR} 
	${kovansim_BINARY_DIR}
	${kovansim_INCLUDE_DIRS}
	${DEVICES_SHARED}/include
)

SET(QT_USE_QTDECLARATIVE TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_PHONON TRUE)

INCLUDE(${QT_USE_FILE})

FILE(GLOB INCLUDES ${INCLUDE}/*.h ${DEVICES_SHARED}/include/*)
FILE(GLOB SOURCES ${SRC}/* ${DEVICES_SHARED}/src/*)
FILE(GLOB UIS ${UI}/*)

SET(kovansim_SRCS_CXX ${SOURCES})
SET(kovansim_UI ${UIS})
SET(kovansim_MOC_SRCS ${INCLUDES})

# SET(kovansim_QRC_FILES ${RC}/rc.qrc ${RC}/logos/logos.qrc)

QT4_WRAP_CPP(kovansim_SRCS_CXX ${kovansim_MOC_SRCS})
QT4_WRAP_UI(kovansim_SRCS_CXX ${kovansim_UI})
# QT4_ADD_RESOURCES(kovansim_SRCS_CXX ${kovansim_QRC_FILES})

SET(CMAKE_CXX_FLAGS "-Wall")

IF(APPLE)
	SET(CMAKE_OSX_SYSROOT "${OSX_DEVELOPER_ROOT}/SDKs/MacOSX10.6.sdk")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
	LINK_DIRECTORIES(/Library/Frameworks/)
	ADD_EXECUTABLE(kovansim MACOSX_BUNDLE ${kovansim_SRCS_CXX})
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	ADD_EXECUTABLE(kovansim ${kovansim_SRCS_CXX})
ELSEIF(WIN32)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
	ADD_EXECUTABLE(kovansim WIN32 ${kovansim_SRCS_CXX} rc/logos/kiss.rc)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${kovansim_SOURCE_DIR}/deploy)
SET(LIBRARY_OUTPUT_PATH ${kovansim_SOURCE_DIR}/lib)
TARGET_LINK_LIBRARIES(kovansim ${QT_LIBRARIES} qscintilla2 kiss-compiler tinyarchive z)

IF(APPLE)
	ADD_CUSTOM_TARGET(kovansim_MacDeployQt ALL 
		COMMAND ${QT_BINARY_DIR}/macdeployqt ${kovansim_SOURCE_DIR}/deploy/kovansim.app -verbose=0
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		DEPENDS kovansim)
	
#	ADD_CUSTOM_TARGET(Info.plist ALL 
#		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/Info.plist ${kovansim_SOURCE_DIR}/deploy/kovansim.app/Contents
#		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#		DEPENDS kovansim)
#	
#	ADD_CUSTOM_TARGET(Icons ALL 
#		COMMAND ${CMAKE_COMMAND} -E copy ${RC}/logos/mac_icon.icns ${kovansim_SOURCE_DIR}/deploy/kovansim.app/Contents/Resources
#		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#		DEPENDS kovansim Info.plist MacDeployQt)
ENDIF()

# add_subdirectory(plugins)