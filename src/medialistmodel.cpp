#include "medialistmodel.h"

#include <QDebug>
#include <QIcon>
#include "media.h"

MediaListModel::MediaListModel(QList<Media*> &mediaList, QObject *parent) :
    QAbstractTableModel(parent),
    _mediaList(mediaList)
{
}

int MediaListModel::columnCount(const QModelIndex &parent) const
{
    return sizeof(Columns);
}

int MediaListModel::rowCount(const QModelIndex &parent) const
{
    return _mediaList.size();
}

Qt::ItemFlags MediaListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//    if (index.column() == Name)
//    {
//        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//    }
//    else if (index.column() == Value)
//    {
//        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
//    }

//    return QAbstractTableModel::flags(index);
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
        case Length:
            return trUtf8("Length");
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
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == Used) {
            QIcon icon;
            // unckeck icon : /icons/resources/glyphicons/glyphicons_153_unchecked.png
            icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_152_check.png"), QSize(), QIcon::Normal, QIcon::Off);
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
        break;
    }

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
