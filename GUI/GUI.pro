#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T01:52:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloadingfiledialog.cpp \
    newserverdialog.cpp \
    foundedfileform.cpp \
    status.cpp \
    fileform.cpp \
    descriptionform.cpp

HEADERS  += mainwindow.h \
    downloadingfiledialog.h \
    newserverdialog.h \
    foundedfileform.h \
    status.h \
    fileform.h \
    descriptionform.h

FORMS    += mainwindow.ui \
    downloadingfiledialog.ui \
    newserverdialog.ui \
    foundedfileform.ui \
    status.ui \
    fileform.ui \
    descriptionform.ui

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


INCLUDEPATH += C:/boost/VC/boost_1_60_0/boost_1_60_0
LIBS += C:\Boost\boost_vc\boost_lib\boost\bin.v2\libs\system\build\msvc-12.0\debug\link-static\threading-multi\libboost_system-vc120-mt-gd-1_60.lib
LIBS += C:\Boost\boost_vc\boost_lib\boost\bin.v2\libs\date_time\build\msvc-12.0\debug\link-static\threading-multi\libboost_date_time-vc120-mt-gd-1_60.lib
LIBS += C:\Boost\boost_vc\boost_lib\boost\bin.v2\libs\regex\build\msvc-12.0\debug\link-static\threading-multi\libboost_regex-vc120-mt-gd-1_60.lib

