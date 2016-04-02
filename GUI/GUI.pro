#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T15:15:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newserverdialog.cpp \
    downloadingfiledialog.cpp \
    status.cpp \
    ../Library/Library/Client.cpp



HEADERS  += mainwindow.h \
    newserverdialog.h \
    downloadingfiledialog.h \
    status.h \
    ../Library/Library/Client.h \
    ../Library/Library/DownloadingFile.h \
    ../Library/Library/FileStatus.h



FORMS    += mainwindow.ui \
    newserverdialog.ui \
    downloadingfiledialog.ui \
    status.ui

RESOURCES += \
    icons.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Library/release/ -lLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Library/debug/ -lLibrary
else:unix: LIBS += -L$$PWD/../Library/ -lLibrary

INCLUDEPATH += $$PWD/../Library/Library
DEPENDPATH += $$PWD/../Library/Library

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Library/release/libLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Library/debug/libLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Library/release/Library.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Library/debug/Library.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Library/libLibrary.a
