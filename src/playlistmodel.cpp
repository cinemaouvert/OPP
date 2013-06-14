#include "playlistmodel.h"

#include <QDebug>
#include <QIcon>
#include <QMimeData>

#include "utils.h"
#include "medialistmodel.h"
#include "mediasettings.h"
#include "playlistplayer.h"
#include "schedulelistmodel.h"

PlaylistModel::PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, ScheduleListModel *scheduleListModel, QObject *parent) :
    QAbstractTableModel(parent),
    _mediaListModel(mediaListModel),
    _playlist(playlist),
    _scheduleListModel(scheduleListModel)

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

    Media *media = _playlist->at(index.row())->media();
    MediaSettings *mediaSettings = _playlist->at(index.row())->mediaSettings();

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
            return media->name();
            break;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Title) {
            return media->name();
        }
        else if (index.column() == Duration) {
            return msecToQTime(media->duration()).toString("hh:mm:ss");
        }
        else if (index.column() == Video) {
            return QString("%1 x %2 ")
                    .arg(mediaSettings->videoTrack().width())
                    .arg(mediaSettings->videoTrack().height()
            ) + mediaSettings->videoTrack().codecDescription();
        }
        else if (index.column() == Audio) {
            return mediaSettings->audioTrack().codecDescription();
        }
        else if (index.column() == Subtitles) {
            if (mediaSettings->subtitlesTrack().trackId() == -1)
                return "Disabled";
            else
                return QString("Track %1").arg(mediaSettings->subtitlesTrack().trackId());
        }
        break;
    }

    return QVariant();
}

bool PlaylistModel::addPlayback(Playback *playback)
{
    const int count = _playlist->count();

    beginInsertRows(QModelIndex(), count, count);
    _playlist->append(playback);
    endInsertRows();
    _scheduleListModel->updateLayout();

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
    _scheduleListModel->updateLayout();
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

void PlaylistModel::updateLayout()
{
    emit layoutChanged();
}

bool PlaylistModel::moveUp(const QModelIndex &index)
{
    if(index.row()>0)
    {
        _playlist->move(index.row(),index.row()-1);
        updateLayout();
        return true;
    }
    return false;
}

bool PlaylistModel::moveDown(const QModelIndex &index)
{
    if(index.row()<_playlist->count()-1)
    {
        _playlist->move(index.row(),index.row()+1);
        updateLayout();
        return true;
    }
    return false;
}

void PlaylistModel::removeAll()
{
    for (int i = _playlist->count()-1; i >= 0; i--) {
        removePlayback(i);
    }
}

