#ifndef GLOBAL_H
#define GLOBAL_H

#include <vlc/vlc.h>
#include <QDebug>

#define VLCERR(CALL) do { \
    CALL; \
    if (libvlc_errmsg()) { \
        qDebug() << "libvlc" << "Error:" << libvlc_errmsg(); \
        libvlc_clearerr(); \
    } \
} while(0);

#define VLC_LAST_ERROR() do { \
    qDebug() << "libvlc" << "Error:" << libvlc_errmsg(); \
    libvlc_clearerr(); \
} while(0);

#endif // GLOBAL_H
