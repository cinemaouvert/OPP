#ifndef PLAYLISTPLAYER_H
#define PLAYLISTPLAYER_H

#include <QObject>
#include "videoview.h"

class MediaPlayer;
class Playlist;

struct libvlc_media_list_player_t;
struct libvlc_media_list_t;
struct libvlc_media_t;
struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_instance_t;

class PlaylistPlayer : public QObject
{
    Q_OBJECT
public:
    enum PlaybackMode {
        DefaultPlayback = 0,
        Loop = 1,
        Repeat = 2
    };

    explicit PlaylistPlayer(libvlc_instance_t *vlcInstance, VideoView *video, PlaybackMode mode = DefaultPlayback, QObject *parent = 0);
    
    ~PlaylistPlayer();

    inline libvlc_media_list_player_t* core() const { return _vlcMediaListPlayer; }

    inline MediaPlayer* mediaPlayer() const { return _mediaPlayer; }

    Playlist* currentPlaylist() const { return _playlist; }

    inline PlaybackMode mode() const { return _mode; }

    void setPlaylist(Playlist *list);

    void setMode(const PlaybackMode &mode);

    bool isPlaying() const;

public slots:

    void playItemAt(const int &index);

    void next();

    void play();

    void previous();

    void stop();

private slots:

    void handlePlaylistEnd();

signals:

    void itemChanged(int);

    void nextItem();

    void end();

private:
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_list_player_t *_vlcMediaListPlayer;
    libvlc_event_manager_t *_vlcEvents;

    Playlist *_playlist;

    MediaPlayer *_mediaPlayer;

    PlaybackMode _mode;

    int _currentIndex;
};

#endif // PLAYLISTPLAYER_H
