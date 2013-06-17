/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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
    enum Columns { Name = 0, Duration = 1, Used = 2, Location = 3 };

    MediaListModel(QObject *parent = 0);

    inline const QList<Media*>& mediaList() { return _mediaList; }

    int columnCount(const QModelIndex &parent) const;

    QTime totalDuration();

    int countPictures();

    int countMovies();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool removeMedia(int index);

    bool addMedia(Media *media);

    void removeAll();

signals:
    /**
     * @brief signal mediaListChanged
     * @param The number of media after the list changed
     */
    void mediaListChanged(int);

private:
    QList<Media*> _mediaList;

    QStringList _mediaFileList;

    friend QDataStream & operator << (QDataStream &, const QList<Media> &);
    friend QDataStream & operator >> (QDataStream &, QList<Media> &);

};

//Metatype
Q_DECLARE_METATYPE(MediaListModel*)
Q_DECLARE_METATYPE( QList<Media> )
QDataStream & operator << (QDataStream & out, const QList<Media> &list);
QDataStream & operator >> (QDataStream & in, QList<Media> &list);
#endif // MEDIALISTMODEL_H
