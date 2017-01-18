#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T13:45:02
#
#-------------------------------------------------

QT       += core gui \
        sql \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RAM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    projectselectorwidget.cpp \
    mainwindowdragevent.cpp \
    dbinterface.cpp \
    ramstatus.cpp \
    ramstage.cpp \
    idletimer.cpp \
    application.cpp \
    ramproject.cpp \
    ramshot.cpp \
    ramstagestatus.cpp \
    shotstatuswidget.cpp \
    assetstatuswidget.cpp \
    ramasset.cpp \
    addassetdialog.cpp \
    addshotsdialog.cpp \
    helpdialog.cpp \
    separatorwidget.cpp

HEADERS  += mainwindow.h \
    projectselectorwidget.h \
    mainwindowdragevent.h \
    dbinterface.h \
    ramstatus.h \
    ramstage.h \
    idletimer.h \
    application.h \
    ramproject.h \
    ramshot.h \
    ramstagestatus.h \
    shotstatuswidget.h \
    assetstatuswidget.h \
    ramasset.h \
    addassetdialog.h \
    addshotsdialog.h \
    helpdialog.h \
    separatorwidget.h

FORMS    += mainwindow.ui \
    projectselectorwidget.ui \
    shotstatuswidget.ui \
    assetstatuswidget.ui \
    addassetdialog.ui \
    addshotsdialog.ui \
    helpdialog.ui \
    separatorwidget.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
