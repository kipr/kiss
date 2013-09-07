KIPR's Instructional Software System
====================================

Building this software is very simple. Make sure you have the following pieces of software installed:

* Qt 4.7 or higher
* CMake 2.8.0 or higher
* QScintilla 2
* libkovanserial
* pcompiler
* libkar

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
