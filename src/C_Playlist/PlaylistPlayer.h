/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
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

struct libvlc_instance_t;

/**
 * @brief Manage playback of Playlist
 */
class PlaylistPlayer : public QObject
{
    Q_OBJECT
public:
    enum Loop
    {
        NOLOOP,
        BIGLOOP,
        SINGLELOOP
    };

    explicit PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent = 0);
    
    ~PlaylistPlayer();

    /**
     * @brief Get media player
     * @return The media player
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline MediaPlayer* mediaPlayer() const { return _mediaPlayer; }

    /**
     * @brief The current used playlist
     * @return The current playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playlist* currentPlaylist() const { return _playlist; }

    /**
     * @brief Set current playlist
     * @param list The new playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setPlaylist(Playlist *list);

public slots:

    /**
     * @brief Play item at `index`
     * @param index The index of item in current playlist to start playback from
     */
    void playItemAt(const int &index);

    /**
     * @brief Initialize item at `index`
     * @param index The index of item in current playlist to initialize
     */
    void initItemAt(const int &index);

    /**
     * @brief Play next playback in current playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void next();

    /**
     * @brief Play previous playback in current playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void previous();

    /**
     * @brief Forward the playback to 1 second
     * @return newTime
     */
    int rewind();

    /**
     * @brief Rewind the playback to 1 second
     * @return newTime
     */
    int forward();

    /**
     * @brief Stop the player
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stop();

    /**
     * @brief Play the current index
     */
    void play();

    /**
     * @brief Set and play a playlist.
     * @param playlist The playlist to play
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void playPlaylist(Playlist *playlist);

    /**
     * @brief Get currentIndex
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    int getCurrentIndex();

    /**
     * @brief Current index ++
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void currentIndexUp();

    /**
     * @brief Current index --
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void currentIndexDown();

    void setLoop(Loop newLoopState);

private slots:

    /**
     * @brief Handle player end state.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void handlePlayerEnd();

signals:

    /**
     * @brief itemChanged
     */
    void itemChanged(int);

    /**
     * @brief end
     */
    void end();

private:

    /**
     * @brief The current playlist
     */
    Playlist *_playlist;

    /**
     * @brief The media player used to play playlist item
     */
    MediaPlayer *_mediaPlayer;

    /**
     * @brief Current index in `_playlist`
     */
    int _currentIndex;

    Loop _loop;
};

#endif // PLAYLISTPLAYER_H
