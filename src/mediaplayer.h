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

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtGui/qwindowdefs.h>

#include <string.h>

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

/**
 * @brief Manage media playback (through Playback)
 */
class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Contains projection return mode values
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    enum BackMode {
        /**
         * Projection return with screenshots
         */
        SCREENSHOT = 0,

        /**
         * Projection return with streaming
         */
        STREAMING = 1,

        /**
         * No projection return
         */
        NONE = 2
    };

    explicit MediaPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    virtual ~MediaPlayer();

    /**
     * @brief Get current played Playback instance
     * @return The Playback instance
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Playback *currentPlayback() const { return _currentPlayback; }

    /**
     * @brief Get libvlc media player core
     * @return The libvlc media player core
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_media_player_t *core() const { return _vlcMediaPlayer; }

    /**
     * @brief Media player is paused
     * @return True if media player is paused, false otherwise.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline bool isPaused() const { return _isPaused; }

    /**
     * @brief Media player is playing
     * @return True if media player is playing, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isPlaying() const;

    /**
     * @brief Get BackMode
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    inline BackMode bMode() const{ return _bMode; }

    /**
     * @brief Setter for BackMode
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void setBackMode(const BackMode &mode);

    /**
     * @brief Get current time
     * @return The current time in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int currentTime() const;

    /**
     * @brief Get current length_vlcMediaPlayer
     * @return The current length in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int currentLength() const;

    /**
     * @brief Get the current movie length
     * @return The current movie length in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    float position() const;

    /**
     * @brief Get current software volume
     * @return The current sofware volume between 0 (min) and 200 (max).
     *         The value 100 corresponds to the nominal volume (0dB).
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int volume() const;

    /**
     * @brief Set video view in which the media player will render the video
     * @param The video view
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setVideoView(VideoView *videoView);

    /**
     * @brief Set video view in which the media player will render the sample of the video
     * @param The video view
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void setVideoBackView(VideoView *videoView);

    /**
     * @brief Open a playback in media player. It will not play the media
     * @param playback The playback to open.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void open(Playback *playback);

    /**
     * @brief Play the stream and media player associed
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void playStream();

    /**
     * @brief Stop the stream and media player associed
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void stopStream();

    /**
     * @brief Stream initialisation. Add a broadcast to an instance of VLC
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void initStream();

    /**
     * @brief Close a playback in media player.
     * @param playback The playback to close.
     *
     * @author Thibaud Lamarche <thibaud.lamarche@gmail.com>
     */
    void close(Playback *playback);

public slots:
    /**
     * @brief Play the media player with the opened Playback
     * @see open()
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void play();

    /**
     * @brief Pause media player
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void pause();

    /**
     * @brief Resume playback to the current movie position after the media player was paused.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void resume();

    /**
     * @brief Stop the media player.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stop();

    /**
     * @brief Set current time
     * @param time The new current time in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentTime(int time);

    /**
     * @brief Set volume
     * @param volume The new volume between 0 (mute) and 100 (0dB).
     *
     * @see setCurrentGain()
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setVolume(int volume);

    /**
     * @brief Set current audio gain.
     * @param gain The new gain between 0.0(dB) and 3.01(dB)
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentGain(float gain);

    /**
     * @brief Set media player position
     * @param position Value between .0 and .99
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setPosition(const float &position);

    /**
     * @brief Set current video ratio
     * @param ratio The new ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentRatio(Ratio ratio);

    /**
     * @brief Set current deinterlacing
     * @param deinterlacing The new deinterlacing mode
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentDeinterlacing(Deinterlacing deinterlacing);

    /**
     * @brief set current subtitles synchronization
     * @param sync The new synchronization between 0.0 to 3.0
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentSubtitlesSync(double sync);

    /**
     * @brief Set current audio track
     * @param track Set 0 to disable audio track, 1 to use the first track...
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentAudioTrack(const int &track);

    /**
     * @brief Set current video track
     * @param track Set -1 to disable video track, 0 to use the first track...
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentVideoTrack(const int &track);

    /**
     * @brief Set current subtitles track
     * @param track Set 0 to disable subtitles track, 1 to use the first track...
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setCurrentSubtitlesTrack(const int &track);

    /**
     * @brief Set current gamma
     * @param gamma The new gamma value between 0.0 to 10.0
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentGamma(float gamma);

    /**
     * @brief Set current contrast
     * @param contrast The new contrast value between 0.0 to 2.0
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentContrast(float contrast);

    /**
     * @brief Set current brightness
     * @param brightness The new brightness value between 0.0 to 2.0
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentBrightness(float brightness);

    /**
     * @brief set current saturation
     * @param saturation The new saturation value between 0.0 to 3.0
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentSaturation(float saturation);

    /**
     * @brief Set current hue
     * @param hue The new hue value between 0 to 20
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentHue(int hue);

    /**
     * @brief set current audio synchronization
     * @param sync The new audio synchronization
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
	 * @author Cyril Naud <futuramath@gmail.com>
     */
    void setCurrentAudioSync(double sync);

private slots:

    /**
     * @brief Apply current playback settings to media player. Automatically called when play()
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void applyCurrentPlaybackSettings();

    /**
     * @brief Taking screenshot.
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void takeScreen();

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
     * @brief Create libvlc event connections
     * @see libvlc_callback()
     * @see removeCoreConnections()
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void createCoreConnections();

    /**
     * @brief Remove libvlc event connections
     * @see libvlc_callback()
     * @see createCoreConnections()
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeCoreConnections();

    /**
     * @brief Libvlc callback. Called every time vlc send an event
     * @param event The libvlc event
     * @param data The media player passed through libvlc_event_manager_t
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    /**
     * @brief Taking periodically screenshots.
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void playScreen();

    /**
     * @brief Stop taking screenshots
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void stopScreen();

    /**
     * @brief The libvlc instance for stream
     */
    libvlc_instance_t * _inst;

    /**
     * @brief The libvlc media player core
     */
    libvlc_media_player_t *_vlcMediaPlayer;

    /**
     * @brief The libvlc media player core for the sample of the video
     */
    libvlc_media_player_t *_vlcBackMediaPlayer;

    /**
     * @brief The libvlc event manager core
     */
    libvlc_event_manager_t *_vlcEvents;

    /**
     * @brief The current playback
     */
    Playback *_currentPlayback;

    /**
     * @brief The video view which the video will be rendered into
     */
    VideoView *_videoView;

    /**
     * @brief The video view which the sample of the video will be rendered into
     */
    VideoView *_videoBackView;

    /**
     * @brief The current window id
     */
    WId _currentWId;

    /**
     * @brief The current volume
     */
    int _currentVolume;

    /**
     * @brief The current audio gain
     */
    int _currentGain;

    /**
     * @brief The current media player pause status. Set to true if paused, false otherwise.
     */
    bool _isPaused;

    /**
     * @brief The mode of projection return
     */
    BackMode _bMode;

    /**
     * @brief Size needed for stream. It depend of the location of the dual screen.
     */
    std::string _sizeScreen;

    QTimer *_timer;
};

#endif // MEDIAPLAYER_H
