#include "playlist.h"

#include <QDebug>

Playlist::Playlist(QObject *parent) :
    QObject(parent)
{
}

Playlist::~Playlist()
{
    qDebug() << "removed playlist " << this;
    foreach(Playback *playback, _playbackList) {
        playback->media()->usageCountAdd(-1);
        delete playback;
    }
}

Playback* Playlist::at(const int &index) const
{
    return _playbackList.at(index);
}

void Playlist::append(Playback *playback)
{
    playback->media()->usageCountAdd();
    _playbackList.append(playback);
}

void Playlist::removeAt(int index)
{
    _playbackList[index]->media()->usageCountAdd(-1);
    _playbackList.removeAt(index);
}

int Playlist::count() const
{
    return _playbackList.count();
}

uint Playlist::totalDuration() const
{
    uint duration = 0;

    foreach(Playback *playback, _playbackList) {
        duration += playback->media()->duration();
    }

    return duration;
}
