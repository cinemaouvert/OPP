#include "mediaplayer.h"

#include <QTime>

#include "global.h"
#include "application.h"
#include "media.h"
#include "videoview.h"
#include "mediasettings.h"
#include "playback.h"

MediaPlayer::MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _videoView(NULL),
    _currentPlayback(NULL),
    _isPaused(false)
{
    _vlcMediaPlayer = libvlc_media_player_new(vlcInstance);
    VLC_LAST_ERROR();
    _vlcEvents = libvlc_media_player_event_manager(_vlcMediaPlayer);

//    /* Disable mouse and keyboard events */
    libvlc_video_set_key_input(_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(_vlcMediaPlayer, false);

    createCoreConnections();
}

MediaPlayer::~MediaPlayer()
{
    removeCoreConnections();
    libvlc_media_player_release(_vlcMediaPlayer);
}

int MediaPlayer::currentTime() const
{
    return libvlc_media_player_get_time(_vlcMediaPlayer);
}

int MediaPlayer::currentLength() const
{
    return libvlc_media_player_get_length(_vlcMediaPlayer);
}

bool MediaPlayer::isPlaying() const
{
    return libvlc_media_player_is_playing(_vlcMediaPlayer);
}

void MediaPlayer::setVideoView(VideoView *videoView)
{
    _videoView = videoView;
}

void MediaPlayer::open(Playback *playback)
{
    if (_currentPlayback) {
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), this, SLOT(setCurrentRatio(Ratio)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(gammaChanged(int)), this, SLOT(setCurrentGamma(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(contrastChanged(int)), this, SLOT(setCurrentContrast(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(brightnessChanged(int)), this, SLOT(setCurrentBrightness(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(saturationChanged(int)), this, SLOT(setCurrentSaturation(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(hueChanged(int)), this, SLOT(setCurrentHue(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), this, SLOT(setCurrentDeinterlacing(Deinterlacing)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), this, SLOT(setCurrentSubtitlesSync(double)));
    }

    _currentPlayback = playback;
    VLCERR( libvlc_media_player_set_media(_vlcMediaPlayer, playback->media()->core()) );

    connect(_currentPlayback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), this, SLOT(setCurrentRatio(Ratio)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(gammaChanged(int)), this, SLOT(setCurrentGamma(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(contrastChanged(int)), this, SLOT(setCurrentContrast(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(brightnessChanged(int)), this, SLOT(setCurrentBrightness(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(saturationChanged(int)), this, SLOT(setCurrentSaturation(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(hueChanged(int)), this, SLOT(setCurrentHue(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), this, SLOT(setCurrentDeinterlacing(Deinterlacing)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), this, SLOT(setCurrentSubtitlesSync(double)));
}

void MediaPlayer::play()
{
    if (!_vlcMediaPlayer)
        return;

    if (_videoView) {
        _currentWId = _videoView->request();
    } else {
        _currentWId = 0;
    }

    if (_currentWId) {
#if defined(Q_OS_WIN32)
        libvlc_media_player_set_hwnd(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_MAC)
        libvlc_media_player_set_nsobject(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_UNIX)
        libvlc_media_player_set_xwindow(_vlcMediaPlayer, _currentWId);
#endif
    }

    VLCERR( libvlc_media_player_play(_vlcMediaPlayer) );
    _isPaused = false;
}

void MediaPlayer::pause()
{
    qDebug()<<libvlc_audio_get_channel(_vlcMediaPlayer);
    libvlc_media_player_set_pause(_vlcMediaPlayer, true);
    _isPaused = true;
}

void MediaPlayer::resume()
{
    libvlc_media_player_set_pause(_vlcMediaPlayer, false);
    _isPaused = false;
}

void MediaPlayer::stop()
{
    if (!_vlcMediaPlayer)
        return;

    if (_videoView)
        _videoView->release();
    _currentWId = 0;

    libvlc_media_player_stop(_vlcMediaPlayer);
    _isPaused = false;
}

void MediaPlayer::setCurrentTime(int time)
{
    libvlc_media_player_set_time(_vlcMediaPlayer, time);
}

void MediaPlayer::setVolume(int volume)
{
    libvlc_audio_set_volume(_vlcMediaPlayer, volume);
}

float MediaPlayer::position() const
{
    return libvlc_media_player_get_position(_vlcMediaPlayer);
}

void MediaPlayer::setPosition(const float &position)
{
    libvlc_media_player_set_position(_vlcMediaPlayer, position);
}

void MediaPlayer::setPosition(const int &position)
{
    setPosition( ((float) position) / 100.f);
}

void MediaPlayer::setCurrentRatio(Ratio ratio)
{
    libvlc_video_set_aspect_ratio(_vlcMediaPlayer, MediaSettings::ratioValues()[ratio].toUtf8().data());
}

void MediaPlayer::setCurrentDeinterlacing(Deinterlacing deinterlacing)
{
    libvlc_video_set_deinterlace(_vlcMediaPlayer, MediaSettings::deinterlacingValues()[deinterlacing].toUtf8().data());
}

void MediaPlayer::setCurrentSubtitlesSync(double sync)
{
    libvlc_video_set_spu_delay(_vlcMediaPlayer,(int64_t)(1000000*sync));
}

void MediaPlayer::setCurrentGamma(int gamma)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Gamma, (float)gamma/50);
}

void MediaPlayer::setCurrentContrast(int contrast)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Contrast, (float)contrast/50);
}

void MediaPlayer::setCurrentBrightness(int brightness)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Brightness, (float)brightness/50);
}

void MediaPlayer::setCurrentSaturation(int saturation)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Saturation, (float)saturation/50);
}

void MediaPlayer::setCurrentHue(int hue)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Hue, (float)hue/100);

}

void MediaPlayer::createCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaPlayerMediaChanged
         << libvlc_MediaPlayerNothingSpecial
         << libvlc_MediaPlayerOpening
         << libvlc_MediaPlayerBuffering
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerForward
         << libvlc_MediaPlayerBackward
         << libvlc_MediaPlayerEndReached
         << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerTimeChanged
         << libvlc_MediaPlayerPositionChanged
         << libvlc_MediaPlayerSeekableChanged
         << libvlc_MediaPlayerPausableChanged
         << libvlc_MediaPlayerTitleChanged
         << libvlc_MediaPlayerSnapshotTaken
         << libvlc_MediaPlayerLengthChanged
         << libvlc_MediaPlayerVout;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(_vlcEvents, event, libvlc_callback, this);
    }
}

void MediaPlayer::removeCoreConnections()
{
    QList<libvlc_event_e> list;
    list << libvlc_MediaPlayerMediaChanged
         << libvlc_MediaPlayerNothingSpecial
         << libvlc_MediaPlayerOpening
         << libvlc_MediaPlayerBuffering
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerForward
         << libvlc_MediaPlayerBackward
         << libvlc_MediaPlayerEndReached
         << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerTimeChanged
         << libvlc_MediaPlayerPositionChanged
         << libvlc_MediaPlayerSeekableChanged
         << libvlc_MediaPlayerPausableChanged
         << libvlc_MediaPlayerTitleChanged
         << libvlc_MediaPlayerSnapshotTaken
         << libvlc_MediaPlayerLengthChanged
         << libvlc_MediaPlayerVout;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_detach(_vlcEvents, event, libvlc_callback, this);
    }
}

void MediaPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    MediaPlayer *player = (MediaPlayer *)data;

    switch(event->type)
    {
    case libvlc_MediaPlayerMediaChanged:
        emit player->mediaChanged(event->u.media_player_media_changed.new_media);
        break;
    case libvlc_MediaPlayerNothingSpecial:
        emit player->nothingSpecial();
        break;
    case libvlc_MediaPlayerOpening:
        emit player->opening();
        break;
    case libvlc_MediaPlayerBuffering:
        emit player->buffering(event->u.media_player_buffering.new_cache);
    case libvlc_MediaPlayerPlaying:
        emit player->playing();
        break;
    case libvlc_MediaPlayerPaused:
        emit player->paused();
        break;
    case libvlc_MediaPlayerStopped:
        emit player->stopped();
        break;
    case libvlc_MediaPlayerForward:
        emit player->forward();
        break;
    case libvlc_MediaPlayerBackward:
        emit player->backward();
        break;
    case libvlc_MediaPlayerEndReached:
        emit player->end();
        break;
    case libvlc_MediaPlayerEncounteredError:
        emit player->error();
        break;
    case libvlc_MediaPlayerTimeChanged:
        emit player->timeChanged(event->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerPositionChanged:
        emit player->positionChanged(event->u.media_player_position_changed.new_position);
        break;
    case libvlc_MediaPlayerSeekableChanged:
        emit player->seekableChanged(event->u.media_player_seekable_changed.new_seekable);
        break;
    case libvlc_MediaPlayerPausableChanged:
        emit player->pausableChanged(event->u.media_player_pausable_changed.new_pausable);
        break;
    case libvlc_MediaPlayerTitleChanged:
        emit player->titleChanged(event->u.media_player_title_changed.new_title);
        break;
    case libvlc_MediaPlayerSnapshotTaken:
        emit player->snapshotTaken(event->u.media_player_snapshot_taken.psz_filename);
        break;
    case libvlc_MediaPlayerLengthChanged:
        emit player->lengthChanged(event->u.media_player_length_changed.new_length);
        break;
    case libvlc_MediaPlayerVout:
        emit player->vout(event->u.media_player_vout.new_count);
        break;
    default:
        break;
    }

    if (event->type >= libvlc_MediaPlayerNothingSpecial &&
        event->type <= libvlc_MediaPlayerEncounteredError) {
        emit player->stateChanged();
    }
}

