#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtGui/qwindowdefs.h>

#include "mediasettings.h"

class Playback;
class VideoView;
class MediaSettings;

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;
struct libvlc_media_t;

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    virtual ~MediaPlayer();

    inline Playback *currentPlayback() const { return _currentPlayback; }

    inline bool isPaused() const { return _isPaused; }

    bool isPlaying() const;

    int currentTime() const;

    int currentLength() const;

    float position() const;

    void setVideoView(VideoView *videoView);

    void open(Playback *playback);

public slots:
    void play();

    void pause();

    void resume();

    void stop();

    void setCurrentTime(int time);

    void setVolume(int volume);

    /**
     * @brief setPosition
     * @param position Value between .0 and .99
     */
    void setPosition(const float &position);

    /**
     * @brief setPosition
     * @param position Value between 0 and 99.
     */
    void setPosition(const int &position);

    void setCurrentRatio(Ratio ratio);

    void setCurrentDeinterlacing(Deinterlacing deinterlacing);


signals:
    void backward();
    void buffering(const float &);
    void end();
    void error();
    void forward();
    void lengthChanged(const int &);
    void mediaChanged(libvlc_media_t *);
    void nothingSpecial();
    void opening();
    void pausableChanged(const bool &);
    void paused();
    void playing();
    void positionChanged(const float &);
    void seekableChanged(const bool &);
    void snapshotTaken(const QString &);
    void stopped();
    void timeChanged(const int &);
    void titleChanged(const int &);
    void vout(const int &);
    void stateChanged();

private:
    void createCoreConnections();
    void removeCoreConnections();

    static void libvlc_callback(const libvlc_event_t *event, void *data);

    libvlc_media_player_t *_vlcMediaPlayer;
    libvlc_event_manager_t *_vlcEvents;

    Playback *_currentPlayback;
    VideoView *_videoView;
    WId _currentWId;

    bool _isPaused;
};

#endif // MEDIAPLAYER_H
