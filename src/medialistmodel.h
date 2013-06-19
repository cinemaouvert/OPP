/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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

#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

#include "media.h"
#include<QTime>
#include "utils.h"

class MediaListModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    /**
     * @enum Columns
     * @brief The Columns enum
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    enum Columns { Name = 0, Duration = 1, Used = 2, Location = 3 };

    MediaListModel(QObject *parent = 0);

    /**
     * @brief Returns the media list
     * @return The media list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QList<Media*>& mediaList() { return _mediaList; }

    /**
     * @brief Returns the number of columns
     * @return The number of columns
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief Returns the total duration of the medias
     * @return The total duration of the medias
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Hamza Haddar <ham.haddar@gmail.com>
     */
    QTime totalDuration();

    /**
     * @brief Returns the number of pictures in the media list
     * @return The number of pictures in the media list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Hamza Haddar <ham.haddar@gmail.com>
     */
    int countPictures();

    /**
     * @brief Returns the number of movies in the media list
     * @return The number of movies in the media list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     * @author Hamza Haddar <ham.haddar@gmail.com>
     */
    int countMovies();

    /**
     * @brief Returns the number of rows
     * @return The number of rows
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the flags applied to the model
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
     * @brief Fill the media list table
     * @param index
     * @param role
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief Remove the media at specified index from the media list
     * @param index
     * @return True if the media has been removed, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool removeMedia(int index);

    /**
     * @brief Add a media to the media list
     * @param media
     * @return True if the media has been added, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool addMedia(Media *media);

    /**
     * @brief Remove all the medias from the media list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeAll();

signals:
    /**
     * @brief signal mediaListChanged
     * @param The number of media after the list changed
     */
    void mediaListChanged(int);

private:
    /**
     * @brief _mediaList The media list
     */
    QList<Media*> _mediaList;

    /**
     * @brief _mediaFileList
     */
    QStringList _mediaFileList;
};

#endif // MEDIALISTMODEL_H
