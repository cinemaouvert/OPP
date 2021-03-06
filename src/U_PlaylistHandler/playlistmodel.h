/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thomas Berthomé <thoberthome@laposte.net>
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
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QIcon>
#include <QMimeData>

#include <vector>

#include "mainwindow.h"
#include "Playlist.h"
#include "utils.h"
#include "medialistmodel.h"
#include "mediasettings.h"
#include "PlaylistPlayer.h"
#include "schedulelistmodel.h"

class PlaylistPlayer;
class Playback;
class MediaListModel;
class ScheduleListModel;

using std::vector;

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /**
     * @enum Columns
     * @brief The Columns enum
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    enum Columns {Status = 0, Title = 1, Duration = 2, Video = 3, Audio = 4, Subtitles = 5, TestPattern = 6};

    /**
     * @enum PlaybackState
     * @brief The PlaybackState enum
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    enum PlaybackState { Playing = 0, Paused = 1, Idle = 2 };

    PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, ScheduleListModel *scheduleListModel, MainWindow *mw, QObject *parent = 0);

    virtual ~PlaylistModel();

    /**
     * @brief Returns the playlist
     * @return The playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Playlist* playlist() const { return _playlist; }

    /**
     * @brief Returns the index of active item
     * @return The index of active item
     */
    int activeItemIndex() const { return _activeItem.first; }

    PlaybackState state(){ return _activeItem.second; }

    /**
     * @brief Returns the number of columns
     * @return The number of columns
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief Returns the number of rows
     * @return The number of rows
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the flags applied to the model
     * @param index
     * @return The flags applied to the model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * @brief Returns the headers data
     * @param section
     * @param orientation
     * @param role
     * @return The headers data
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief Fill the playlist table
     * @param index
     * @param role
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, QVariant value, int role);

    /**
     * @brief Add a playback to the playlist
     * @param playback
     * @return True if the playback has been added, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool addPlayback(Playback *playback,int row=-1);

    /**
     * @brief Remove playback matching to the media
     * @param media
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removePlaybackWithDeps(Media *media);

    /**
     * @brief Remove a playback
     * @param index
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removePlayback(int index);

    /**
     * @brief Drop mime data
     * @return True if data has been dropped, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );

    /**
     * @brief Update the layout
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateLayout();

    /**
     * @brief Move an item up
     * @return True if the item has been moved, false otherwise
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    bool moveUp(const QModelIndex &index);

    /**
     * @brief Move an item down
     * @return True if the item has been moved, false otherwise
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    bool moveDown(const QModelIndex &index);

    /**
     * @brief Remove all items in the playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeAll();

    /**
     * @brief Get if the playlist is running or not
     */
    bool isRunning(){return _running;}

    /**
     * @brief Fix delay for no more overlapping
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
    */
    void repareDelay(QList<Schedule*> scheduleList, uint duration);

    /**
     * @brief Getif the media is running or not
     *
     * @author Thibaud Lamarche <thibaud.lamarche@gmail.com>
     */
    bool isRunningMedia(Media *media){
        for (int i = 0; i < _playlist->count(); i++) {
            if ((*_playlist->at(i)->media()) == media) {
                return isRunningMedia(i);
            }
        }
        return false;
    }

    /**
     * @brief Getif the media is running or not
     *
     * @author Thibaud Lamarche <thibaud.lamarche@gmail.com>
     */
    bool isRunningMedia(int index){
        return _running && _activeItem.first >= index;
    }

signals:

    /**
     * @brief activeItemChanged
     */
    void activeItemChanged();

public slots:

    /**
     * @brief Play the current item
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void playItem();

    /**
     * @brief Pause the playback of the current item
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void pauseItem();

    /**
     * @brief Stop the playback of the current item
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stopItem();

    /**
     * @brief Change the item to play
     * @param index
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setActiveItem(int index);

private:
    /**
     * @brief _playlist The playlist
     */
    Playlist *_playlist;
    /**
     * @brief _mediaListModel The media list model
     */
    MediaListModel *_mediaListModel;
    /**
     * @brief _activeItem The active item
     */
    QPair<int, PlaybackState> _activeItem;
    /**
     * @brief _scheduleListModel The schedule list model
     */
    ScheduleListModel *_scheduleListModel;

    /**
     * @brief _running get if the playlist is running or not
     */
    bool _running;

    /**
     * @brief mainwindow
     */
    MainWindow *_mw;

    vector<QString> _subtitle;

};


#endif // PLAYLISTMODEL_H
