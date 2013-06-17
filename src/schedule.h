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

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "playlist.h"

class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(Playlist *playlist, const QDateTime &launchAt, QObject *parent = 0);
    virtual ~Schedule();

    inline Playlist* playlist() const { return _playlist; }


    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief cancel
     */
    void cancel();

    /**
     * @brief isExpired
     * @return
     */
    bool isExpired() const;

    /**
     * @brief isActive
     * @return
     */
    bool isActive() const;

    /**
     * @brief wasTriggered
     * @return
     */
    bool wasTriggered() const;

    /**
     * @brief launchAt
     * @return
     */
    inline const QDateTime & launchAt() const { return _launchAt; }

    /**
     * @brief finishAt
     * @return
     */
    QDateTime finishAt() const;

    /**
     * @brief delay
     * @param ms
     */
    void delay(int ms);

signals:

    /**
     * @brief triggered
     * @param playlist
     */
    void triggered(Playlist *playlist);

private slots:

    /**
     * @brief timeout
     */
    void timeout();

private:

    /**
     * @brief _timer
     */
    QTimer _timer;

    /**
     * @brief _launchAt
     */
    QDateTime _launchAt;

    /**
     * @brief _playlist
     */
    Playlist *_playlist;

    /**
     * @brief _wasTriggered
     */
    bool _wasTriggered;
};

#endif // SCHEDULE_H
