#include "playlistplayer.h"

#include "playlist.h"
#include "media.h"
#include "mediaplayer.h"
#include "global.h"

#include <QTimer>;

PlaylistPlayer::PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _currentIndex(0)
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
    if (index >= _playlist->count())
        return;

    _mediaPlayer->_isPaused = false;
    _currentIndex = index - 1;

    //  apply media settings

    libvlc_media_list_player_play_item_at_index(_vlcMediaListPlayer, index);
}

void PlaylistPlayer::next()
{
    if (_currentIndex == _playlist->count() - 1) {
        playItemAt(0);
        return;
    }

    libvlc_media_list_player_next(_vlcMediaListPlayer);
}

void PlaylistPlayer::previous()
{
    if (_currentIndex <= 0) {
        playItemAt(_playlist->count() - 1);
        return;
    }

    _currentIndex -= 2;

    libvlc_media_list_player_previous(_vlcMediaListPlayer);
}

void PlaylistPlayer::stop()
{
    _mediaPlayer->_isPaused = false;
    _currentIndex = -1;

    libvlc_media_list_player_stop(_vlcMediaListPlayer);
}

void PlaylistPlayer::playPlaylist(Playlist *playlist)
{
    setPlaylist(playlist);

    playItemAt(0);
}

void PlaylistPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    PlaylistPlayer *core = (PlaylistPlayer *)data;
    Playback *currentPlayback;
    switch(event->type)
    {
    case libvlc_MediaListPlayerNextItemSet:
        core->_currentIndex++;

        currentPlayback = core->_playlist->at(core->_currentIndex);

        // TODO : set param on change in mainwindow.

//        connect(currentPlayback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), core->_mediaPlayer, SLOT(setCurrentRatio(Ratio)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(gammaChanged(float)), core->_mediaPlayer, SLOT(setCurrentGamma(float)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(contrastChanged(float)), core->_mediaPlayer, SLOT(setCurrentContrast(float)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(brightnessChanged(float)), core->_mediaPlayer, SLOT(setCurrentBrightness(float)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(saturationChanged(float)), core->_mediaPlayer, SLOT(setCurrentSaturation(float)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(hueChanged(int)), core->_mediaPlayer, SLOT(setCurrentHue(int)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), core->_mediaPlayer, SLOT(setCurrentDeinterlacing(Deinterlacing)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), core->_mediaPlayer, SLOT(setCurrentSubtitlesSync(double)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(audioSyncChanged(double)), core->_mediaPlayer, SLOT(setCurrentAudioSync(double)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(audioTrackChanged(AudioTrack)), core->_mediaPlayer, SLOT(setCurrentAudioTrack(AudioTrack)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(videoTrackChanged(VideoTrack)), core->_mediaPlayer, SLOT(setCurrentVideoTrack(VideoTrack)));
//        connect(currentPlayback->mediaSettings(), SIGNAL(subtitlesTrackChanged(Track)), core->_mediaPlayer, SLOT(setCurrentSubtitlesTrack(Track)));

        emit core->itemChanged(core->_currentIndex);

        QTimer::singleShot(1000, core, SLOT(applyCurrentPlaybackSettings()));

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

void PlaylistPlayer::applyCurrentPlaybackSettings()
{
    _mediaPlayer->applyMediaSettings(_playlist->at(_currentIndex)->mediaSettings());
}
