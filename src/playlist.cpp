#include "playlist.h"

Playlist::Playlist(QObject *parent) :
    QObject(parent)
{
}

Playlist::~Playlist()
{
}

const Playback &Playlist::at(const int &index) const
{
    return _playbackList.at(index);
}

void Playlist::append(const Playback &playback)
{
    _playbackList.append(playback);
}

int Playlist::count() const
{
    return _playbackList.count();
}
