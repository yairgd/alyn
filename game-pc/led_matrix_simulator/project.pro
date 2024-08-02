#-------------------------------------------------
#
# Project created by QtCreator 2023-10-06T00:00:00
#
#-------------------------------------------------

QT += widgets

TARGET = ChessboardApp
TEMPLATE = app

# Add the path to your source files here
SOURCES += main.cpp \
    chessboardwidget.cpp \
    mainwindow.cpp


HEADERS += chessboardwidget.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
