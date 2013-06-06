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
    
    inline QList<Schedule>& scheduleList() { return _scheduleList; }

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

private:
    QList<Schedule> _scheduleList;
};

#endif // SCHEDULELISTMODEL_H
