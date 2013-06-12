#include "playback.h"

Playback::Playback(Media *media, QObject *parent) :
    QObject(parent)
{
    _media = media;
    _mediaSettings = new MediaSettings();
    _mediaSettings->setAudioTrack(_media->audioTracks().count()==1 ? -1 : _media->audioTracks().at(1));
    _mediaSettings->setVideoTrack(_media->videoTracks().count()==1 ? -1 : _media->videoTracks().at(1));
}

Playback::Playback(const Playback &playback)
{
    _media=playback.media();
    _mediaSettings = new MediaSettings(*playback.mediaSettings());
}

Playback::~Playback()
{
    delete _mediaSettings;
}

Playback& Playback::operator =(const Playback &playback)
{
    if(this!=&playback) {
        _media=playback.media();
        _mediaSettings = new MediaSettings(*playback.mediaSettings());
    }
    return *this;
}

