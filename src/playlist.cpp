/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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

#include "playlist.h"

#include <QDebug>

#include <vlc/vlc.h>

#include "media.h"

int Playlist::s_instanceCount = 0;

Playlist::Playlist(const QString &title, QObject *parent) :
    QObject(parent),
    _title(title)
{
    _id = Playlist::s_instanceCount++;
}

Playlist::~Playlist()
{ 
    foreach(Playback *playback, _playbackList) {
        playback->media()->usageCountAdd(-1);
        delete playback;
    }
}

void Playlist::setId(int id)
{
    _id = id;
}

void Playlist::setTitle(const QString &title)
{
    _title = title;
    emit titleChanged();
}

Playback* Playlist::at(const int &index) const
{
    return _playbackList.at(index);
}

void Playlist::append(Playback *playback)
{
    playback->media()->usageCountAdd();

    _playbackList.append(playback);
}

void Playlist::removeAt(int index)
{
    _playbackList[index]->media()->usageCountAdd(-1);

    delete _playbackList[index];
    _playbackList.removeAt(index);
}

int Playlist::indexOf(Playback *playback) const
{
    return _playbackList.indexOf(playback);
}

void Playlist::move(int from, int to)
{
    _playbackList.move(from, to);
}


int Playlist::count() const
{
    return _playbackList.count();
}

uint Playlist::totalDuration() const
{
    uint duration = 0;

    foreach(Playback *playback, _playbackList) {
        duration += playback->media()->duration();
    }

    return duration;
}
