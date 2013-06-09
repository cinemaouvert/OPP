#include "playlistplayer.h"

#include "playlist.h"
#include "media.h"
#include "mediaplayer.h"
#include "global.h"

PlaylistPlayer::PlaylistPlayer(libvlc_instance_t *vlcInstance, VideoView *video, PlaybackMode mode, QObject *parent) :
    QObject(parent),
    _mode(mode)
{

    _mediaPlayer = new MediaPlayer(vlcInstance, this);
    _mediaPlayer->setVideoView(video);

    _vlcMediaListPlayer = libvlc_media_list_player_new(vlcInstance);
    _vlcEvents = libvlc_media_list_player_event_manager(_vlcMediaListPlayer);

    libvlc_media_list_player_set_media_player(_vlcMediaListPlayer, _mediaPlayer->core());
    createCoreConnections();
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

void PlaylistPlayer::setMode(const PlaybackMode &mode)
{
    _mode = mode;
    libvlc_media_list_player_set_playback_mode(_vlcMediaListPlayer, libvlc_playback_mode_t(mode));
}

void PlaylistPlayer::playItemAt(const int &index)
{
    libvlc_media_list_player_play_item_at_index(_vlcMediaListPlayer, index);
}

void PlaylistPlayer::next()
{
    libvlc_media_list_player_next(_vlcMediaListPlayer);
}

void PlaylistPlayer::play()
{
    libvlc_media_list_player_play(_vlcMediaListPlayer);
}

void PlaylistPlayer::previous()
{
    libvlc_media_list_player_previous(_vlcMediaListPlayer);
}

void PlaylistPlayer::stop()
{
    libvlc_media_list_player_stop(_vlcMediaListPlayer);
}

void PlaylistPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    PlaylistPlayer *core = (PlaylistPlayer *)data;

    switch(event->type)
    {
    case libvlc_MediaListPlayerPlayed:
        emit core->played();
        break;
    case libvlc_MediaListPlayerNextItemSet:
        emit core->nextItemSet(event->u.media_list_player_next_item_set.item);
        break;
    case libvlc_MediaListPlayerStopped:
        emit core->stopped();
        break;
    default:
        break;
    }
}

void PlaylistPlayer::createCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerPlayed
        << libvlc_MediaListPlayerNextItemSet
        << libvlc_MediaListPlayerStopped;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(_vlcEvents, event, libvlc_callback, this);
    }
}

void PlaylistPlayer::removeCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerPlayed
        << libvlc_MediaListPlayerNextItemSet
        << libvlc_MediaListPlayerStopped;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_detach(_vlcEvents, event, libvlc_callback, this);
    }
}
