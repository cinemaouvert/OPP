#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtGui/qwindowdefs.h>

class Media;
class VideoView;

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_manager_t;

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    virtual ~MediaPlayer();

    inline bool isPaused() const { return _isPaused; }

    int length() const;

    void setVideoView(VideoView *videoView);

    void open(const Media &media);

    void play();

    void pause();

    void resume();

    void stop();

    bool isPlaying() const;

private:
    libvlc_media_player_t *_vlcMediaPlayer;
    libvlc_event_manager_t *_vlcEvents;

    VideoView *_videoView;
    WId _currentWId;

    bool _isPaused;
};

#endif // MEDIAPLAYER_H
