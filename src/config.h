#ifndef CONFIG_H
#define CONFIG_H



#include <QObject>

namespace config_opp {
    const QString URL = "http://localhost/";
    const QString VERSION = "0.9.3";

    #if defined(Q_OS_MAC)
        const QString LINK_UPDATE = "macos/latest/";
        const QString NAME_INSTALLER = "setup_opp.exe";
    #elif defined(Q_OS_WIN)
        const QString LINK_UPDATE = "windows/latest/";
        const QString NAME_INSTALLER = "setup_opp.exe";
    #elif defined(Q_OS_UNIX)
        const QString LINK_UPDATE = "unix/latest/";
        const QString NAME_INSTALLER = "opp.deb";
    #endif
}




#endif // CONFIG_H
