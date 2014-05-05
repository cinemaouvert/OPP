/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "playlist.h"

/**
 * @brief Manage Playlist playback scheduling
 */
class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(Playlist *playlist, const QDateTime &launchAt, QObject *parent = 0);
    virtual ~Schedule();

    /**
     * @brief Get sheduled playlist
     * @return The scheduled playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Playlist* playlist() const { return _playlist; }

    /**
     * @brief Start the schedule timer. It will timeout at `_launchAt`
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void start();

    /**
     * @brief Stop the schedule timer.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stop();

    /**
     * @brief Cancel the timer manually.
     * @warning You could not re-activate the schedule after calling this cancel()
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void cancel();

    /**
     * @brief Is the schedule launch date is expired ?
     * @return True if the schedule is expirated, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isExpired() const;

    /**
     * @brief Is the schedule activated ?
     * @return True if the schedule timer is activated, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isActive() const;

    /**
     * @brief Is the schedule canceled ?
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool canceled() const;

    /**
     * @brief Get the date the associated playlist will be launched at
     * @return The date the associated playlist will be launched at
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QDateTime & launchAt() const { return _launchAt; }

    /**
     * @brief Get the date the associated playlist will finish at. It depends of the playlist duration
     * @return The date the associated playlist will finish at
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QDateTime finishAt() const;

    /**
     * @brief Delay the schedule of `msecs` ms
     * @param msecs The duration in ms to delay. Negative delay are accepted.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void delay(int msecs);

signals:

    /**
     * @brief Emitted when the schedule timer timeout
     * @param playlist The scheduled playlist to play
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void triggered(Playlist *playlist);

private slots:

    /**
     * @brief Perform timeout action to the schedule. It is called when the timer timeout to manage specific behavior.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void timeout();

private:

    /**
     * @brief The schedule timer
     */
    QTimer *_timer;

    /**
     * @brief The date the associated playlist will be launched at
     */
    QDateTime _launchAt;

    /**
     * @brief The scheduled playlist
     */
    Playlist *_playlist;

    /**
     * @brief Tell the schedule is canceled or not.
     */
    bool _canceled;
};

#endif // SCHEDULE_H
