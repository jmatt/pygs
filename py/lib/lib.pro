CONFIG += qt
QT = core gui
QXT = core
DEFINES = BUILD_QXT_GUI BUILD_QXT_CORE
TEMPLATE = lib
DEPENDPATH += . ../../libqxt/src/core ../../libqxt/src/gui
INCLUDEPATH += $$DEPENDPATH
HEADERS += qxtglobalshortcut.h

SOURCES += qxtglobal.cpp \
           qxtglobalshortcut.cpp \

unix:!macx {
CONFIG += static
SOURCES += qxtglobalshortcut_x11.cpp
}

macx {
CONFIG += static
SOURCES += qxtglobalshortcut_mac.cpp
}

win32 {
SOURCES += qxtglobalshortcut_win.cpp
target.path = .
DESTDIR = .
sources.files = $$SOURCES test.pro
sources.path = $$DEPENDPATH
headers.path = $$DEPENDPATH
INSTALLS += target sources
}

# install
TARGET = pygs

win32:LIBS      += -luser32
macx:LIBS       += -framework Carbon
