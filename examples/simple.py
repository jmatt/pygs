# python simple.py &

__author__ = "J. Matt Peterson"
__version__ = "$Revision: 0.11"
__date__ = "$Date: 2010/06/29 10:34:00 $"

import sys

from PyQt4 import QtCore, QtGui
import pygs

SHORTCUT = "Alt+Ctrl+S" #Ctrl maps to Command on Mac OS X
QUIT_SHORTCUT = "Alt+Ctrl+F" #again, Ctrl maps to Command on Mac OS X

def activated():
    print "Shortcut Activated!"

def finito():
    app.quit()

app = QtGui.QApplication(list(""))

shortcut = pygs.QxtGlobalShortcut()
shortcut.setShortcut(QtGui.QKeySequence(SHORTCUT))
shortcut.connect(shortcut, QtCore.SIGNAL("activated()"), activated)

fin = pygs.QxtGlobalShortcut()
fin.setShortcut(QtGui.QKeySequence(QUIT_SHORTCUT))
app.connect(fin, QtCore.SIGNAL("activated()"), finito)

# Could also map directly using app
# app.connect(fin, QtCore.SIGNAL("activated()"), app, QtCore.SLOT("exit()"))

appresult = app.exec_()
del(shortcut)
del(fin)
sys.exit(appresult)

