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
    assetstatuswidget.cpp \
    ramasset.cpp \
    addassetdialog.cpp \
    addshotsdialog.cpp \
    helpdialog.cpp \
    separatorwidget.cpp \
    assetstatusbox.cpp \
    shotdetailsdialog.cpp \
    assetdetailsdialog.cpp \
    xmlreader.cpp \
    ramassignment.cpp

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
    assetstatuswidget.h \
    ramasset.h \
    addassetdialog.h \
    addshotsdialog.h \
    helpdialog.h \
    separatorwidget.h \
    assetstatusbox.h \
    shotdetailsdialog.h \
    assetdetailsdialog.h \
    xmlreader.h \
    ramassignment.h

FORMS    += mainwindow.ui \
    projectselectorwidget.ui \
    assetstatuswidget.ui \
    addassetdialog.ui \
    addshotsdialog.ui \
    helpdialog.ui \
    separatorwidget.ui \
    assetstatusbox.ui \
    shotdetailsdialog.ui \
    assetdetailsdialog.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
