/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#include <QTimer>
#include <vlc/vlc.h>

#include "mainwindow.h"
#include "playlisttableview.h"
#include "Playlist.h"
#include "PlaylistPlayer.h"
#include "media.h"
#include "MediaPlayer.h"


PlaylistPlayer::PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _currentIndex(-1)
{
    _mediaPlayer = new MediaPlayer(vlcInstance, this);
    _loop = NOLOOP;

    connect(_mediaPlayer, SIGNAL(end()), this, SLOT(handlePlayerEnd()));
}

PlaylistPlayer::~PlaylistPlayer()
{
    delete _mediaPlayer;
}

void PlaylistPlayer::setPlaylist(Playlist *playlist)
{
    _playlist = playlist;
    _currentIndex = 0;
}

void PlaylistPlayer::playItemAt(const int &index)
{
    if (_mediaPlayer->isPaused()) {
        _mediaPlayer->resume();
    } else {
        initItemAt(index);

        _mediaPlayer->play();
    }
}

void PlaylistPlayer::initItemAt(const int &index)
{
    if (index >= _playlist->count())
        return;

    _currentIndex = index;

    ((MainWindow *)this->parent())->setSelectedMediaTimeByIndex(index);

    _mediaPlayer->open(_playlist->at(index));

    emit itemChanged(_currentIndex);
}

void PlaylistPlayer::next()
{
    if(_mediaPlayer->isStopped() || _mediaPlayer->isPaused()){
        if (_currentIndex >= _playlist->count() - 1) {
            initItemAt(0);
        } else {
            initItemAt(++_currentIndex);
        }
    }
    else
    {
        if (_currentIndex >= _playlist->count() - 1) {
            playItemAt(0);
        } else {
            playItemAt(++_currentIndex);
        }
    }
}

void PlaylistPlayer::previous()
{
    if(_mediaPlayer->isStopped() || _mediaPlayer->isPaused()){
        if (_currentIndex == 0) {
            initItemAt(_playlist->count() - 1);
        } else {
            initItemAt(--_currentIndex);
        }
    }
    else
    {
        if (_currentIndex == 0) {
            playItemAt(_playlist->count() - 1);
        } else {
            playItemAt(--_currentIndex);
        }
    }
}

int PlaylistPlayer::forward()
{
    int newTime = _mediaPlayer->jumpForward();

    if(newTime != -1){
        if(newTime == 0)
            stop();
    }

    return newTime;
}

int PlaylistPlayer::rewind()
{
    int newTime = _mediaPlayer->jumpBack();

    if(newTime != -1){
        if(newTime < 0)
            stop();
    }

    return newTime;
}

void PlaylistPlayer::stop()
{
    _mediaPlayer->stop();
    _mediaPlayer->close(_playlist->at(_currentIndex));
}

void PlaylistPlayer::play()
{
    //Creation de la video window si elle n'existe pas
    ((MainWindow*)this->parent())->needVideoWindow();

    // play or resume playback
    QModelIndexList indexes = ((MainWindow*)this->parent())->currentPlaylistTableView()->selectionModel()->selectedRows();

    // if no selected item play current playlist from first item
    if (indexes.count() == 0) {
        playItemAt(0);

    // play playlist at selected item otherwise
    } else {
        const int index = indexes.first().row();
        playItemAt(index);
    }
}

void PlaylistPlayer::playPlaylist(Playlist *playlist)
{
    setPlaylist(playlist);
    playItemAt(0);
}

void PlaylistPlayer::handlePlayerEnd()
{
    if (_currentIndex == _playlist->count() - 1) {
        switch(_loop){
            case BIGLOOP:
                next(); // auto next
                play();
                break;
            case SINGLELOOP:
                playItemAt(_currentIndex);
                break;
            case NOLOOP:
                emit end();
                break;
            default:
                break;
        }
    } else {
        switch(_loop){
            case BIGLOOP:
                next(); // auto next
                play();
                break;
            case SINGLELOOP:
                playItemAt(_currentIndex);
                break;
            case NOLOOP:
                next(); // auto next
                play();
                break;
            default:
                break;
        }
    }
}

int  PlaylistPlayer::getCurrentIndex()
{
    return _currentIndex;
}

void  PlaylistPlayer::currentIndexUp()
{
    _currentIndex++;
}

void  PlaylistPlayer::currentIndexDown()
{
    _currentIndex--;
}

void PlaylistPlayer::setLoop(Loop newLoopState)
{
    _loop = newLoopState;
}
