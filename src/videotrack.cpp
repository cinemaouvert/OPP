#include "videotrack.h"

VideoTrack::VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    Track(vlcTrackInfo, parent)
{
}

VideoTrack::VideoTrack(QObject *parent) :
    Track(parent)
{
}

QSize VideoTrack::size() const
{
    return QSize(_vlcTrackInfo.u.video.i_width, _vlcTrackInfo.u.video.i_height);
}

uint VideoTrack::width() const
{
    return _vlcTrackInfo.u.video.i_width;
}

uint VideoTrack::height() const
{
    return _vlcTrackInfo.u.video.i_height;
}
