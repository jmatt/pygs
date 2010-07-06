CONFIG += qt
QT = core gui
QXT = core
DEFINES = BUILD_QXT_GUI BUILD_QXT_CORE
TEMPLATE = lib
DEPENDPATH += . ../libqxt/src/core ../libqxt/src/gui
INCLUDEPATH += $$DEPENDPATH
HEADERS += qxtglobalshortcut.h

SOURCES += qxtglobal.cpp \
           qxtglobalshortcut.cpp \
           qxtglobalshortcut_win.cpp \

# install
TARGET = qxtgs
target.path = .
DESTDIR = .
sources.files = $$SOURCES test.pro
sources.path = $$DEPENDPATH
headers.path = $$DEPENDPATH
INSTALLS += target sources

win32:LIBS      += -luser32
macx:LIBS       += -framework Carbon
