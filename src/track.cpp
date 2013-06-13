#include "track.h"

#include <vlc/plugins/vlc_fourcc.h>
#include <vlc/plugins/vlc_common.h>
#include <vlc/plugins/vlc_es.h>

#include <QDebug>

Track::Track(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    QObject(parent),
    _vlcTrackInfo(*vlcTrackInfo)
{
}

Track::Track(QObject *parent) :
    QObject(parent)
{
    _vlcTrackInfo.i_id = -1;
}

Track::Track(const Track &track)
{
    _vlcTrackInfo = track._vlcTrackInfo;
}

Track::~Track()
{
}

Track & Track::operator=(const Track &track)
{
    if (this != &track) {
        _vlcTrackInfo = track._vlcTrackInfo;
    }

    return *this;
}

bool Track::operator==(const Track &track)
{
    return _vlcTrackInfo.i_id == track._vlcTrackInfo.i_id;
}

QString Track::codecDescription() const
{
    return QString( vlc_fourcc_GetDescription(UNKNOWN_ES, _vlcTrackInfo.i_codec) );
}
