#include "audiotrack.h"

AudioTrack::AudioTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent) :
    Track(vlcTrackInfo, parent)
{
}

AudioTrack::AudioTrack(QObject *parent) :
    Track(parent)
{
}

uint AudioTrack::rate() const
{
    return _vlcTrackInfo.u.audio.i_rate;
}
