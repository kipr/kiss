set(PLUGIN_NAME c_lexer)

project(${PLUGIN_NAME})
cmake_minimum_required(VERSION 2.8.0)

find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR} ${KISS_INCLUDE_DIRS})

include(${QT_USE_FILE})

file(GLOB LOCAL_INCLUDES *.hpp)
file(GLOB LOCAL_SOURCES *.cpp)

qt4_wrap_cpp(INCLUDES_MOC ${LOCAL_INCLUDES})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_DLL)

add_library(${PLUGIN_NAME} SHARED ${LOCAL_SOURCES} ${INCLUDES_MOC})
link_directories(${LIBRARY_OUTPUT_PATH})

target_link_libraries(${PLUGIN_NAME} kisside qscintilla2 ${QT_LIBRARIES})

if(APPLE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -headerpad_max_install_names")
add_custom_target(${PLUGIN_NAME}_osx_install_names ALL 
	COMMAND ${OSX_INSTALL_NAMES_SCRIPT} lib${PLUGIN_NAME}.dylib
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
		libkar.dylib
		libqscintilla2.9.dylib
		libpcompiler.dylib
	WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
	DEPENDS ${PLUGIN_NAME})
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${EXECUTABLE_OUTPUT_PATH}/KISS.app/Contents/lexers)
else()
	install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${EXECUTABLE_OUTPUT_PATH}/lexers
		RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/lexers)
endif()