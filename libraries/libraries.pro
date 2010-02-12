TEMPLATE = subdirs

exists(../trademarks) {
	SUBDIRS = kiss \
		  extraGUI \
		  ../trademarks/simulator
}
!exists(../trademarks) {
SUBDIRS = kiss \
          cbc_sim \
	  cbc2_sim \
	  extraGUI
}
