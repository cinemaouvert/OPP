
#include <QDebug>
#include <QStringList>

#include <vlc/vlc.h>

#include "config.h"

namespace config_opp {
    QString getOppVersion(){
        int libvlcMajorVersion = getLibVlcMajor();

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

    QString getLibVlcVersion(){
        return QString(libvlc_get_version());
    }

    /** get the libvlc major version */
    int getLibVlcMajor(){
        QString libvlcVersion = getLibVlcVersion();
        for(int i = 0; i < libvlcVersion.size(); ++i){
            if(libvlcVersion.at(i) != QChar(' ')){
                return libvlcVersion.at(i).digitValue() ;
            }
        }

        return 0;
    }

    int getLibVlcMinor(){
        QString libvlcVersion = getLibVlcVersion();
        QStringList list = libvlcVersion.split(".");
        return list.at(1).toInt();
    }

    int getLibVlcPatch(){
        QString libvlcVersion = getLibVlcVersion();
        QStringList list1 = libvlcVersion.split(".");
        QString lastPart = list1.at(2);
        QStringList list2 = lastPart.split(" ");
        return list2.at(0).toInt();
    }
}
