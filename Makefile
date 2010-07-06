.DEFAULT_GOAL = development
.PHONY = develoipment clean

libqxt-dev :
	mkdir -p libqxt/src/core
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.h -O libqxt/src/core/qxtglobal.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/core/qxtglobal.cpp -O libqxt/src/core/qxtglobal.cpp
	mkdir -p libqxt/src/gui
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.cpp -O libqxt/src/gui/qxtglobalshortcut.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut.h -O libqxt/src/gui/qxtglobalshortcut.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_p.h -O libqxt/src/gui/qxtglobalshortcut_p.h
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_mac.cpp -O libqxt/src/gui/qxtglobalshortcut_mac.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_win.cpp -O libqxt/src/gui/qxtglobalshortcut_win.cpp
	wget http://dev.libqxt.org/libqxt/raw/tip/src/gui/qxtglobalshortcut_x11.cpp -O libqxt/src/gui/qxtglobalshortcut_x11.cpp
libqxt :
	

development : libqxt-dev
	git clone 	

stable : libqxt
	git clone ..stable
