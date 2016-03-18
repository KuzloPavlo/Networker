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
    status.cpp

HEADERS  += mainwindow.h \
    newserverdialog.h \
    downloadingfiledialog.h \
    status.h

FORMS    += mainwindow.ui \
    newserverdialog.ui \
    downloadingfiledialog.ui \
    status.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    ../Resources/Icons/acquisition.png \
    ../Resources/Icons/blank.png \
    ../Resources/Icons/check.png \
    ../Resources/Icons/defrag.png \
    ../Resources/Icons/Gorillaz.png \
    ../Resources/Icons/Hardware ad alt.png \
    ../Resources/Icons/Home alt.png \
    ../Resources/Icons/pixadex.png \
    ../Resources/Icons/sherlock.png \
    ../Resources/Icons/softwareupdate.png \
    ../Resources/Icons/winrar.png
