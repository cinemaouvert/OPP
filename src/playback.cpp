#include "playback.h"

Playback::Playback(Media *media, QObject *parent) :
    QObject(parent)
{
    _media = media;
}

Playback::Playback(const Playback &playback)
{
    _media=playback.media();
    _mediaSettings=playback.mediaSettings();
}

Playback::~Playback()
{

}

Playback& Playback::operator =(const Playback &playback)
{
    if(this!=&playback)
    {

    }
    return *this;
}

