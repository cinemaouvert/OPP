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

#ifndef UTILS_H
#define UTILS_H

#include <QTime>

/**
 * @brief Convert milliseconds to QTime
 * @param msecs Milliseconds to convert
 * @return QTime corresponding to the milliseconds
 *
 * @author Florian Mhun <florian.mhun@gmail.com>
 */
QTime msecToQTime(uint msecs);

/**
 * @brief Convert C to milliseconds
 * @param time V to convert
 * @return Number of milliseconds corresponding to the value of the QTime
 *
 * @author Cyril Naud <futuramath@gmail.com>
 */
int qTimeToMsec(QTime time);

/**
 * @brief wait t ms between start movie and taking snapshot to have thumbnail
 *
 * @author Thomas Berthomé <thoberthome@laposte.net>
 */
void waitSnap(int t);

/**
 * @brief Return the human readable size
 *
 * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
 */
QString humanSize(int size);

#endif // UTILS_H
