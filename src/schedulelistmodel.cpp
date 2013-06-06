#include "schedulelistmodel.h"

#include "schedule.h"

ScheduleListModel::ScheduleListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ScheduleListModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int ScheduleListModel::rowCount(const QModelIndex &parent) const
{
    return _scheduleList.size();
}

Qt::ItemFlags ScheduleListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ScheduleListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case LaunchAt:
            return trUtf8("Launch at");
            break;
        case FinishAt:
            return trUtf8("Finish at");
            break;
        case PlaylistId:
            return trUtf8("Playlist");
            break;
        case Running:
            return trUtf8("Running");
            break;
        }
    }

    return QVariant();
}

QVariant ScheduleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _scheduleList.count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (index.column()) {
        case LaunchAt:
            return _scheduleList[index.row()].launchAt();
            break;
        case FinishAt:
            return _scheduleList[index.row()].finishAt();
            break;
        case PlaylistId:
            return _scheduleList[index.row()].playlist()->totalDuration();
            break;
        case Running:
            break;
        }
        break;
    }
    return QVariant();
}

bool ScheduleListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        _scheduleList.removeAt(position);
    }

    endRemoveRows();
    return true;
}
