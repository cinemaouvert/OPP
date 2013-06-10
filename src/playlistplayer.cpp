#include "playlistplayer.h"

#include "playlist.h"
#include "media.h"
#include "mediaplayer.h"
#include "global.h"

PlaylistPlayer::PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _currentIndex(-1)
{
    _mediaPlayer = new MediaPlayer(vlcInstance, this);

    _vlcMediaListPlayer = libvlc_media_list_player_new(vlcInstance);
    _vlcEvents = libvlc_media_list_player_event_manager(_vlcMediaListPlayer);

    libvlc_media_list_player_set_media_player(_vlcMediaListPlayer, _mediaPlayer->core());
    createCoreConnections();

    connect(_mediaPlayer, SIGNAL(end()), this, SLOT(handlePlaylistEnd()));
    connect(this, SIGNAL(end()), _mediaPlayer, SLOT(stop()));
}

PlaylistPlayer::~PlaylistPlayer()
{
    removeCoreConnections();

    delete _mediaPlayer;
    libvlc_media_list_player_release(_vlcMediaListPlayer);
}

void PlaylistPlayer::setPlaylist(Playlist *playlist)
{
    _playlist = playlist;
    libvlc_media_list_player_set_media_list(_vlcMediaListPlayer, playlist->core());
}

bool PlaylistPlayer::isPlaying() const
{
    return libvlc_media_list_player_is_playing(_vlcMediaListPlayer);
}

void PlaylistPlayer::playItemAt(const int &index)
{
    _mediaPlayer->_isPaused = false;
    _currentIndex = index - 1;

    libvlc_media_list_player_play_item_at_index(_vlcMediaListPlayer, index);
}

void PlaylistPlayer::next()
{
    libvlc_media_list_player_next(_vlcMediaListPlayer);
}

void PlaylistPlayer::play()
{
    _mediaPlayer->_isPaused = false;

    libvlc_media_list_player_play(_vlcMediaListPlayer);
}

void PlaylistPlayer::previous()
{
    _currentIndex -= 2;

    libvlc_media_list_player_previous(_vlcMediaListPlayer);
}

void PlaylistPlayer::stop()
{
    _mediaPlayer->_isPaused = false;
    _currentIndex = -1;

    libvlc_media_list_player_stop(_vlcMediaListPlayer);
}

void PlaylistPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    PlaylistPlayer *core = (PlaylistPlayer *)data;

    switch(event->type)
    {
    case libvlc_MediaListPlayerNextItemSet:
        core->_currentIndex++;
        emit core->itemChanged(core->_currentIndex);
        break;
    default:
        break;
    }
}

void PlaylistPlayer::createCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerNextItemSet;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(_vlcEvents, event, libvlc_callback, this);
    }
}

void PlaylistPlayer::removeCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerNextItemSet;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_detach(_vlcEvents, event, libvlc_callback, this);
    }
}

void PlaylistPlayer::handlePlaylistEnd()
{
    if (_currentIndex == _playlist->count() - 1) {
        emit end();
    }
}
