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

#ifndef SCHEDULELISTMODEL_H
#define SCHEDULELISTMODEL_H

#include <QAbstractTableModel>
#include <QMessageBox>
#include <cstdlib>

#include "schedule.h"

class ScheduleListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /**
     * @enum Columns
     * @brief The Columns enum
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    enum Columns { LaunchAt = 0, FinishAt = 1, PlaylistId = 2, State = 3 };

    explicit ScheduleListModel(QObject *parent = 0);

    /**
     * @brief Indicates if the automation is enabled
     * @return True if automation is enabled, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline bool isAutomationEnabled() const { return _automationEnabled; }

    /**
     * @brief Returns the schedule list
     * @return The schedule list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QList<Schedule*>& scheduleList() { return _scheduleList; }

    /**
     * @brief Returns the number of columns
     * @return The number of columns
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief Returns the number of rows
     * @return The number of rows
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the flags applied to the model
     * @return The flags applied to the model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * @brief Returns the headers data
     * @param section
     * @param orientation
     * @param role
     * @return The headers data
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief Fill the schedule list table
     * @param index
     * @param role
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief Remove schedule matching with a playlist
     * @param playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeScheduleWithDeps(Playlist *playlist);

    /**
     * @brief Remove a schedule
     * @param index The index of the schedule to remove
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeSchedule(int index);

    /**
     * @brief Add schedule to the schedule list
     * @param schedule The schedule to add
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void addSchedule(Schedule *schedule);

    /**
     * @brief Indicates if a schedule is schedulable
     * @param schedule The schedule
     * @return True if the schedule is schedulable, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isSchedulable(Schedule *schedule) const;

    /**
     * @brief Indicates if a playlist is schedules
     * @param playlist The playlist
     * @return True if the playlist is scheduled, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isScheduled(Playlist *playlist) const;

    /**
     * @brief Delay all the schedules
     * @param ms The delay
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    int delayAll(int ms);

    /**
     * @brief Launch the automation
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void startAutomation();

    /**
     * @brief Stop the automation
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void stopAutomation();

    /**
     * @brief Update the layout
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateLayout();

    /**
     * @brief Remove all the schedules
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void removeAll();

    /**
     * @brief Returns the total duration of the schedules
     * @return The total duration of the schedules
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    QTime totalDuration();

public slots:
    /**
     * @brief Enable or disable automation
     * @param checked True to enable automation, false to disable
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void toggleAutomation(bool checked);

private:
    /**
     * @brief _scheduleList The list of schedules
     */
    QList<Schedule*> _scheduleList;

    /**
     * @brief _automationEnabled True if the automation is enabled, false otherwise
     */
    bool _automationEnabled;

signals:
    /**
     * @brief signal scheduleListChanged
     */
    void scheduleListChanged();
};



#endif // SCHEDULELISTMODEL_H
