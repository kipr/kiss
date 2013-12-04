set(PLUGIN_NAME kovan_serial_interface)

project(${PLUGIN_NAME})
cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Widgets REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${INCLUDE_DIRS} ../shared)

file(GLOB LOCAL_INCLUDES *.hpp ../shared/*.hpp)
file(GLOB LOCAL_SOURCES *.cpp ../shared/*.cpp)

qt5_wrap_cpp(INCLUDES_MOC ${LOCAL_INCLUDES})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_DLL)

add_library(${PLUGIN_NAME} SHARED ${LOCAL_SOURCES} ${INCLUDES_MOC})
qt5_use_modules(${PLUGIN_NAME} Widgets)
link_directories(${LIBRARY_OUTPUT_PATH})

target_link_libraries(${PLUGIN_NAME} kisside pcompiler kovanserial kar)

if(APPLE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(${PLUGIN_NAME}_osx_install_names ALL 
	COMMAND ${OSX_INSTALL_NAMES_SCRIPT} lib${PLUGIN_NAME}.dylib
		QtCore.framework/Versions/4/QtCore
		QtScript.framework/Versions/4/QtScript
		QtDBus.framework/Versions/4/QtDBus
		QtGui.framework/Versions/5/QtGui
		QtNetwork.framework/Versions/4/QtNetwork
		QtXml.framework/Versions/4/QtXml
		QtXmlPatterns.framework/Versions/4/QtXmlPatterns
		QtSvg.framework/Versions/4/QtSvg
		QtSql.framework/Versions/4/QtSql
		QtWebKit.framework/Versions/4/QtWebKit
		phonon.framework/Versions/4/phonon
		QtDeclarative.framework/Versions/4/QtDeclarative
		libkar.dylib
		libpcompiler.dylib
		libkovanserial.dylib
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS ${PLUGIN_NAME})
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/interfaces)
else()
	install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${EXECUTABLE_OUTPUT_PATH}/interfaces
		RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/interfaces)
endif()