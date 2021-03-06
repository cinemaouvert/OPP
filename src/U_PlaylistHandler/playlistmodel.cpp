/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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

#include "playlistmodel.h"
#include "mainwindow.h"

PlaylistModel::PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, ScheduleListModel *scheduleListModel, MainWindow* mw, QObject *parent) :
    QAbstractTableModel(parent),
    _playlist(playlist),
    _mediaListModel(mediaListModel),
    _scheduleListModel(scheduleListModel),
    _mw(mw)
{
    _activeItem.first = -1;
    _activeItem.second = Idle;
    _running = false;
}

PlaylistModel::~PlaylistModel()
{
    delete _playlist;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 7;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _playlist->count();
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Status:
            return trUtf8("Status");
            break;
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
        }
    }

    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return QVariant(QString::number(section + 1));
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
            default :
                break;
            }
        }
        break;
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Title:
            return media->name();
            break;
        case Subtitles:
            return _subtitle.at(index.row());
            break;
        default:
            break;
        }
    case Qt::DisplayRole:
        if (index.column() == Title) {
            return media->name();
        }
        else if (index.column() == Duration) {
            return msecToQTime(media->duration()).toString("hh:mm:ss");
        }
        else if (index.column() == Video) {
            if (media->videoTracks().count() == 0 || mediaSettings->videoTrack() < 0) {
                return "Disabled";
            } else {
                libvlc_media_player_t *mp;
                mp = libvlc_media_player_new_from_media(media->core());
                return media->extension().toUpper()
                        +(" (")
                        +QString("%1 x %2")
                        .arg(libvlc_video_get_width(mp))
                        .arg(libvlc_video_get_height(mp))
                        +(")");
            }
        }
        else if (index.column() == Audio) {
            if (media->audioTracks().count() == 0  || mediaSettings->audioTrack() == 0) {
                return "Disabled";
            } else {
                //return media->audioTracks().at(mediaSettings->audioTrack()-1).codecDescription();
                return media->codec_audio().at(0);
            }
        }
        else if (index.column() == Subtitles) {
            return _subtitle.at(index.row());
        }
        break;
    }

    return QVariant();
}

bool PlaylistModel::setData(const QModelIndex &index, QVariant value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _playlist->count()) {
        return false;
    }

    switch (role)
    {
    case Qt::DecorationRole:
        break;
    case Qt::ToolTipRole:
        break;
    case Qt::DisplayRole:
        if (index.column() == Subtitles) {
            _subtitle[index.row()] = value.toString();

            QModelIndex top = createIndex(index.row(), 0);
            QModelIndex bottom = createIndex(index.row(), 6);
            emit dataChanged(top, bottom);
            return true;
        }
        break;
    }

    return true;
}

bool PlaylistModel::addPlayback(Playback *playback, int row)
{
    const int count = _playlist->count();
    if(row == -1)
        row = count;
    beginInsertRows(QModelIndex(), row, row);
    _playlist->append(playback,row);

    if(!playback->mediaSettings()->subtitlesFile().isEmpty()){
        _subtitle.push_back(playback->mediaSettings()->subtitlesFile());
    } else if (playback->media()->subtitlesTracks().count() == 0 || playback->mediaSettings()->subtitlesTrack() == 0) {
        _subtitle.push_back("Disabled");
    } else {
        _subtitle.push_back(QString("Track %1")
                .arg(playback->media()->subtitlesTracks().at(playback->mediaSettings()->subtitlesTrack()-1).trackId()));
    }

    endInsertRows();
    _scheduleListModel->updateLayout();
    _mw->updateProjectSummary();

    updateLayout();

    return true;
}

bool PlaylistModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{
    Q_UNUSED(action);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    QString indexes = data->html();

    int countIndexes = indexes.count(":");
    if(indexes.startsWith("#")){
        if(!isRunning()){
            if(row != -1)
                 _playlist->move(indexes.remove("#").toInt(),row);
        }else{
            QMessageBox::critical(NULL, tr("Moving during playlist"), tr("This media can not be moved with drag and drop during a projection.(Please use the arrows on the right)") , tr("Ok"));
        }
    }else{
        for (int i = 0; i < countIndexes; i++) {

            Media *media = _mediaListModel->mediaList().at(indexes.section(":", i, i).toInt());
            if (!media)
                return false;

            QList<Schedule*> scheduleList = _scheduleListModel->scheduleList();

            foreach(Schedule *schedule, scheduleList)
            {
                if(this->playlist() == schedule->playlist())
                {
                    foreach(Schedule *schedule2, scheduleList)
                    {
                        if(schedule->launchAt() < schedule2->launchAt())
                        {
                            if(schedule->finishAt().addMSecs(media->duration()) > schedule2->launchAt())
                            {
                                int delay = QMessageBox::warning(NULL, tr("Add track into playlist"), tr("This new track create an overlapping.") , tr("Delay automation"), tr("Do not add track"));

                                if(delay == 0)
                                {
                                    uint t = media->duration();
                                    schedule2->delay(t);
                                    repareDelay(scheduleList, t);
                                }
                                if(delay == 1)
                                {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
            addPlayback(new Playback(media),row);
        }
    }

    return true;
}

void PlaylistModel::removePlaybackWithDeps(Media *media)
{
    for (int i = _playlist->count() - 1; i >= 0; i--) {
        if ((*_playlist->at(i)->media()) == media) {
            removePlayback(i);
        }
    }

    updateLayout();
}

void PlaylistModel::removePlayback(int index)
{
    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), index, index);

    _playlist->removeAt(index);

    endRemoveRows();
    _scheduleListModel->updateLayout();
    _mw->updateProjectSummary();

    updateLayout();
}

void PlaylistModel::playItem()
{
    _running = true;
    _activeItem.second = Playing;
    updateLayout();
}

void PlaylistModel::pauseItem()
{
    _running = true;
    _activeItem.second = Paused;
    updateLayout();
}

void PlaylistModel::stopItem()
{
    _running = false;
    _activeItem.second = Idle;
    updateLayout();
}

void PlaylistModel::setActiveItem(int index)
{
    _activeItem.first = index;
    emit activeItemChanged();
    updateLayout();
}

void PlaylistModel::updateLayout()
{
    emit layoutChanged();
}

bool PlaylistModel::moveUp(const QModelIndex &index)
{
    if(index.row() > 0)
    {
        int tmpSecond = _activeItem.second;
        if(_activeItem.first == index.row()){
            _playlist->move(index.row(),index.row()-1);
            _activeItem.first = index.row();
            _activeItem.second = Idle;
            updateLayout();

            _activeItem.first = index.row()-1;
            _activeItem.second = (PlaybackState)tmpSecond;
        }else{
            if(_activeItem.first == index.row()-1){
                _playlist->move(index.row(),index.row()-1);
                _activeItem.first = index.row()-1;
                _activeItem.second = Idle;
                updateLayout();

                _activeItem.first = index.row();
                _activeItem.second = (PlaybackState)tmpSecond;
            }else{
                _playlist->move(index.row(),index.row()-1);
                updateLayout();
                _activeItem.second = (PlaybackState)tmpSecond;
            }

        }
        return true;
    }
    return false;
}

bool PlaylistModel::moveDown(const QModelIndex &index)
{
    if(index.row() < _playlist->count()-1)
    {
        int tmpSecond = _activeItem.second;
        if(_activeItem.first == index.row()){
            _playlist->move(index.row(),index.row()+1);
            _activeItem.first = index.row();
            _activeItem.second = Idle;
            updateLayout();

            _activeItem.first = index.row()+1;
            _activeItem.second = (PlaybackState)tmpSecond;
        }else{
            if(_activeItem.first == index.row()+1){
                _playlist->move(index.row(),index.row()+1);
                _activeItem.first = index.row()+1;
                _activeItem.second = Idle;
                updateLayout();

                _activeItem.first = index.row();
                _activeItem.second = (PlaybackState)tmpSecond;
            }else{
                _playlist->move(index.row(),index.row()+1);
                updateLayout();
                _activeItem.second = (PlaybackState)tmpSecond;
            }

        }
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

void PlaylistModel::repareDelay(QList<Schedule *> scheduleList, uint duration)
{
    foreach(Schedule *schedule1, scheduleList)
    {
        foreach(Schedule *schedule2, scheduleList)
        {
            if(schedule1 != schedule2)
            {
                if(schedule1->finishAt() > schedule2->launchAt() && schedule1->launchAt() < schedule2->launchAt())
                {
                    schedule2->delay(duration);
                    repareDelay(scheduleList, duration);
                }
            }
        }
    }
}
