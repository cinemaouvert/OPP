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
    /**
     * @enum Columns
     * @brief The Columns enum
     */
    enum Columns { LaunchAt = 0, FinishAt = 1, PlaylistId = 2, State = 3 };

    explicit ScheduleListModel(QObject *parent = 0);

    /**
     * @brief Indicates if the automation is enabled
     * @param
     * @returnTrue if automation is enabled, false otherwise
     */
    inline bool isAutomationEnabled() const { return _automationEnabled; }

    /**
     * @brief Returns the schedule list
     * @param
     * @return The schedule list
     */
    inline const QList<Schedule*>& scheduleList() { return _scheduleList; }

    /**
     * @brief Returns the number of columns
     * @param
     * @return The number of columns
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief Returns the number of rows
     * @param
     * @return The number of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the flags applied to the model
     * @param index
     * @return The flags applied to the model
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * @brief Returns the headers data
     * @param section
     * @param orientation
     * @param role
     * @return The headers data
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief Fill the schedule list table
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief Remove schedule matching with a playlist
     * @param playlist
     * @return
     */
    void removeScheduleWithDeps(Playlist *playlist);

    /**
     * @brief Remove a schedule
     * @param index The index of the schedule to remove
     * @return
     */
    void removeSchedule(int index);

    /**
     * @brief Add schedule to the schedule list
     * @param schedule The schedule to add
     * @return
     */
    void addSchedule(Schedule *schedule);

    /**
     * @brief Indicates if a schedule is schedulable
     * @param schedule The schedule
     * @return True if the schedule is schedulable, false otherwise
     */
    bool isSchedulable(Schedule *schedule) const;

    /**
     * @brief Indicates if a playlist is schedules
     * @param playlist The playlist
     * @returnTrue if the playlist is scheduled, false otherwise
     */
    bool isScheduled(Playlist *playlist) const;

    /**
     * @brief Delay all the schedules
     * @param ms The delay
     * @return
     */
    void delayAll(int ms);

    /**
     * @brief Launch the automation
     * @param
     * @return
     */
    void startAutomation();

    /**
     * @brief Stop the automation
     * @param
     * @return
     */
    void stopAutomation();

    /**
     * @brief Update the layout
     * @param
     * @return
     */
    void updateLayout();

    /**
     * @brief Remove all the schedules
     * @param
     * @return
     */
    void removeAll();

public slots:
    /**
     * @brief Enable or disable automation
     * @param checked True to enable automation, false to disable
     * @return
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
};

#endif // SCHEDULELISTMODEL_H
