#include "datastorage.h"

#include <QDebug>
#include <QtXml>
#include <QMessageBox>

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
    qDebug()<<"Start save";
    QDomDocument doc;
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());
    QDomElement opp = doc.createElement("opp");
    opp.setAttribute("name", "name");
    opp.setAttribute("notes", "notes");
    doc.appendChild(opp);

    QDomElement medias = doc.createElement("medias");
    QDomElement playlists = doc.createElement("playlists");
    QDomElement schedules = doc.createElement("schedules");
    opp.appendChild(medias);
    opp.appendChild(playlists);
    opp.appendChild(schedules);

    QDomElement media1 = doc.createElement("media");
    media1.setAttribute("id",1);
    media1.setAttribute("location","location");
    medias.appendChild(media1);

    QDomElement playlist1 = doc.createElement("playlist");
    playlist1.setAttribute("title", "title");
    playlists.appendChild(playlist1);

    QDomElement playback1 = doc.createElement("playback");
    playback1.setAttribute("id", 1);
    playback1.setAttribute("media-id", 1);
    playback1.setAttribute("ratio", 1);
    playlist1.appendChild(playback1);

    QDomElement schedule1 = doc.createElement("schedule");
    schedule1.setAttribute("playlist-id", 1);
    schedule1.setAttribute("launchAt", 123456);
    schedule1.setAttribute("wasTriggered", 0);
    schedules.appendChild(schedule1);

    QFile fichier("test.opp");
    fichier.open(QIODevice::WriteOnly);
    QTextStream ts(&fichier);
    int indent = 2;
    doc.save(ts, indent);

    qDebug()<<"End save";
}

void DataStorage::load(const QFile &file)
{

}
