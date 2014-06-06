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

#ifndef CUSTOMEVENTFILTER_H
#define CUSTOMEVENTFILTER_H

#include "locker.h"

#include <QObject>
#include <QEvent>
#include <QTimer>

class CustomEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CustomEventFilter(Locker* lock, QObject *parent = 0);
    virtual ~CustomEventFilter();

    /**
     * @brief GetFilename
     * @return false
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    bool eventFilter(QObject * o, QEvent * e);

private:
    /**
     * @brief The timer
     */
    QTimer* _timer;
    /**
     * @brief The locker
     */
    Locker* _lock;
    
};

#endif // CUSTOMEVENTFILTER_H
