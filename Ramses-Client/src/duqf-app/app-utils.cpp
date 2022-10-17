#include "app-utils.h"

#ifdef Q_OS_WIN
#include "windows.h"
#endif

#include "duqf-app/app-style.h"

DuApplication::DuApplication(int &argc, char *argv[]) : QApplication(argc, argv)
{
#ifndef QT_DEBUG
    // handles messages from the app and redirects them to stdout (info) or stderr (debug, warning, critical, fatal)
    qInstallMessageHandler(MessageHandler::messageOutput);
#endif
    qDebug() << "Initializing application";

    //set style
    DuUI::updateCSS(":/styles/default");

    //create splash screen
    if (QFile(SPLASH_IMAGE).exists())
    {
        QPixmap pixmap(SPLASH_IMAGE);
        _splashScreen = new DuSplashScreen( pixmap );
    }
    else
    {
        _splashScreen = new DuSplashScreen( );
    }

    //set app icon
    qApp->setWindowIcon(QIcon(APP_ICON));

    // settings
    QCoreApplication::setOrganizationName(STR_COMPANYNAME);
    QCoreApplication::setOrganizationDomain(STR_COMPANYDOMAIN);
    QCoreApplication::setApplicationName(STR_PRODUCTNAME);
    QCoreApplication::setApplicationVersion(STR_VERSION);

    //idle timer
    _idleTimer = new QTimer();
    connect(_idleTimer,SIGNAL(timeout()),this,SLOT(idleTimeOut()));
    _idleTimeout=120*60*1000;
    _idleTimer->start(_idleTimeout);

    for (int i = 1; i < argc; i++)
    {
        _args << QString(argv[i]);
    }
}

DuSplashScreen *DuApplication::splashScreen() const
{
    return _splashScreen;
}

void DuApplication::showSplashScreen()
{
    _splashScreen->show();
}

void DuApplication::setIdleTimeOut(int to)
{
    _idleTimeout = to;
    _idleTimer->start(_idleTimeout);
}

bool DuApplication::processArgs(QStringList examples, QStringList helpStrings)
{
    bool nobanner = false;
    bool help = false;

    // No console without args on windows
#ifdef Q_OS_WIN
    bool hideConsole = _args.count() == 0;
#endif
    for (int i = 0; i < _args.count(); i++)
    {
        QString arg = _args.at(i);
        if ( arg.toLower() == "--no-banner" ) nobanner = true;
        if (arg.toLower() == "-h" || arg.toLower() == "--help" ) help = true;
#ifdef Q_OS_WIN
        if (arg.toLower() == "--hide-console") hideConsole = true;
#endif
    }

    if (!nobanner)
    {
        qInfo().noquote() << STR_PRODUCTNAME;
        qInfo().noquote() << "version " + QString(STR_VERSION);
        qInfo().noquote() << STR_LEGALCOPYRIGHT;
        qInfo() << "";
        qInfo() << "This program comes with ABSOLUTELY NO WARRANTY;";
        qInfo() << "This is free software, and you are welcome to redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.";
        qInfo() << "";

#ifdef QT_DEBUG
        qInfo() << "Note: this is a development build and should not be used in production.";
        qInfo() << "";
#endif
    }

    if (help)
    {
        qInfo().noquote() << STR_PRODUCTDESCRIPTION;
        foreach(QString example, examples)
        {
            qInfo().noquote() << example;
        }
        qInfo() << "General";
        qInfo() << "    -h / --help       Print basic options without launching the application";
        qInfo().noquote() << "    See the documentation at " + QString(URL_DOC) + " for detailed descriptions of the options";
        qInfo() << "    --no-banner       Hides the banner with product information and legal notice";
        foreach(QString h, helpStrings)
        {
            qInfo().noquote() << h;
        }
#ifdef Q_OS_WIN
        qInfo() << "    --hide-console    Hides the console when starting the application using command line arguments";
#endif
    }

#ifdef Q_OS_WIN
#ifndef QT_DEBUG
    if (hideConsole)
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
#endif
#endif

    return help;
}

void DuApplication::checkUpdate()
{
    if (QString(URL_UPDATE) == "") return;

    // Get Sys info
#ifdef Q_OS_LINUX
    QString os("linux");
    QString distrib = QSysInfo::productType() % "-" % QSysInfo::productVersion();
    QString kernel = QSysInfo::kernelType() % "-" % QSysInfo::kernelVersion();
#endif
#ifdef Q_OS_WIN
    QString os("win");
    QString distrib = QSysInfo::productVersion();
    QString kernel = QSysInfo::kernelType() % "-" % QSysInfo::kernelVersion();
#endif
#ifdef Q_OS_MAC
    QString os("mac");
    QString distrib = QSysInfo::productType() % "-" % QSysInfo::productVersion();
    QString kernel = QSysInfo::kernelType() % "-" % QSysInfo::kernelVersion();
#endif
    // Build request
    QString serverAddress(URL_UPDATE);
    if (!serverAddress.endsWith("/")) serverAddress += "/";
    QUrl url(serverAddress %
             "?getVersion&name=" %
             QString(STR_INTERNALNAME) %
             "&version=" % QString(STR_VERSION) %
             "&os=" % os %
             "&osVersion=" % distrib % " (" % kernel % ")" %
             "&languageCode=en"
             );
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString(STR_INTERNALNAME) + " v" + QString(STR_VERSION));
    QNetworkAccessManager *am = new QNetworkAccessManager(this);

    qInfo() << "Checking for update... This is the (only) info sent to the server at " % QString(URL_UPDATE) % ":";
    qInfo().noquote() << "OS: " % os;
    qInfo().noquote() << "OS Version: " % distrib % " (" % kernel % ")";
    qInfo().noquote() << "App Name: " % QString(STR_INTERNALNAME);
    qInfo().noquote() << "App Version: " % QString(STR_VERSION);
    qInfo().noquote() << "Language code: en";

    connect(am, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotUpdateInfo(QNetworkReply*)));
    am->get(request);
}

bool DuApplication::notify(QObject *receiver, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseButtonPress)
    {
        //reset timer
        _idleTimer->start(_idleTimeout);
    }
    return QApplication::notify(receiver,ev);
}

void DuApplication::idleTimeOut()
{
    emit idle();
}

void DuApplication::gotUpdateInfo(QNetworkReply *rep)
{
    if (rep->error() != QNetworkReply::NoError)
    {
        qDebug() << "Update Check failed!";
        qDebug() << rep->errorString();
        return;
    }

    QString repAll = rep->readAll();

    qDebug() << repAll;

    QJsonDocument repDoc = QJsonDocument::fromJson(repAll.toUtf8());
    _updateInfo = repDoc.object();

    if (_updateInfo.isEmpty())
    {
        _updateInfo.insert("message",repAll);
        _updateInfo.insert("accepted",false);
        _updateInfo.insert("success",false);
    }

    if ( _updateInfo.value("success").toBool() )
    {
        QSettings settings;
        settings.setValue("updates/latestUpdateCheck", QDate::currentDate());

        qInfo() << "Got update information:";
        if (_updateInfo.value("update").toBool())
        {
            qInfo().noquote() << "A new version is available! Version: " % _updateInfo.value("version").toString();
            qInfo().noquote() << "(Current version is: " % QString(STR_VERSION) % ")";
            qInfo().noquote() << "Update notes: " % _updateInfo.value("description").toString();
            qInfo().noquote() << "Detailed changelog: " % _updateInfo.value("changelogURL").toString();
            qInfo().noquote() << "Download: " % _updateInfo.value("downloadURL").toString();
        }
        else
        {
            qInfo() << "this version is already up-to-date, congrats!";
        }
        qInfo().noquote() << "Donate: " % _updateInfo.value("donateURL").toString();
        double month = _updateInfo.value("monthlyFund").toDouble(0.0);
        double goal = _updateInfo.value("fundingGoal").toDouble(4000);
        if (goal > 0) {
            double ratio = month / goal * 100;
            qInfo().noquote() << "This month, we've collected $" % QString::number(month) % ". That's " % QString::number(ratio, 'f', 0) % " % of our monthly goal. Thanks for your support!";
        }

        // Save to settings
        settings.setValue("updates/version", _updateInfo.value("version"));
        settings.setValue("updates/description", _updateInfo.value("description"));
        settings.setValue("updates/changelogURL", _updateInfo.value("changelogURL"));
        settings.setValue("updates/downloadURL", _updateInfo.value("downloadURL"));
        settings.setValue("updates/donateURL", _updateInfo.value("donateURL"));
        settings.setValue("updates/monthlyFund", _updateInfo.value("monthlyFund"));
        settings.setValue("updates/fundingGoal", _updateInfo.value("fundingGoal").toDouble(4000));
    }

    emit newUpdateInfo(_updateInfo);

    rep->deleteLater();
}

const QJsonObject &DuApplication::updateInfo()
{
    if (!_updateInfo.isEmpty() && _updateInfo.value("success").toBool())
        return _updateInfo;

    // Build from settigns
    qDebug() << "Using saved update info.";

    QSettings settings;
    _updateInfo.insert("message","");
    _updateInfo.insert("accepted",true);
    _updateInfo.insert("success",true);
    _updateInfo.insert("version", settings.value("updates/version").toString());
    _updateInfo.insert("description", settings.value("updates/description").toString());
    _updateInfo.insert("changelogURL", settings.value("updates/changelogURL").toString());
    _updateInfo.insert("downloadURL", settings.value("updates/downloadURL").toString());
    _updateInfo.insert("donateURL", settings.value("updates/donateURL").toString());
    _updateInfo.insert("monthlyFund", settings.value("updates/monthlyFund").toDouble());
    _updateInfo.insert("fundingGoal", settings.value("updates/fundingGoal", 4000).toDouble());

    return _updateInfo;
}
