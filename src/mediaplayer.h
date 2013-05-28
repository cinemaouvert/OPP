#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtGui/qwindowdefs.h>

class Media;
class VideoView;

struct libvlc_media_player_t;
struct libvlc_event_manager_t;

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = 0);
    virtual ~MediaPlayer();

    inline Media* currentMedia() const { return _currentMedia; }

    int length() const;

    void setVideoView(VideoView *videoView);

    void open(Media *media);

    void play();

private:
    Media *_currentMedia;

    libvlc_media_player_t *_vlcMediaPlayer;
    libvlc_event_manager_t *_vlcEvents;

    VideoView *_videoView;
    WId _currentWId;
};

#endif // MEDIAPLAYER_H
