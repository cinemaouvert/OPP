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
     * @brief Get project title
     * @return The project title
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline const QString& projectTitle() const { return _projectTitle; }

    /**
     * @brief Get project notes
     * @return The project notes
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline const QString& projectNotes() const { return _projectNotes; }

    /**
     * @brief Set media list model
     * @param model The model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setMediaListModel(MediaListModel* model);

    /**
     * @brief Add playlist model
     * @param model The model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void addPlaylistModel(PlaylistModel* model);

    /**
     * @brief Set schedule list model
     * @param model The model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setScheduleListModel(ScheduleListModel *model);

    /**
     * @brief Save into file
     * @param file The file to save the project into. It must be a file named *.opp
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void save(QFile &file);

    /**
     * @brief Load a project from file
     * @param file The file to load. It must be a file named *.opp
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void load(QFile &file);

    /**
     * @brief Clear all model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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
     * @brief Set project title
     * @param title The new project title
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setProjectTitle(const QString &title);

    /**
     * @brief Set project notes
     * @param notes The new project notes
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setProjectNotes(const QString &notes);

protected:

    /**
     * @brief Find playlist by id
     * @param id The playlist identifier to search
     * @return The Playlist if found, NULL otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playlist* findPlaylistById(int id) const;

    /**
     * @brief Find Media by id
     * @param id The media identifier to search
     * @return The Media if found, NULL otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Media* findMediaById(int id) const;

private:

    /**
     * @brief The media list model
     */
    MediaListModel* _mediaListModel;

    /**
     * @brief List of playlist model
     */
    QList<PlaylistModel*> _playlistModelList;

    /**
     * @brief The schedule list model
     */
    ScheduleListModel* _scheduleListModel;

    /**
     * @brief The project title
     */
    QString _projectTitle;

    /**
     * @brief The project notes
     */
    QString _projectNotes;

    /**
     * @brief The application
     */
    Application *_app;

    // FIX : ref 0000001
    MainWindow *_win;
};

#endif // DATASTORAGE_H
