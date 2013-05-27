#include "medialistmodel.h"

#include "media.h"

MediaListModel::MediaListModel(QList<Media*> &mediaList, QObject *parent) :
    QAbstractListModel(parent),
    _mediaList(mediaList)
{
}

int MediaListModel::rowCount(const QModelIndex &parent) const
{
    return _mediaList.size();
}

QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _mediaList.count()) {
        return QVariant();
    }

//    switch (role)
//    {
//    case Qt::DisplayRole:
        if (index.column() == Location) {
            return _mediaList[index.row()]->location();
        }
//        else if (index.column() == Value)
//        {
//            return _mediaList[index.row()].second;
//        }
//        break;
//    }

    return QVariant();
}

//QVariant MedialListModel::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid())
//        return QVariant();

//    if (index.row() >= _mediaList.size())
//        return QVariant();

////    if (role == Qt::DisplayRole)
////        return _mediaList.at(index.row());
////    else
//        return QVariant();
//}

//QVariant MedialListModel::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     if (role != Qt::DisplayRole)
//         return QVariant();

//     if (orientation == Qt::Horizontal)
//         return QString("%1").arg(section);
//     else
//         return QString("%1").arg(section);
// }

void MediaListModel::addMedia(Media *media)
{
    const int count = _mediaList.count();
    beginInsertRows(QModelIndex(), count, count);
    _mediaList << media;
    endInsertRows();
}
