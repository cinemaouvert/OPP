#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include "media.h"
#include "playback.h"

class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(QObject *parent = 0);
    virtual ~Playlist();

    inline QList<Playback>& playbackList() {return _playbackList;}
    inline const QList<Playback>& playbackList() const {return _playbackList;}
    
private:
    QList<Playback> _playbackList;
    
};

#endif // PLAYLIST_H
