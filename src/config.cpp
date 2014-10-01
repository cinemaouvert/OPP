
#include <QDebug>

#include <vlc/vlc.h>

#include "config.h"

namespace config_opp {
    QString getVersion(){
        /** get the libvlc major version */
        QString libvlcVersion = QString(libvlc_get_version());
        int libvlcMajorVersion;
        for(int i = 0; i < libvlcVersion.size(); ++i){
            if(libvlcVersion.at(i) != QChar(' ')){
                libvlcMajorVersion = libvlcVersion.at(i).digitValue() ;
                break;
            }
        }

        /** check if the build is possible */
        if(libvlcMajorVersion > 0){
            #if (QT_VERSION >= 0x050000) // Qt version 5 and above
                if(libvlcMajorVersion >= 3){
                    return QString("v2.0-stable");
                }else{
                    qDebug() << "OPP error: you must use a VLC 3.X core";
                    abort();
                }
            #else // until version 5
                if(libvlcMajorVersion < 3){
                    return QString("v1.0-stable");
                }else{
                    qDebug() << "OPP error: you must use a Qt 5.X core.";
                    abort();
                }
            #endif
        }

        #if (QT_VERSION >= 0x050000)
            qDebug() << "OPP error: unable to determine the used libvlc version.";
            abort();
        #else
            return QString("v1.0-stable");
        #endif
    }
}
