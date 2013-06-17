/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef PLAYLISTLISTMANAGER_H
#define PLAYLISTLISTMANAGER_H

#include <QObject>

// TODO : not yet implemented
/**
 * @class PlaylistListManager
 * @brief Manage playlist list and display into a QTabWidget
 */
class PlaylistListManager : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistListManager(QObject *parent = 0);
    
//    void addItem(PlaylistModel *model);
//    void removeItem(int index);
//    inline const QList<PlaylistModel*> &items();
//    void setCurrentIndex(int index);
//    void clear();
private:

//    QTabWidget *viewList;
    
};

#endif // PLAYLISTLISTMANAGER_H
