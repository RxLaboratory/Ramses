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
    ramasset.cpp \
    addassetdialog.cpp \
    addshotsdialog.cpp \
    helpdialog.cpp \
    separatorwidget.cpp \
    assetstatusbox.cpp \
    assetdetailsdialog.cpp \
    xmlreader.cpp \
    adminwidget.cpp \
    shotwidget.cpp \
    shotassetswidget.cpp \
    maintable.cpp \
    updater.cpp \
    importerdialog.cpp \
    renamedialog.cpp \
    settingswidget.cpp \
    ramuuid.cpp

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
    ramasset.h \
    addassetdialog.h \
    addshotsdialog.h \
    helpdialog.h \
    separatorwidget.h \
    assetstatusbox.h \
    assetdetailsdialog.h \
    xmlreader.h \
    adminwidget.h \
    shotwidget.h \
    shotassetswidget.h \
    maintable.h \
    updater.h \
    importerdialog.h \
    renamedialog.h \
    settingswidget.h \
    ramuuid.h

FORMS    += mainwindow.ui \
    projectselectorwidget.ui \
    addassetdialog.ui \
    addshotsdialog.ui \
    helpdialog.ui \
    separatorwidget.ui \
    assetdetailsdialog.ui \
    adminwidget.ui \
    shotwidget.ui \
    shotassetswidget.ui \
    assetstatusbox.ui \
    maintable.ui \
    importerdialog.ui \
    renamedialog.ui \
    settingswidget.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
