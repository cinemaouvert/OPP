/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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

struct libvlc_media_list_t;
struct libvlc_instance_t;

class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(libvlc_instance_t *vlcInstance, const QString &title, QObject *parent = 0);
    virtual ~Playlist();

    /**
     * @brief playbackList
     * @return
     */
    inline const QList<Playback*>& playbackList() { return _playbackList; }

    /**
     * @brief id
     * @return
     */
    inline int id() const { return _id; }

    /**
     * @brief core
     * @return
     */
    inline libvlc_media_list_t* core() const { return _vlcMediaList; }

    /**
     * @brief title
     * @return
     */
    inline QString title() const { return _title; }

    /**
     * @brief setId
     * @param id
     */
    void setId(int id);

    /**
     * @brief setTitle
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief at
     * @param index
     * @return
     */
    Playback* at(const int &index) const;

    /**
     * @brief append
     * @param playback
     */
    void append(Playback *playback);

    /**
     * @brief removeAt
     * @param index
     */
    void removeAt(int index);

    /**
     * @brief indexOf
     * @param playback
     * @return
     */
    int indexOf(Playback *playback) const;

    /**
     * @brief move
     * @param from
     * @param to
     */
    void move(int from, int to);

    /**
     * @brief count
     * @return
     */
    int count() const;

    /**
     * @brief totalDuration
     * @return
     */
    uint totalDuration() const;

    /**
     * @brief lock
     */
    void lock();

    /**
     * @brief unlock
     */
    void unlock();

signals:

    /**
     * @brief titleChanged
     */
    void titleChanged();

private:

    /**
     * @brief s_instanceCount
     */
    static int s_instanceCount;

    /**
     * @brief _id
     */
    int _id;

    /**
     * @brief _playbackList
     */
    QList<Playback*> _playbackList;

    /**
     * @brief _vlcMediaList
     */
    libvlc_media_list_t *_vlcMediaList;

    /**
     * @brief _title
     */
    QString _title;
};

#endif // PLAYLIST_H
