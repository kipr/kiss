CONFIG -= debug release_and_debug
CONFIG += release

mac:CONFIG += x86
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
unix:QMAKE_DEL_FILE=rm -rf
win32:CONFIG -= embed_manifest_dll

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

QMAKE_DISTCLEAN += build/obj build/moc build/ui build/rcc build