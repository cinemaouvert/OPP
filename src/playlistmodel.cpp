#include "playlistmodel.h"

#include <QDebug>
#include <QIcon>
#include <QMimeData>

#include "utils.h"
#include "medialistmodel.h"
#include "mediasettings.h"
#include "playlistplayer.h"

PlaylistModel::PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, QObject *parent) :
    QAbstractTableModel(parent),
    _mediaListModel(mediaListModel),
    _playlist(playlist)
{
    _activeItem.first = -1;
    _activeItem.second = Idle;
}

PlaylistModel::~PlaylistModel()
{
    delete _playlist;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return _playlist->count();
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
        case Status:
            return trUtf8("Status");
            break;
        }
    }

    return QVariant();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _playlist->count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DecorationRole:
        if (index.column() == Status && index.row() == _activeItem.first) {
            switch(_activeItem.second)
            {
            case Playing:
                return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_173_play.png"));
                break;
            case Paused:
                return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_174_pause.png"));
                break;
            }
        }
        break;
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Title:
            return _playlist->at(index.row())->media()->name();
            break;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Title) {
            return _playlist->at(index.row())->media()->name();
        }
        else if (index.column() == Duration) {
            return msecToQTime(_playlist->at(index.row())->media()->duration()).toString("hh:mm:ss");
        }
        else if (index.column() == Video) {
            QSize size = _playlist->at(index.row())->media()->videoResolution();
            return QString("%1 x %2").arg(size.width()).arg(size.height());
        }
        break;
    }

    return QVariant();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _playlist->count() || value.toString().count() == 0) {
        return false;
    }

    switch (role)
    {
    case Qt::EditRole:
//        if (index.column() == Video) {
//            _playlist->at(index.row())->mediaSettings()->setRatio(
//                (Ratio) MediaSettings::ratioValues().indexOf(QRegExp(value.toString()))
//            );
//        }
//        else if (index.column() == Subtitles) {
//            qDebug() << value;
////            _playlist->at(index.row())->mediaSettings()->setSubtitlesTrack();
//        } else if (index.column() == Subtitles) {
//            qDebug() << value;
////            _playlist->at(index.row())->mediaSettings()->setSubtitlesTrack();
//        }
        emit dataChanged(index, index);
        return true;
        break;
    }
    return false;
}

bool PlaylistModel::addPlayback(Playback *playback)
{
    const int count = _playlist->count();

    beginInsertRows(QModelIndex(), count, count);
    _playlist->append(playback);
    endInsertRows();

    return true;
}

bool PlaylistModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{
    QString indexes = data->html();
    int countIndexes = indexes.count(":");

    for (int i = 0; i < countIndexes; i++) {
        Media *media = _mediaListModel->mediaList().at(indexes.section(":", i, i).toInt());

        if (!media)
            return false;

        addPlayback(new Playback(media));
    }
    return true;
}

void PlaylistModel::removePlaybackWithDeps(Media *media)
{
    for (int i = _playlist->count() - 1; i >= 0; i--) {
        if (_playlist->at(i)->media() == media) {
            removePlayback(i);
        }
    }
}

void PlaylistModel::removePlayback(int index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), index, index);

    _playlist->removeAt(index);

    endRemoveRows(); 
}

void PlaylistModel::playItem()
{
    _activeItem.second = Playing;
    emit layoutChanged();
}

void PlaylistModel::pauseItem()
{
    _activeItem.second = Paused;
    emit layoutChanged();
}

void PlaylistModel::stopItem()
{
    _activeItem.second = Idle;
    emit layoutChanged();
}

void PlaylistModel::setPlayingItem(int index)
{
    _activeItem.first = index;
    emit layoutChanged();
}
