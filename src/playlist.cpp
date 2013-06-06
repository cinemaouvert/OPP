#include "playlist.h"

Playlist::Playlist(QObject *parent) :
    QObject(parent)
{
}

Playlist::~Playlist()
{
}

Playback* Playlist::at(const int &index) const
{
    return const_cast<Playback*>(&_playbackList.at(index));
}

void Playlist::append(const Playback &playback)
{
    _playbackList.append(playback);
}

int Playlist::count() const
{
    return _playbackList.count();
}

uint Playlist::totalDuration() const
{
    uint duration = 0;

    foreach(const Playback &playback, _playbackList) {
        duration += playback.media()->duration();
    }

    return duration;
}
