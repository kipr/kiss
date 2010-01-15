TEMPLATE = subdirs

exists(../trademarks) {
	SUBDIRS = kiss \
		  extraGUI \
		  ../trademarks/simulator
}
!exists(../trademarks) {
SUBDIRS = kiss \
          kiss_sim \
	  extraGUI
}
