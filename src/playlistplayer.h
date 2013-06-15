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

    /**
     * @brief core
     * @return
     */
    inline libvlc_media_list_player_t* core() const { return _vlcMediaListPlayer; }

    /**
     * @brief mediaPlayer
     * @return
     */
    inline MediaPlayer* mediaPlayer() const { return _mediaPlayer; }

    /**
     * @brief currentPlaylist
     * @return
     */
    Playlist* currentPlaylist() const { return _playlist; }

    /**
     * @brief setPlaylist
     * @param list
     */
    void setPlaylist(Playlist *list);

    /**
     * @brief isPlaying
     * @return
     */
    bool isPlaying() const;

public slots:

    /**
     * @brief playItemAt
     * @param index
     */
    void playItemAt(const int &index);

    /**
     * @brief next
     */
    void next();

    /**
     * @brief previous
     */
    void previous();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief playPlaylist
     * @param playlist
     */
    void playPlaylist(Playlist *playlist);

    /**
     * @brief applyCurrentPlaybackSettings
     */
    void applyCurrentPlaybackSettings();

private slots:

    /**
     * @brief handlePlayerEnd
     */
    void handlePlayerEnd();

signals:

    /**
     * @brief itemChanged
     */
    void itemChanged(int);

    /**
     * @brief nextItem
     */
    void nextItem();

    /**
     * @brief end
     */
    void end();

private:

    /**
     * @brief libvlc_callback
     * @param event
     * @param data
     */
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    /**
     * @brief createCoreConnections
     */
    void createCoreConnections();

    /**
     * @brief removeCoreConnections
     */
    void removeCoreConnections();

    /**
     * @brief _vlcMediaListPlayer
     */
    libvlc_media_list_player_t *_vlcMediaListPlayer;

    /**
     * @brief _vlcEvents
     */
    libvlc_event_manager_t *_vlcEvents;

    /**
     * @brief _playlist
     */
    Playlist *_playlist;

    /**
     * @brief _mediaPlayer
     */
    MediaPlayer *_mediaPlayer;

    /**
     * @brief _currentIndex
     */
    int _currentIndex;
};

#endif // PLAYLISTPLAYER_H
