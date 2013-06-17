/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef PLAYLISTPLAYER_H
#define PLAYLISTPLAYER_H

#include <QObject>

class MediaPlayer;
class Playlist;

struct libvlc_media_list_player_t;
struct libvlc_media_list_t;
struct libvlc_media_t;
struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_instance_t;

class PlaylistPlayer : public QObject
{
    Q_OBJECT
public:

    explicit PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    
    ~PlaylistPlayer();

    /**
     * @brief core
     * @return
     */
    inline libvlc_media_list_player_t* core() const { return _vlcMediaListPlayer; }

    /**
     * @brief mediaPlayer
     * @return
     */
    inline MediaPlayer* mediaPlayer() const { return _mediaPlayer; }

    /**
     * @brief currentPlaylist
     * @return
     */
    Playlist* currentPlaylist() const { return _playlist; }

    /**
     * @brief setPlaylist
     * @param list
     */
    void setPlaylist(Playlist *list);

    /**
     * @brief isPlaying
     * @return
     */
    bool isPlaying() const;

public slots:

    /**
     * @brief playItemAt
     * @param index
     */
    void playItemAt(const int &index);

    /**
     * @brief next
     */
    void next();

    /**
     * @brief previous
     */
    void previous();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief playPlaylist
     * @param playlist
     */
    void playPlaylist(Playlist *playlist);

    /**
     * @brief applyCurrentPlaybackSettings
     */
    void applyCurrentPlaybackSettings();

private slots:

    /**
     * @brief handlePlayerEnd
     */
    void handlePlayerEnd();

signals:

    /**
     * @brief itemChanged
     */
    void itemChanged(int);

    /**
     * @brief nextItem
     */
    void nextItem();

    /**
     * @brief end
     */
    void end();

private:

    /**
     * @brief libvlc_callback
     * @param event
     * @param data
     */
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    /**
     * @brief createCoreConnections
     */
    void createCoreConnections();

    /**
     * @brief removeCoreConnections
     */
    void removeCoreConnections();

    /**
     * @brief _vlcMediaListPlayer
     */
    libvlc_media_list_player_t *_vlcMediaListPlayer;

    /**
     * @brief _vlcEvents
     */
    libvlc_event_manager_t *_vlcEvents;

    /**
     * @brief _playlist
     */
    Playlist *_playlist;

    /**
     * @brief _mediaPlayer
     */
    MediaPlayer *_mediaPlayer;

    /**
     * @brief _currentIndex
     */
    int _currentIndex;
};

#endif // PLAYLISTPLAYER_H
