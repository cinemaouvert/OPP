#include "mediaplayer.h"

#include <math.h>

#include <QTime>
#include <QTimer>

#include "global.h"
#include "application.h"
#include "media.h"
#include "videoview.h"
#include "mediasettings.h"
#include "playback.h"

MediaPlayer::MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _currentPlayback(NULL),
    _videoView(NULL),
    _isPaused(false),
    _currentVolume(50),
    _currentGain(0)
{
    _vlcMediaPlayer = libvlc_media_player_new(vlcInstance);
    VLC_LAST_ERROR();
    _vlcEvents = libvlc_media_player_event_manager(_vlcMediaPlayer);

    libvlc_video_set_key_input(_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(_vlcMediaPlayer, false);

    connect(this, SIGNAL(vout(int)), this, SLOT(applyCurrentPlaybackSettings()));

    createCoreConnections();
}

MediaPlayer::~MediaPlayer()
{
    disconnect(this, SIGNAL(vout(int)), this, SLOT(applyCurrentPlaybackSettings()));

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

    _currentWId = _videoView->request();

    if (_currentWId) {
#if defined(Q_OS_WIN)
        libvlc_media_player_set_hwnd(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_MAC)
        libvlc_media_player_set_nsobject(_vlcMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_UNIX)
        libvlc_media_player_set_xwindow(_vlcMediaPlayer, _currentWId);
#endif
    }
}

void MediaPlayer::open(Playback *playback)
{
    if (_currentPlayback) {
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(gainChanged(float)), this, SLOT(setCurrentGain(float)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), this, SLOT(setCurrentRatio(Ratio)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(gammaChanged(float)), this, SLOT(setCurrentGamma(float)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(contrastChanged(float)), this, SLOT(setCurrentContrast(float)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(brightnessChanged(float)), this, SLOT(setCurrentBrightness(float)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(saturationChanged(float)), this, SLOT(setCurrentSaturation(float)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(hueChanged(int)), this, SLOT(setCurrentHue(int)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), this, SLOT(setCurrentDeinterlacing(Deinterlacing)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), this, SLOT(setCurrentSubtitlesSync(double)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(audioSyncChanged(double)), this, SLOT(setCurrentAudioSync(double)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(audioTrackChanged(AudioTrack)), this, SLOT(setCurrentAudioTrack(AudioTrack)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(videoTrackChanged(VideoTrack)), this, SLOT(setCurrentVideoTrack(VideoTrack)));
        disconnect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesTrackChanged(Track)), this, SLOT(setCurrentSubtitlesTrack(Track)));
    }

    _currentPlayback = playback;
    VLCERR( libvlc_media_player_set_media(_vlcMediaPlayer, playback->media()->core()) );

    connect(_currentPlayback->mediaSettings(), SIGNAL(gainChanged(float)), this, SLOT(setCurrentGain(float)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), this, SLOT(setCurrentRatio(Ratio)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(gammaChanged(float)), this, SLOT(setCurrentGamma(float)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(contrastChanged(float)), this, SLOT(setCurrentContrast(float)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(brightnessChanged(float)), this, SLOT(setCurrentBrightness(float)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(saturationChanged(float)), this, SLOT(setCurrentSaturation(float)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(hueChanged(int)), this, SLOT(setCurrentHue(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), this, SLOT(setCurrentDeinterlacing(Deinterlacing)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), this, SLOT(setCurrentSubtitlesSync(double)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(audioSyncChanged(double)), this, SLOT(setCurrentAudioSync(double)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(audioTrackChanged(AudioTrack)), this, SLOT(setCurrentAudioTrack(AudioTrack)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(videoTrackChanged(VideoTrack)), this, SLOT(setCurrentVideoTrack(VideoTrack)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesTrackChanged(Track)), this, SLOT(setCurrentSubtitlesTrack(Track)));
}

void MediaPlayer::play()
{
    VLCERR( libvlc_media_player_play(_vlcMediaPlayer) );
    _isPaused = false;
}

void MediaPlayer::pause()
{
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
    _currentVolume = volume > 100 ? 100 : volume;

    libvlc_audio_set_volume(_vlcMediaPlayer, ((float) _currentVolume) * powf(10.f, _currentGain/10.f) );
}

float MediaPlayer::position() const
{
    return libvlc_media_player_get_position(_vlcMediaPlayer);
}

int MediaPlayer::volume() const {
    return libvlc_audio_get_volume(_vlcMediaPlayer);
}

void MediaPlayer::setPosition(const float &position)
{
    libvlc_media_player_set_position(_vlcMediaPlayer, position);
}

void MediaPlayer::setPosition(const int &position)
{
    setPosition( ((float) position) / 100.f);
}

void MediaPlayer::applyCurrentPlaybackSettings()
{
    setCurrentGain(_currentPlayback->mediaSettings()->gain());

    setCurrentSubtitlesTrack(_currentPlayback->mediaSettings()->subtitlesTrack());
    setCurrentVideoTrack(_currentPlayback->mediaSettings()->videoTrack());
    setCurrentAudioTrack(_currentPlayback->mediaSettings()->audioTrack());

    setCurrentBrightness(_currentPlayback->mediaSettings()->brightness());
    setCurrentContrast(_currentPlayback->mediaSettings()->contrast());
    setCurrentDeinterlacing(_currentPlayback->mediaSettings()->deinterlacing());
    setCurrentGamma(_currentPlayback->mediaSettings()->gamma());
    setCurrentHue(_currentPlayback->mediaSettings()->hue());
    setCurrentRatio(_currentPlayback->mediaSettings()->ratio());
    setCurrentSaturation(_currentPlayback->mediaSettings()->saturation());
    setCurrentSubtitlesSync(_currentPlayback->mediaSettings()->subtitlesSync());
}

void MediaPlayer::setCurrentGain(float gain)
{
    _currentGain = gain;
    setVolume(_currentVolume);
}

void MediaPlayer::setCurrentAudioTrack(const AudioTrack &track)
{
    const int index = _currentPlayback->media()->audioTracks().indexOf(track);
    libvlc_audio_set_track(_vlcMediaPlayer, index == -1 ? 0 : index + 1);
}

void MediaPlayer::setCurrentVideoTrack(const VideoTrack &track)
{
    libvlc_video_set_track(_vlcMediaPlayer, _currentPlayback->media()->videoTracks().indexOf(track));
}

void MediaPlayer::setCurrentSubtitlesTrack(const Track &track)
{
    const QList<Track> &tracks = _currentPlayback->media()->subtitlesTracks();

    if (tracks.count() == 0)
        return;

    const int index = tracks.indexOf(track);

    libvlc_video_set_spu(_vlcMediaPlayer, index == -1 ? 0 : index + 1);
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

void MediaPlayer::setCurrentGamma(float gamma)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Gamma, gamma);
}

void MediaPlayer::setCurrentContrast(float contrast)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Contrast, contrast);
}

void MediaPlayer::setCurrentBrightness(float brightness)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Brightness, brightness);
}

void MediaPlayer::setCurrentSaturation(float saturation)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_float(_vlcMediaPlayer,libvlc_adjust_Saturation, saturation);
}

void MediaPlayer::setCurrentHue(int hue)
{
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Enable,1);
    libvlc_video_set_adjust_int(_vlcMediaPlayer,libvlc_adjust_Hue, hue);
}

void MediaPlayer::setCurrentAudioSync(double sync)
{
    libvlc_audio_set_delay(_vlcMediaPlayer,(int64_t)(1000000*sync));
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
        break;
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
        qDebug() << "<<<<<<<<<<<<<< vout >>>>>>>>>>>>>";
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

