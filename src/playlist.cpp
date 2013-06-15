#include "playlist.h"

#include <QDebug>

#include "global.h"
#include "media.h"

int Playlist::s_instanceCount = 0;

Playlist::Playlist(libvlc_instance_t *vlcInstance, const QString &title, QObject *parent) :
    QObject(parent),
    _title(title)
{
    _id = Playlist::s_instanceCount++;
    _vlcMediaList = libvlc_media_list_new(vlcInstance);
}

Playlist::~Playlist()
{ 
    foreach(Playback *playback, _playbackList) {
        playback->media()->usageCountAdd(-1);
        delete playback;
    }

    libvlc_media_list_release(_vlcMediaList);
}

void Playlist::setId(int id)
{
    _id = id;
}

void Playlist::setTitle(const QString &title)
{
    _title = title;
    emit titleChanged();
}

Playback* Playlist::at(const int &index) const
{
    return _playbackList.at(index);
}

void Playlist::append(Playback *playback)
{
    lock();

    playback->media()->usageCountAdd();

    libvlc_media_list_add_media(_vlcMediaList, playback->media()->core());
    _playbackList.append(playback);

    unlock();
}

void Playlist::removeAt(int index)
{
    lock();

    _playbackList[index]->media()->usageCountAdd(-1);

    libvlc_media_list_remove_index(_vlcMediaList, index);
    delete _playbackList[index];
    _playbackList.removeAt(index);

    unlock();
}

int Playlist::indexOf(Playback *playback) const
{
    return _playbackList.indexOf(playback);
}

void Playlist::move(int from, int to)
{
    _playbackList.move(from, to);
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

void Playlist::lock()
{
    libvlc_media_list_lock(_vlcMediaList);
}

void Playlist::unlock()
{
    libvlc_media_list_unlock(_vlcMediaList);
}
