#ifndef APPVERSION_H
#define APPVERSION_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 5
#define VERSION_BUILD 0
#define VERSION_SUFFIX "Beta3"

#define STRINGIFY_VERSION(A, B, C) CONCAT(A, B, C )
#define CONCAT(A, B, C ) STRINGIFY( A##.##B##.##C )
#define STRINGIFY(A) #A

#ifdef QT_DEBUG
#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX "_development build"
#else
#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX
#endif

#define VER_FILEVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_FILEVERSION STR_VERSION

// Keep the product version as fixed
#define VER_PRODUCTVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_PRODUCTVERSION STR_VERSION

#define STR_COMPANYNAME "RxLaboratory"
#define STR_FILEDESCRIPTION "Ramses"
#define STR_INTERNALNAME "Ramses-Client"
#define STR_LEGALCOPYRIGHT "Copyright (c) 2021-2022 RxLaboratory, Nicolas Dufresne and contributors"
#define STR_LEGALTRADEMARKS1 "All Rights Reserved"
#define STR_ORIGINALFILENAME "Ramses.exe"
#define STR_PRODUCTNAME "Ramses - Rx Asset Management System"
#define STR_PRODUCTDESCRIPTION "Client application for the Rx Asset and production management system for motion pictures production."

#define STR_COMPANYDOMAIN "rxlaboratory.org"

#define URL_CHAT "http://chat.rxlab.info"
#define URL_FORUM ""
#define URL_DOC "http://ramses.rxlab.guide"
#define URL_SOURCECODE "https://github.com/RxLaboratory/Ramses"
#define URL_BUGREPORT "https://github.com/RxLaboratory/Ramses/issues/new/choose"
#define URL_DONATION "http://donate.rxlab.info"
#define URL_UPDATE "http://api.rxlab.io"

#define APP_ICON ":/icons/app"
#define SPLASH_IMAGE ":/images/splash"

#define USE_SYSTRAY true
#define SYSTRAY_ICON_LIGHT ":/icons/tray-color"
#define SYSTRAY_ICON_COLOR ":/icons/tray-color"
#define SYSTRAY_ICON_DARK ":/icons/tray-color"

#endif // APPVERSION_H
