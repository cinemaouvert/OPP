#include "videotrack.h"

VideoTrack::VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    Track(vlcTrackInfo, parent)
{
}

VideoTrack::VideoTrack(QObject *parent) :
    Track(parent)
{
}
