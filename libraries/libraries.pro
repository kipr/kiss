TEMPLATE = subdirs

exists(../trademarks) {
	SUBDIRS = 	kiss \
		  		../trademarks/simulator
}
!exists(../trademarks) {
SUBDIRS = 	kiss \
	  		cbc2_sim
}
