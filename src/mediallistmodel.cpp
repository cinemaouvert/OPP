#include "mediallistmodel.h"

#include "media.h"

MedialListModel::MedialListModel(QList<Media*> &mediaList, QObject *parent) :
    QAbstractListModel(parent),
    _mediaList(mediaList)
{
}

int MedialListModel::rowCount(const QModelIndex &parent) const {
    return _mediaList.size();
}

QVariant MedialListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _mediaList.size())
        return QVariant();

//    if (role == Qt::DisplayRole)
//        return _mediaList.at(index.row());
//    else
        return QVariant();
}

QVariant MedialListModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         return QString("%1").arg(section);
     else
         return QString("%1").arg(section);
 }
