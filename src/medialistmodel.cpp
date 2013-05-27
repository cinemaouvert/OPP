#include "medialistmodel.h"

#include <QDebug>
#include <QIcon>
#include "media.h"

MediaListModel::MediaListModel(QList<Media> &mediaList, QObject *parent) :
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
            return _mediaList[index.row()].name();
            break;
        case Location:
            return _mediaList[index.row()].location();
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
            return _mediaList[index.row()].location();
        }
        if (index.column() == Name) {
            return _mediaList[index.row()].name();
        }
        break;
    }

    return QVariant();
}

//QVariant MedialListModel::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     if (role != Qt::DisplayRole)
//         return QVariant();

//     if (orientation == Qt::Horizontal)
//         return QString("%1").arg(section);
//     else
//         return QString("%1").arg(section);
// }

bool MediaListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        _mediaList.removeAt(position);
    }

    endRemoveRows();
    return true;
}

bool MediaListModel::addMedia(const Media &media)
{
    const int count = _mediaList.count();
    if (!_mediaList.contains(media)) {
        beginInsertRows(QModelIndex(), count, count);
        _mediaList.append(media);
        endInsertRows();
        return true;
    } else {
        return false;
    }
}
