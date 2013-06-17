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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "playlist.h"

class PlaylistPlayer;
class Playback;
class MediaListModel;
class ScheduleListModel;

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { Title = 0, Duration = 1, Video = 2, Audio = 3, Subtitles = 4, TestPattern = 5, Status = 6 };
    enum PlaybackState { Playing = 0, Paused = 1, Idle = 2 };

    PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, ScheduleListModel *scheduleListModel, QObject *parent = 0);

    virtual ~PlaylistModel();

    inline Playlist* playlist() const { return _playlist; }

    int activeItemIndex() const { return _activeItem.first; }

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool addPlayback(Playback *playback);

    void removePlaybackWithDeps(Media *media);

    void removePlayback(int index);

    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );

    void updateLayout();

    bool moveUp(const QModelIndex &index);

    bool moveDown(const QModelIndex &index);

    void removeAll();

public slots:

    void playItem();

    void pauseItem();

    void stopItem();

    void setPlayingItem(int index);

private:
    Playlist *_playlist;
    MediaListModel *_mediaListModel;
    QPair<int, PlaybackState> _activeItem;
    ScheduleListModel *_scheduleListModel;
};


#endif // PLAYLISTMODEL_H
