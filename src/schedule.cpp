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

#include "schedule.h"
#include <iostream>
#include <cstdlib>
#include "playlist.h"

Schedule::Schedule(Playlist *playlist, const QDateTime &launchAt, QObject *parent) :
    QObject(parent),
    _launchAt(launchAt),
    _playlist(playlist),
    _canceled(false)
{
    _timer = new QTimer();
}

Schedule::~Schedule()
{
    delete _timer;
}

QDateTime Schedule::finishAt() const
{
    return _launchAt.addMSecs(_playlist->totalDuration());
}

void Schedule::start()
{
    if (isExpired() || isActive())
        return;

    _timer->connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _timer->start(QDateTime::currentDateTime().msecsTo(_launchAt));
}

void Schedule::stop()
{
    _timer->stop();
}

void Schedule::cancel()
{
    _canceled = true;
}

bool Schedule::isExpired() const
{
    return _launchAt < QDateTime::currentDateTime();
}

bool Schedule::isActive() const
{
    return _timer->isActive();
}

bool Schedule::canceled() const
{
    return _canceled;
}

void Schedule::delay(int ms)
{
    bool wasActived = isActive();
    stop();

    _launchAt = _launchAt.addMSecs(ms);

    if (wasActived)
        start();
}

void Schedule::timeout()
{
    stop();
    _canceled = true;

    emit triggered(_playlist);
}
