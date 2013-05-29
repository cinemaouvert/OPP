#include "playlistmodel.h"

#include <QDebug>
#include <QIcon>

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    //return sizeof(Columns);
    return 8;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return _mediaList.size();
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Title:
            return trUtf8("Title");
            break;
        case Duration:
            return trUtf8("Duration");
            break;
        case Video:
            return trUtf8("Video");
            break;
        case Audio:
            return trUtf8("Audio");
            break;
        case Subtitles:
            return trUtf8("Subtitles");
            break;
        case TestPattern:
            return trUtf8("Test pattern");
            break;
        case Gain:
            return trUtf8("Gain");
            break;
        case Status:
            return trUtf8("Status");
            break;
        }
    }

    return QVariant();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _mediaList.count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Title:
            return _mediaList[index.row()].name();
            break;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Title) {
            return _mediaList[index.row()].name();
        }
        break;
    }

    return QVariant();
}

bool PlaylistModel::addMedia(const Media &media)
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
