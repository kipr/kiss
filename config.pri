
CONFIG -= debug release_and_debug
CONFIG += release

mac:CONFIG += x86 ppc
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
#QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
#mac:CONFIG -= gnu-libiconv
unix:QMAKE_DEL_FILE=rm -rf

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc


QMAKE_DISTCLEAN += build/obj build/moc build/ui build/rcc build

INSTALL_BASE = deploy
