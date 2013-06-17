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

#ifndef SCHEDULELISTMODEL_H
#define SCHEDULELISTMODEL_H

#include <QAbstractTableModel>

#include "schedule.h"

class ScheduleListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { LaunchAt = 0, FinishAt = 1, PlaylistId = 2, State = 3 };

    explicit ScheduleListModel(QObject *parent = 0);

    inline bool isAutomationEnabled() const { return _automationEnabled; }
    
    inline const QList<Schedule*>& scheduleList() { return _scheduleList; }

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    void removeScheduleWithDeps(Playlist *playlist);

    void removeSchedule(int index);

    void addSchedule(Schedule *schedule);

    bool isSchedulable(Schedule *schedule) const;

    bool isScheduled(Playlist *playlist) const;

    void delayAll(int ms);

    void startAutomation();

    void stopAutomation();

    void updateLayout();

    void removeAll();

public slots:
    void toggleAutomation(bool checked);

private:
    QList<Schedule*> _scheduleList;

    bool _automationEnabled;
};

#endif // SCHEDULELISTMODEL_H
