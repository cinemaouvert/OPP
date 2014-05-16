/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "mediaplayer.h"

#include <math.h>
#include <string.h>
#include <sstream>

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QTime>
#include <QTimer>

#include <vlc/vlc.h>

#include "application.h"
#include "media.h"
#include "videoview.h"
#include "mainwindow.h"
#include "playlistplayer.h"
#include "mediasettings.h"
#include "playback.h"

MediaPlayer::MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _inst(vlcInstance),
    _vlcMediaPlayer(NULL),
    _vlcBackMediaPlayer(NULL),
    _vlcEvents(NULL),
    _currentPlayback(NULL),
    _videoView(NULL),
    _videoBackView(NULL),
    _currentVolume(50),
    _currentGain(0),
    _isPaused(false)
{
    QSettings settings("opp","opp");
    if(settings.value("VideoReturnMode").toString() == "none")
    {
        _bMode = NONE;
    }
    else  if(settings.value("VideoReturnMode").toString() == "pictures")
    {
        _bMode = SCREENSHOT;
    }
    else
    {
        _bMode = STREAMING;
    }
    if(settings.value("locateR").toBool())
    {
        std::stringstream ss;
            ss << QApplication::desktop()->screenGeometry().width();
        _sizeScreen ="screen-left="+ ss.str();
    }
    else
    {
        std::stringstream ss;
            ss << (QApplication::desktop()->screen()->width()-QApplication::desktop()->screenGeometry().width());
        _sizeScreen ="screen-width="+ ss.str();
    }
    _vlcBackMediaPlayer = libvlc_media_player_new(_inst);
    _vlcMediaPlayer = libvlc_media_player_new(_inst);
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
    libvlc_media_player_release(_vlcBackMediaPlayer);
    libvlc_vlm_release(_inst);
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

void MediaPlayer::setBackMode(const BackMode &mode)
{
    _bMode = mode;
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

void MediaPlayer::setVideoBackView(VideoView *videoView)
{
    _videoBackView = videoView;

    _currentWId = _videoBackView->request();

    if (_currentWId) {
#if defined(Q_OS_WIN)
        libvlc_media_player_set_hwnd(_vlcBackMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_MAC)
        libvlc_media_player_set_nsobject(_vlcBackMediaPlayer, (void *)_currentWId);
#elif defined(Q_OS_UNIX)
        libvlc_media_player_set_xwindow(_vlcBackMediaPlayer, _currentWId);
#endif
    }
}

void MediaPlayer::close(Playback *playback){
    if(playback){
        disconnect(playback->mediaSettings(), SIGNAL(gainChanged(float)), this, SLOT(setCurrentGain(float)));
        disconnect(playback->mediaSettings(), SIGNAL(ratioChanged(Ratio)), this, SLOT(setCurrentRatio(Ratio)));
        disconnect(playback->mediaSettings(), SIGNAL(gammaChanged(float)), this, SLOT(setCurrentGamma(float)));
        disconnect(playback->mediaSettings(), SIGNAL(contrastChanged(float)), this, SLOT(setCurrentContrast(float)));
        disconnect(playback->mediaSettings(), SIGNAL(brightnessChanged(float)), this, SLOT(setCurrentBrightness(float)));
        disconnect(playback->mediaSettings(), SIGNAL(saturationChanged(float)), this, SLOT(setCurrentSaturation(float)));
        disconnect(playback->mediaSettings(), SIGNAL(hueChanged(int)), this, SLOT(setCurrentHue(int)));
        disconnect(playback->mediaSettings(), SIGNAL(deinterlacingChanged(Deinterlacing)), this, SLOT(setCurrentDeinterlacing(Deinterlacing)));
        disconnect(playback->mediaSettings(), SIGNAL(subtitlesSyncChanged(double)), this, SLOT(setCurrentSubtitlesSync(double)));
        disconnect(playback->mediaSettings(), SIGNAL(audioSyncChanged(double)), this, SLOT(setCurrentAudioSync(double)));
        disconnect(playback->mediaSettings(), SIGNAL(audioTrackChanged(int)), this, SLOT(setCurrentAudioTrack(int)));
        disconnect(playback->mediaSettings(), SIGNAL(videoTrackChanged(int)), this, SLOT(setCurrentVideoTrack(int)));
        disconnect(playback->mediaSettings(), SIGNAL(subtitlesTrackChanged(int)), this, SLOT(setCurrentSubtitlesTrack(int)));
    }
}

void MediaPlayer::open(Playback *playback)
{
    close(playback);

    _currentPlayback = playback;

    libvlc_media_player_set_media(_vlcMediaPlayer, playback->media()->core());
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
    connect(_currentPlayback->mediaSettings(), SIGNAL(audioTrackChanged(int)), this, SLOT(setCurrentAudioTrack(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(videoTrackChanged(int)), this, SLOT(setCurrentVideoTrack(int)));
    connect(_currentPlayback->mediaSettings(), SIGNAL(subtitlesTrackChanged(int)), this, SLOT(setCurrentSubtitlesTrack(int)));

    if(_currentPlayback->media()->isImage()){
        libvlc_media_player_set_time(_vlcMediaPlayer, 0);
        qDebug() << "time image";
    }
}

void MediaPlayer::initStream()
{
    const char* params[] = {"screen-fragment-size=16",
            _sizeScreen.c_str(),
            "screen-fps=25"};

    libvlc_vlm_add_broadcast(_inst, "mybroad",
        "screen:// --sout",
        "#transcode{vcodec=mp2v,acodec=none,ab=128}:standard{access=http,mux=ts,dst=127.0.0.1:8080/stream}",
        3, params, 1 , 0);

    Media *m = new Media("http://127.0.0.1:8080/stream", _inst,0,false);
    libvlc_media_player_set_media(_vlcBackMediaPlayer, m->core());
}

void MediaPlayer::play()
{
    switch (_bMode)
    {
    case SCREENSHOT:
        playScreen();
        break;
    case STREAMING:
        playStream();
        break;
    default:
        break;
    }
    libvlc_media_player_play(_vlcMediaPlayer);
    _isPaused = false;
}

void MediaPlayer::playStream()
{
    libvlc_vlm_play_media(_inst, "mybroad");
    libvlc_media_player_play(_vlcBackMediaPlayer);
}

void MediaPlayer::playScreen()
{
    _timer = new QTimer();
    _timer->connect(_timer, SIGNAL(timeout()), this, SLOT(takeScreen()));
    _timer->start(40);
}

void MediaPlayer::takeScreen()
{
    if(isPlaying())
    {
        libvlc_video_take_snapshot(_vlcMediaPlayer, 0, "tmp.png", libvlc_video_get_width(_vlcMediaPlayer), libvlc_video_get_height(_vlcMediaPlayer));
        ((MainWindow *)((PlaylistPlayer *)this->parent())->parent())->setScreenshot("tmp.png");
    }
}

void MediaPlayer::stopScreen()
{
    _timer->stop();
}

void MediaPlayer::pause()
{
    switch (_bMode)
    {
    case SCREENSHOT:
        stopScreen();
        break;
    case STREAMING:
        stopStream();
        break;
    default:
        break;
    }
    libvlc_media_player_set_pause(_vlcMediaPlayer, true);
    _isPaused = true;
}

void MediaPlayer::resume()
{
    switch (_bMode)
    {
    case SCREENSHOT:
        playScreen();
        break;
    case STREAMING:
        playStream();
        break;
    default:
        break;
    }
    libvlc_media_player_set_pause(_vlcMediaPlayer, false);
    _isPaused = false;
}

void MediaPlayer::stop()
{
    if (!_vlcMediaPlayer)
        return;

    /*if (_videoView)
        _videoView->release();*/
    _currentWId = 0;

    switch (_bMode)
    {
    case SCREENSHOT:
        stopScreen();
        ((MainWindow *)((PlaylistPlayer *)this->parent())->parent())->setScreenshot(":/icons/resources/images/intertitle.jpg");
        break;
    case STREAMING:
        stopStream();
        break;
    default:
        break;
    }
    libvlc_media_player_stop(_vlcMediaPlayer);
    _isPaused = false;
}

void MediaPlayer::stopStream()
{
    libvlc_media_player_stop(_vlcBackMediaPlayer);
    libvlc_vlm_stop_media(_inst, "mybroad");
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

void MediaPlayer::setCurrentAudioTrack(const int &track)
{
    // call libvlc_video_set_audio() when media does not contains any audio track cause trouble
    if (_currentPlayback->media()->audioTracks().size() > 0)
        libvlc_audio_set_track(_vlcMediaPlayer, track);
}

void MediaPlayer::setCurrentVideoTrack(const int &track)
{
        libvlc_video_set_track(_vlcMediaPlayer, track);
}

void MediaPlayer::setCurrentSubtitlesTrack(const int &track)
{
    // call libvlc_video_set_spu() when media does not contains any subtitles track cause trouble
    if (_currentPlayback->media()->subtitlesTracks().size() > 0)
        libvlc_video_set_spu(_vlcMediaPlayer, track);
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
        //TODO Write to the log file
        qDebug() <<  "<<<<<<<<<<<<<<vout >>>>>>>>>>>>>";
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

