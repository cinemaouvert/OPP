#include "playback.h"

Playback::Playback(Media *media, QObject *parent) :
    QObject(parent)
{
    _media = media;
    _mediaSettings = new MediaSettings();

    if (_media->videoTracks().count() > 0)
        _mediaSettings->setVideoTrack(0);

    if (_media->audioTracks().count() > 0)
        _mediaSettings->setAudioTrack(1);

    _mediaSettings->setOutMark(_media->duration());
}

Playback::~Playback()
{
    delete _mediaSettings;
}
