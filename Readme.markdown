KIPR's Instructional Software System
====================================

Building this software is very simple. Make sure you have the following pieces of software installed:

* CMake 2.8.0 or higher
* GCC
* Qt 4.7 or higher
* QScintilla 2
* libkar
* pcompiler
* easydevice

Building
========

	mkdir -p build
	cd build
	cmake ..
	make
	make install

You should now have KISS ready to go in the deploy/ directory.

Authors
=======

* Braden McDorman
* Nafis Zaman
