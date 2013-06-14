#include "datastorage.h"

#include <QDebug>

DataStorage::DataStorage(QObject *parent) :
    QObject(parent),
    _mediaListModel(0),
    _scheduleListModel(0)
{
}


void DataStorage::setMediaListModel(MediaListModel* model)
{
    _mediaListModel = model;
    qDebug() << "add media list model";
}

void DataStorage::addPlaylistModel(PlaylistModel* model)
{
    _playlistModelList.append(model);
    qDebug() << "add playlist model";
}
void DataStorage::setScheduleListModel(ScheduleListModel *model)
{
    _scheduleListModel = model;
    qDebug() << "add schedule list model";
}
void DataStorage::setProjectTitle(const QString &title)
{
    _projectTitle = title;
    qDebug() << "set title";
}
void DataStorage::setProjectNotes(const QString &notes)
{
    qDebug() << "set notes";

    _projectNotes = notes;
}

void DataStorage::save(const QFile &file)
{

}

void DataStorage::load(const QFile &file)
{

}
