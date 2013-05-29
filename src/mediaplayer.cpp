#include "mediaplayer.h"

#include "global.h"
#include "application.h"
#include "media.h"
#include "videoview.h"

MediaPlayer::MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _videoView(NULL),
    _isPaused(false)
{
    _vlcMediaPlayer = libvlc_media_player_new(vlcInstance);
    VLC_LAST_ERROR();
//    _vlcEvents = libvlc_media_player_event_manager(_vlcMediaPlayer);

//    /* Disable mouse and keyboard events */
    libvlc_video_set_key_input(_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(_vlcMediaPlayer, false);

//    createCoreConnections();
}

MediaPlayer::~MediaPlayer()
{
    libvlc_media_player_release(_vlcMediaPlayer);
}

int MediaPlayer::length() const
{
    libvlc_time_t length = libvlc_media_player_get_length(_vlcMediaPlayer);
    return length;
}

void MediaPlayer::setVideoView(VideoView *videoView)
{
    _videoView = videoView;
}

void MediaPlayer::open(const Media &media)
{
    VLCERR( libvlc_media_player_set_media(_vlcMediaPlayer, media.core()) );
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

bool MediaPlayer::isPlaying() const
{
    return libvlc_media_player_is_playing(_vlcMediaPlayer);
}
