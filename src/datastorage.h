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

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QString>
#include <QFile>

class MediaListModel;
class PlaylistModel;
class ScheduleListModel;
class Application;
class Media;
class Playlist;

// FIX : ref 0000001
class MainWindow;

class DataStorage : public QObject
{
    Q_OBJECT
public:
    explicit DataStorage(Application *app, MainWindow *win/*FIX : ref 0000001*/, QObject *parent = 0);

    /**
     * @brief projectTitle
     */
    inline const QString& projectTitle() const { return _projectTitle; }

    /**
     * @brief projectNotes
     */
    inline const QString& projectNotes() const { return _projectNotes; }

    /**
     * @brief setMediaListModel
     */
    void setMediaListModel(MediaListModel* model);

    /**
     * @brief addPlaylistModel
     */
    void addPlaylistModel(PlaylistModel* model);

    /**
     * @brief setScheduleListModel
     */
    void setScheduleListModel(ScheduleListModel *model);

    /**
     * @brief save
     * @param file
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void save(QFile &file);

    /**
     * @brief load
     * @param file
     */
    void load(QFile &file);

    /**
     * @brief clear
     */
    void clear();
    
signals:

    /**
     * @brief saved
     */
    void saved();

    /**
     * @brief loaded
     */
    void loaded();

public slots:

    /**
     * @brief setProjectTitle
     * @param title
     */
    void setProjectTitle(const QString &title);

    /**
     * @brief setProjectNotes
     * @param notes
     */
    void setProjectNotes(const QString &notes);

protected:

    /**
     * @brief findPlaylistById
     * @param id
     * @return
     */
    Playlist* findPlaylistById(int id) const;

    /**
     * @brief findMediaById
     * @param id
     * @return
     */
    Media* findMediaById(int id) const;

private:

    /**
     * @brief _mediaListModel
     */
    MediaListModel* _mediaListModel;

    /**
     * @brief _playlistModelList
     */
    QList<PlaylistModel*> _playlistModelList;

    /**
     * @brief _scheduleListModel
     */
    ScheduleListModel* _scheduleListModel;

    /**
     * @brief _projectTitle
     */
    QString _projectTitle;

    /**
     * @brief _projectNotes
     */
    QString _projectNotes;

    /**
     * @brief _app
     */
    Application *_app;

    // FIX : ref 0000001
    MainWindow *_win;
};

#endif // DATASTORAGE_H
