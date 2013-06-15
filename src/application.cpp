#include "application.h"

#include <QStringList>
#include <QDebug>

#include <vlc/vlc.h>

Application::Application()
{
    QStringList vlcargs;
        vlcargs << "--intf=dummy"
                <<"--no-media-library"
    #if !defined(Q_OS_MAC)
                  <<"--no-one-instance"
    #endif
                       <<"--no-plugins-cache"
                       <<"--no-stats"
                       <<"--no-osd"
                       <<"--no-loop"
                       <<"--no-video-title-show"
    #if defined(Q_OS_MAC)
                      << "--vout=macosx"
                      << "--data-path=/Applications/VLC.app/Contents/MacOS/plugins"
    #endif
                       <<"--drop-late-frames";
    initVlcInstanceFromArgs(vlcargs);
}

Application::~Application()
{
    libvlc_release(_vlcInstance);
}

void Application::initVlcInstanceFromArgs(const QStringList &args)
{
    char **argV = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); ++i)
        argV[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    // Create new libvlc instance
    _vlcInstance = libvlc_new(args.count(), argV);

    if (_vlcInstance) {
        qDebug() << "Using libvlc version:" << QString(libvlc_get_version());
    } else {
        qDebug() << "opp error: libvlc failed to load!";
        abort();
    }
}
