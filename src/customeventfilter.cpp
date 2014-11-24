/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Geoffrey Bergé <geoffrey.berge@live.fr>
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

#include "customeventfilter.h"
#include <QDebug>

CustomEventFilter::CustomEventFilter(MainWindow* win, QObject *parent) :
    QObject(parent)
{
    _lock = win->locker();
    _timer = new QTimer();
    _mw = win;
}

CustomEventFilter::~CustomEventFilter()
{
    delete _timer;
}

bool CustomEventFilter::eventFilter(QObject * o, QEvent * e)
{
    /**
     * Handle the autolock.
     */
    if(e->type() == QEvent::MouseMove && _lock->getAutoLock() && _lock->getTime() != -1){
        _timer->connect(_timer, SIGNAL(timeout()), _lock, SLOT(autoLock()));
        _timer->start(_lock->getTime()*60);
    }

    /**
     * If the user click anywhere left hand of the Playlist tab widget,
     * the tab widget select the active track.
     */
    else if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        QTabWidget* p = _mw->playlistTabWidget();

        int mouseGlobalX = event->globalX() - _mw->x();
        int pGlobalX = _mw->x() + p->x();

        if((mouseGlobalX < pGlobalX
            || mouseGlobalX > pGlobalX + p->width())
          )
        {
            int currentIndex = p->currentIndex();
            PlaylistTableView* view = (PlaylistTableView*) p->widget(currentIndex);
            view->selectPlayingItem();
        }
    }

    QObject::eventFilter(o, e);

    return false;
}

