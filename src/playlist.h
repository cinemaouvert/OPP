#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

#include "playback.h"

struct libvlc_media_list_t;
struct libvlc_instance_t;

class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(libvlc_instance_t *vlcInstance, const QString &title, QObject *parent = 0);
    virtual ~Playlist();

    inline const QList<Playback*>& playbackList() { return _playbackList; }

    inline libvlc_media_list_t* core() const { return _vlcMediaList; }

    inline QString title() const { return _title; }

    void setTitle(const QString &title);

    Playback* at(const int &index) const;

    void append(Playback *playback);

    void removeAt(int index);

    int indexOf(Playback *playback) const;

    int count() const;

    uint totalDuration() const;

    void lock();

    void unlock();

private:
    QList<Playback*> _playbackList;

    libvlc_media_list_t *_vlcMediaList;
    
    QString _title;
};

#endif // PLAYLIST_H
