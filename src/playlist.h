/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

#include "playback.h"

struct libvlc_instance_t;

/**
 * @brief Manage list of Playback
 */
class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(const QString &title, QObject *parent = 0);
    virtual ~Playlist();

    /**
     * @brief Get Playback list
     * @return The list of Playback instance
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QList<Playback*>& playbackList() { return _playbackList; }

    /**
     * @brief Get identifier. It is used for serialization
     * @return The playlist identifier
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline int id() const { return _id; }

    /**
     * @brief Get title
     * @return The playlist title
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline QString title() const { return _title; }

    /**
     * @brief Set identifier
     * @param id The new identifier
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setId(int id);

    /**
     * @brief Set title
     * @param title The new playlist title
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setTitle(const QString &title);

    /**
     * @brief Get item at `index`
     * @param index Index of the item into the list
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playback* at(const int &index) const;

    /**
     * @brief Append item into the list
     * @param playback The item to append into the list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void append(Playback *playback);

    /**
     * @brief Remove item at `index`
     * @param index Index of the item to remove
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeAt(int index);

    /**
     * @brief Get item index
     * @param playback The item to search
     * @return The index of `playback` into the list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int indexOf(Playback *playback) const;

    /**
     * @brief Move item at `from` to position `to`
     * @param from Index of the item to move
     * @param to Destination index
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void move(int from, int to);

    /**
     * @brief Get number of items into the list
     * @return The number of items
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int count() const;

    /**
     * @brief Get total playback duration of the playlist
     * @return The total duration in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint totalDuration() const;

signals:

    /**
     * @brief titleChanged
     */
    void titleChanged();

private:

    /**
     * @brief Number of created instance, used to set unique identifier for each instance
     */
    static int s_instanceCount;

    /**
     * @brief The playlist identifier
     */
    int _id;

    /**
     * @brief The list of Playback
     */
    QList<Playback*> _playbackList;

    /**
     * @brief The playlist title
     */
    QString _title;
};

#endif // PLAYLIST_H
