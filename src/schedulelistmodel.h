#ifndef SCHEDULELISTMODEL_H
#define SCHEDULELISTMODEL_H

#include <QAbstractTableModel>

#include "schedule.h"

class ScheduleListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { LaunchAt = 0, FinishAt = 1, PlaylistId = 2, Running = 3 };

    explicit ScheduleListModel(QObject *parent = 0);
    
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

private:
    QList<Schedule*> _scheduleList;

    bool _automationEnabled;
};

#endif // SCHEDULELISTMODEL_H
