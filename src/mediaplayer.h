/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtGui/qwindowdefs.h>

#include "mediasettings.h"
#include "playlistplayer.h"

class Playback;
class VideoView;
class MediaSettings;
class Media;
class Track;
class VideoTrack;
class AudioTrack;

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

    /**
     * @brief currentPlayback
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Playback *currentPlayback() const { return _currentPlayback; }

    /**
     * @brief core
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_media_player_t *core() const { return _vlcMediaPlayer; }

    /**
     * @brief isPaused
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline bool isPaused() const { return _isPaused; }

    /**
     * @brief isPlaying
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isPlaying() const;

    /**
     * @brief currentTime
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int currentTime() const;

    /**
     * @brief currentLength
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int currentLength() const;

    /**
     * @brief position
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    float position() const;

    /**
     * @brief get current software volume
     * @return
     */
    int volume() const;

    /**
     * @brief setVideoView
     * @param videoView
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setVideoView(VideoView *videoView);

    /**
     * @brief open
     * @param playback
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void open(Playback *playback);

public slots:
    /**
     * @brief play
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void play();

    /**
     * @brief pause
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void pause();

    /**
     * @brief resume
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void resume();

    /**
     * @brief stop
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stop();

    /**
     * @brief setCurrentTime
     * @param time
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentTime(int time);

    /**
     * @brief setVolume
     * @param volume
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setVolume(int volume);

    /**
     * @brief setCurrentGain
     * @param gain
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentGain(float gain);

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

    /**
     * @brief setCurrentRatio
     * @param ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentRatio(Ratio ratio);

    /**
     * @brief setCurrentDeinterlacing
     * @param deinterlacing
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentDeinterlacing(Deinterlacing deinterlacing);

    /**
     * @brief setCurrentSubtitlesSync
     * @param sync
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentSubtitlesSync(double sync);

    /**
     * @brief setCurrentAudioTrack
     * @param track Set 0 to disable audio track, 1 to use the first track...
     */
    void setCurrentAudioTrack(const int &track);

    /**
     * @brief setCurrentVideoTrack
     * @param track Set -1 to disable video track, 0 to use the first track...
     */
    void setCurrentVideoTrack(const int &track);

    /**
     * @brief setCurrentSubtitlesTrack
     * @param track Set 0 to disable subtitles track, 1 to use the first track...
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentSubtitlesTrack(const int &track);

    /**
     * @brief setCurrentGamma
     * @param gamma
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentGamma(float gamma);

    /**
     * @brief setCurrentContrast
     * @param contrast
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentContrast(float contrast);

    /**
     * @brief setCurrentBrightness
     * @param brightness
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentBrightness(float brightness);

    /**
     * @brief setCurrentSaturation
     * @param saturation
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentSaturation(float saturation);

    /**
     * @brief setCurrentHue
     * @param hue
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentHue(int hue);

    /**
     * @brief setCurrentAudioSync
     * @param sync
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentAudioSync(double sync);

private slots:

    /**
     * @brief applyCurrentPlaybackSettings
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void applyCurrentPlaybackSettings();

signals:
    /**
     * @brief backward
     */
    void backward();

    /**
     * @brief buffering
     */
    void buffering(const float &);

    /**
     * @brief end
     */
    void end();

    /**
     * @brief error
     */
    void error();

    /**
     * @brief forward
     */
    void forward();

    /**
     * @brief lengthChanged
     */
    void lengthChanged(const int &);

    /**
     * @brief mediaChanged
     */
    void mediaChanged(libvlc_media_t *);

    /**
     * @brief nothingSpecial
     */
    void nothingSpecial();

    /**
     * @brief opening
     */
    void opening();

    /**
     * @brief pausableChanged
     */
    void pausableChanged(const bool &);

    /**
     * @brief paused
     */
    void paused();

    /**
     * @brief playing
     */
    void playing(bool = true);

    /**
     * @brief positionChanged
     */
    void positionChanged(const float &);

    /**
     * @brief seekableChanged
     */
    void seekableChanged(const bool &);

    /**
     * @brief snapshotTaken
     */
    void snapshotTaken(const QString &);

    /**
     * @brief stopped
     */
    void stopped();

    /**
     * @brief timeChanged
     */
    void timeChanged(const int &);

    /**
     * @brief titleChanged
     */
    void titleChanged(const int &);

    /**
     * @brief vout
     */
    void vout(const int &);

    /**
     * @brief stateChanged
     */
    void stateChanged();

private:
    /**
     * @brief createCoreConnections
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void createCoreConnections();

    /**
     * @brief removeCoreConnections
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeCoreConnections();

    /**
     * @brief libvlc_callback
     * @param event
     * @param data
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    /**
     * @brief _vlcMediaPlayer
     */
    libvlc_media_player_t *_vlcMediaPlayer;

    /**
     * @brief _vlcEvents
     */
    libvlc_event_manager_t *_vlcEvents;

    /**
     * @brief _currentPlayback
     */
    Playback *_currentPlayback;

    /**
     * @brief _videoView
     */
    VideoView *_videoView;

    /**
     * @brief _currentWId
     */
    WId _currentWId;

    /**
     * @brief _currentVolume
     */
    int _currentVolume;

    /**
     * @brief _currentGain
     */
    int _currentGain;

    /**
     * @brief _isPaused
     */
    bool _isPaused;
};

#endif // MEDIAPLAYER_H
