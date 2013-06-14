#include "schedulelistmodel.h"

#include <QIcon>

#include "schedule.h"

ScheduleListModel::ScheduleListModel(QObject *parent) :
    QAbstractTableModel(parent),
    _automationEnabled(false)
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
        case State:
            return trUtf8("State");
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
    case Qt::DecorationRole:
        if (index.column() == State) {
            if (_scheduleList[index.row()]->isExpired()) {
                if (_scheduleList[index.row()]->wasTriggered())
                    return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_206_ok_2.png"));
                else
                    return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_207_remove_2.png"));
            } else if (_automationEnabled) {
                return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_187_more.png"));
            }
            else
                return QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_230_moon.png"));
        }
        break;
    case Qt::DisplayRole:
        switch (index.column()) {
        case LaunchAt:
            return _scheduleList[index.row()]->launchAt().toString("dd/MM/yy hh:mm:ss");
            break;
        case FinishAt:
            return _scheduleList[index.row()]->finishAt().toString("dd/MM/yy hh:mm:ss");
            break;
        case PlaylistId:
            return _scheduleList[index.row()]->playlist()->title();
            break;
        }
        break;
    case Qt::ToolTipRole:
        switch (index.column()) {
        case LaunchAt:
            return _scheduleList[index.row()]->launchAt().toString();
            break;
        case FinishAt:
            return _scheduleList[index.row()]->finishAt().toString();
            break;
        case PlaylistId:
            return _scheduleList[index.row()]->playlist()->title();
            break;
        case State:
            if (index.column() == State) {
                if (_scheduleList[index.row()]->isExpired()) {
                    if (_scheduleList[index.row()]->wasTriggered())
                        return tr("Played");
                    else
                        return tr("Ignored");
                } else if (_automationEnabled){
                    return tr("Pending...");
                }
                else
                    return tr("Sleeping...");
            }
            break;
        }
        break;
    }
    return QVariant();
}

void ScheduleListModel::removeScheduleWithDeps(Playlist *playlist)
{
    for (int i = _scheduleList.count() - 1; i >= 0; --i) {
        if (_scheduleList[i]->playlist() == playlist) {
            removeSchedule(i);
        }
    }
}

void ScheduleListModel::removeSchedule(int index)
{
    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), index, index);
    delete _scheduleList[index];
    _scheduleList.removeAt(index);
    endRemoveRows();
}

void ScheduleListModel::addSchedule(Schedule *schedule)
{
    const int count = _scheduleList.count();

    beginInsertRows(QModelIndex(), count, count);
    _scheduleList.append(schedule);
    endInsertRows();

    connect(schedule, SIGNAL(triggered(Playlist*)), this, SIGNAL(layoutChanged()));

    if (_automationEnabled)
        schedule->start();
}

bool ScheduleListModel::isSchedulable(Schedule *schedule) const
{
    foreach (Schedule *other, _scheduleList) {
        if (schedule->launchAt() >= other->launchAt() && schedule->launchAt() <= other->finishAt())
            return false;
    }
    return true;
}

bool ScheduleListModel::isScheduled(Playlist *playlist) const {
    foreach(Schedule *schedule, _scheduleList) {
        if (schedule->playlist() == playlist)
            return true;
    }
    return false;
}

void ScheduleListModel::delayAll(int ms)
{
    if (ms == 0) return;

    foreach(Schedule *schedule, _scheduleList) {
        if(!schedule->isExpired())
            schedule->delay(ms);
    }

    emit layoutChanged();
}

void ScheduleListModel::toggleAutomation(bool checked) {
    checked ? startAutomation() : stopAutomation();
    updateLayout();
}

void ScheduleListModel::startAutomation() {
    foreach(Schedule *schedule, _scheduleList) {
        schedule->start();
    }

    _automationEnabled = true;
}

void ScheduleListModel::stopAutomation() {
    foreach(Schedule *schedule, _scheduleList) {
        schedule->stop();
    }

    _automationEnabled = false;
}

void ScheduleListModel::updateLayout()
{
    emit layoutChanged();
}

void ScheduleListModel::removeAll()
{
    for (int i = _scheduleList.count()-1; i >= 0; i--) {
        removeSchedule(i);
    }
}

