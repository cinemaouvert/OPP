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

#include "medialistmodel.h"

#include <QDebug>
#include <QIcon>
#include <QStandardItem>

#include "utils.h"
#include "media.h"

MediaListModel::MediaListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int MediaListModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int MediaListModel::rowCount(const QModelIndex &parent) const
{
    return _mediaList.size();
}

Qt::ItemFlags MediaListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MediaListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Name:
            return trUtf8("Name");
            break;
        case Duration:
            return trUtf8("Duration");
            break;
        case Used:
            return trUtf8("Used");
            break;
        case Location:
            return trUtf8("Location");
            break;
        }
    }

    return QVariant();
}

QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _mediaList.count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Name:
            return _mediaList[index.row()]->name();
            break;
        case Location:
            return _mediaList[index.row()]->location();
            break;
        case Duration:
            return msecToQTime(_mediaList[index.row()]->duration()).toString("hh:mm:ss");
            break;
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == Used) {
            QIcon icon;
            if (_mediaList[index.row()]->isUsed()){
                icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_152_check.png"), QSize(), QIcon::Normal, QIcon::Off);
            }
            else
                icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_153_unchecked.png"), QSize(), QIcon::Normal, QIcon::Off);
            return icon;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Location) {
            return _mediaList[index.row()]->location();
        }
        if (index.column() == Name) {
            return _mediaList[index.row()]->name();
        }
        if (index.column() == Duration) {
            return msecToQTime(_mediaList[index.row()]->duration()).toString("hh:mm:ss");
        }
        break;
    }

    return QVariant();
}

bool MediaListModel::removeMedia(int index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), index, index);

    _mediaFileList.removeAll(_mediaList[index]->location());
    delete _mediaList[index];
    _mediaList.removeAt(index);

    endRemoveRows();

    emit mediaListChanged(_mediaList.count());
    return true;
}

bool MediaListModel::addMedia(Media *media)
{   
    if (_mediaFileList.contains(media->location()))
        return false;

    const int count = _mediaList.count();

    beginInsertRows(QModelIndex(), count, count);

    _mediaList.append(media);
    connect(media, SIGNAL(usageCountChanged()), this, SIGNAL(layoutChanged()));


    endInsertRows();


    _mediaFileList.append(media->location());

    emit mediaListChanged(_mediaList.count());
    return true;
}

QDataStream & operator << (QDataStream & out, const QList<Media> &list)
{
    for(int i=0; i<list.size(); ++i){
        QString loc(list[i].location());
        out << loc;
    }
    return out;
}

QDataStream & operator >> (QDataStream & in, QList<Media> &list)
{
    in >> list;
    return in;
}

QTime MediaListModel::totalDuration()
{
    int duration = 0;

    foreach (Media* media, _mediaList) {
         duration += media->duration();
    }

    return msecToQTime(duration);
}

int MediaListModel::countPictures()
{
    int count = 0;

    foreach (Media* media, _mediaList)
        if (media->isImage()) count++;

    return count;
}

int MediaListModel::countMovies()
{
    int count = 0;

    foreach (Media* media, _mediaList)
        if (!media->isImage()) count++;

    return count;
}

void MediaListModel::removeAll()
{
    for (int i = _mediaList.count()-1; i >= 0; i--) {
        removeMedia(i);
    }
}
