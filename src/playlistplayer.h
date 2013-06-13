#ifndef PLAYLISTPLAYER_H
#define PLAYLISTPLAYER_H

#include <QObject>

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

    explicit PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    
    ~PlaylistPlayer();

    inline libvlc_media_list_player_t* core() const { return _vlcMediaListPlayer; }

    inline MediaPlayer* mediaPlayer() const { return _mediaPlayer; }

    Playlist* currentPlaylist() const { return _playlist; }

    void setPlaylist(Playlist *list);

    bool isPlaying() const;

public slots:

    void playItemAt(const int &index);

    void next();

    void previous();

    void stop();

    void playPlaylist(Playlist *playlist);

    void applyCurrentPlaybackSettings();

private slots:

    void handlePlayerEnd();

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

    int _currentIndex;
};

#endif // PLAYLISTPLAYER_H
