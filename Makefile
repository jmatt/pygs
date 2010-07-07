.DEFAULT_GOAL = development
.PHONY = development stable build clean libqxt-development libqxt-stable

CHK_DIR_EXISTS  = test -d
MKDIR		= mkdir -p
RMDIR		= rmdir
COPY		= cp
COPY_FILE	= $(COPY)
COPY_DIR	= cp -r
DEL_FILE	= rm -f 
DEL_FILE_RECURSIVE = $(DEL_FILE) -r

libqxt-development :
	-$(MKDIR) libqxt/src/core
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.h -O libqxt/src/core/qxtglobal.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.cpp -O libqxt/src/core/qxtglobal.cpp
	-$(MKDIR) libqxt/src/gui
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.cpp -O libqxt/src/gui/qxtglobalshortcut.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.h -O libqxt/src/gui/qxtglobalshortcut.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_p.h -O libqxt/src/gui/qxtglobalshortcut_p.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_mac.cpp -O libqxt/src/gui/qxtglobalshortcut_mac.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_win.cpp -O libqxt/src/gui/qxtglobalshortcut_win.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_x11.cpp -O libqxt/src/gui/qxtglobalshortcut_x11.cpp

libqxt-stable :
	-$(MKDIR) libqxt/src/core
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.h -O libqxt/src/core/qxtglobal.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.cpp -O libqxt/src/core/qxtglobal.cpp
	-$(MKDIR) libqxt/src/gui
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.cpp -O libqxt/src/gui/qxtglobalshortcut.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.h -O libqxt/src/gui/qxtglobalshortcut.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_p.h -O libqxt/src/gui/qxtglobalshortcut_p.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_mac.cpp -O libqxt/src/gui/qxtglobalshortcut_mac.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_win.cpp -O libqxt/src/gui/qxtglobalshortcut_win.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_x11.cpp -O libqxt/src/gui/qxtglobalshortcut_x11.cpp

development :	libqxt-development build

stable :	libqxt-stable build

build :		
		cd py && python configure.py && make Makefile && make install
		echo
		echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		echo ~~                           THANKS GOES TO                             ~~
		echo ~~ PyQt4/SIP from Riverbank Computing Ltd http://riverbankcomputing.com ~~
		echo ~~ libqxt and Handelsweise http://libqxt.org http://www.handelsweise.de ~~
		echo ~~ Qt and Nokia http://qt.nokia.com                                     ~~
		echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		echo 
		echo                        Success! pygs is installed.
clean :		
		-$(DEL_FILE_RECURSIVE) "py/build"
		-$(DEL_FILE_RECURSIVE) "py/lib/release"
		-$(DEL_FILE_RECURSIVE) "py/lib/debug"
		-$(DEL_FILE) "py/lib/libpygs.a"
		-$(DEL_FILE) "py/lib/pygs.dll"
		-$(DEL_FILE) "py/lib/*.o"
		-$(DEL_FILE) "py/lib/*.cpp"
		-$(DEL_FILE) "py/lib/Makefile"
		-$(DEL_FILE) "py/lib/Makefile.Debug"
		-$(DEL_FILE) "py/lib/Makefile.Release"
		-$(DEL_FILE) "py/Makefile"