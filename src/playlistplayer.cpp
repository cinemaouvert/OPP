/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#include "playlistplayer.h"

#include <QTimer>

#include <vlc/vlc.h>

#include "playlist.h"
#include "media.h"
#include "mediaplayer.h"
#include "mainwindow.h"


PlaylistPlayer::PlaylistPlayer(libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _currentIndex(-1)
{
    _mediaPlayer = new MediaPlayer(vlcInstance, this);

    connect(_mediaPlayer, SIGNAL(end()), this, SLOT(handlePlayerEnd()));
    connect(this, SIGNAL(end()), this, SLOT(stop()));
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

bool PlaylistPlayer::isPlaying() const
{
    return _mediaPlayer->isPlaying();
}

void PlaylistPlayer::playItemAt(const int &index)
{
    if (index >= _playlist->count())
        return;

    _currentIndex = index;

    ((MainWindow *)this->parent())->setSelectedMediaTimeByIndex(index);

    _mediaPlayer->open(_playlist->at(index));
    _mediaPlayer->play();

    emit itemChanged(_currentIndex);
}

void PlaylistPlayer::next()
{
    if (_currentIndex >= _playlist->count() - 1) {
        playItemAt(0);
    } else {
        playItemAt(++_currentIndex);
    }
}

void PlaylistPlayer::previous()
{
    if (_currentIndex == 0) {
        playItemAt(_playlist->count() - 1);
    } else {
        playItemAt(--_currentIndex);
    }
}

void PlaylistPlayer::stop()
{
    _currentIndex = 0;
    _mediaPlayer->stop();
    _mediaPlayer->close(_playlist->at(_currentIndex));
}

void PlaylistPlayer::playPlaylist(Playlist *playlist)
{
    setPlaylist(playlist);
    playItemAt(0);
}

void PlaylistPlayer::handlePlayerEnd()
{
    if (_currentIndex == _playlist->count() - 1) {
        emit end();
    } else {
        next(); // auto next
    }
}

int  PlaylistPlayer::getCurrentIndex(){
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
