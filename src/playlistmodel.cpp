#include "playlistmodel.h"

#include <QDebug>
#include <QIcon>
#include <QMimeData>

#include "utils.h"
#include "medialistmodel.h"
#include "mediasettings.h"

PlaylistModel::PlaylistModel(MediaListModel *mediaListModel, QObject *parent) :
    QAbstractTableModel(parent),
    _mediaListModel(mediaListModel)
{
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return _playlist.count();
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
    if (!index.isValid() || index.row() < 0 || index.row() >= _playlist.count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Title:
            return _playlist.at(index.row())->media()->name();
            break;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Title) {
            return _playlist.at(index.row())->media()->name();
        }
        else if (index.column() == Duration) {
            return msecToQTime(_playlist.at(index.row())->media()->duration()).toString("hh:mm:ss");
        }
        else if (index.column() == Video) {
            return MediaSettings::ratioValues().at(_playlist.at(index.row())->mediaSettings()->ratio());
        }
        break;
    }

    return QVariant();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _playlist.count() || value.toString().count() == 0) {
        return false;
    }

    switch (role)
    {
    case Qt::EditRole:
        if (index.column() == Video) {
            _playlist.at(index.row())->mediaSettings()->setRatio(
                (Ratio) MediaSettings::ratioValues().indexOf(QRegExp(value.toString()))
            );
        }
        emit dataChanged(index, index);
        return true;
        break;
    }
    return false;
}

bool PlaylistModel::addPlayback(const Playback &playback)
{
    const int count = _playlist.count();

    beginInsertRows(QModelIndex(), count, count);
    _playlist.append(playback);
    endInsertRows();

    return true;
}

bool PlaylistModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{
    QString indexes = data->html();
    int countIndexes = indexes.count(":");

    for (int i = 0; i < countIndexes; i++) {
        Media *media = const_cast<Media*>( &_mediaListModel->mediaList().at(indexes.section(":", i, i).toInt()) );

        if (!media)
            return false;

        media->usageCountAdd();
        addPlayback(Playback(media));
    }
    return true;
}
