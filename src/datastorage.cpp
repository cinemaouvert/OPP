#include "datastorage.h"

#include <QDebug>
#include <QtXml>
#include <QMessageBox>
#include "media.h"
#include "medialistmodel.h"
#include "playlistmodel.h"
#include "playback.h"
#include "mediasettings.h"
#include "schedulelistmodel.h"

#include "medialistmodel.h"
#include "playlistmodel.h"
#include "schedulelistmodel.h"
#include "application.h"

#include "mainwindow.h" // FIX : ref 0000001

DataStorage::DataStorage(Application *app, MainWindow *win /*FIX : ref 0000001*/, QObject *parent) :
    QObject(parent),
    _mediaListModel(0),
    _scheduleListModel(0),
    _app(app),
    _win(win) // FIX : ref 0000001
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

void DataStorage::save(QFile &file)
{
    qDebug()<<"Start save";
    QDomDocument doc;
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());
    QDomElement opp = doc.createElement("opp");
    opp.setAttribute("name", _projectTitle);
    opp.setAttribute("notes", _projectNotes);
    doc.appendChild(opp);

    QDomElement medias = doc.createElement("medias");
    QDomElement playlists = doc.createElement("playlists");
    QDomElement schedules = doc.createElement("schedules");
    opp.appendChild(medias);
    opp.appendChild(playlists);
    opp.appendChild(schedules);

    /*List of medias*/
    QDomElement media;
    const QList<Media*>& mediaList = _mediaListModel->mediaList();
    int mediaId=0;
    foreach(Media* mediaElement, mediaList)
    {
        media= doc.createElement("media");
        media.setAttribute("id",mediaId);
        mediaId++;
        media.setAttribute("location",mediaElement->location());
        medias.appendChild(media);
    }

    /*List of playlists*/
    QDomElement playlist;
    int playlistId=0;
    foreach(PlaylistModel* playlistElement, _playlistModelList)
    {
        playlist= doc.createElement("playlist");
        playlist.setAttribute("title", playlistElement->playlist()->title());
        playlist.setAttribute("id", playlistId);
        playlistId++;
        playlists.appendChild(playlist);
        const QList<Playback*>& playbackList = playlistElement->playlist()->playbackList();
        QDomElement playback;
        int playbackId=0;
        foreach(Playback* playbackElement, playbackList)
        {
            playback = doc.createElement("playback");
            playback.setAttribute("id", playbackId);
            playbackId++;
            playback.setAttribute("media-id", mediaList.indexOf(playbackElement->media()));
            playback.setAttribute("ratio", playbackElement->mediaSettings()->ratio());
            playback.setAttribute("scale", playbackElement->mediaSettings()->scale());
            playback.setAttribute("deinterlacing", playbackElement->mediaSettings()->deinterlacing());
            playback.setAttribute("subtitlesSync", playbackElement->mediaSettings()->subtitlesSync());
            playback.setAttribute("gamma", playbackElement->mediaSettings()->gamma());
            playback.setAttribute("contrast", playbackElement->mediaSettings()->contrast());
            playback.setAttribute("brightness", playbackElement->mediaSettings()->brightness());
            playback.setAttribute("saturation", playbackElement->mediaSettings()->saturation());
            playback.setAttribute("hue", playbackElement->mediaSettings()->hue());
            playback.setAttribute("audioSync", playbackElement->mediaSettings()->audioSync());
            playback.setAttribute("audioTrack", playbackElement->mediaSettings()->audioTrack().trackId());
            playback.setAttribute("videoTrack", playbackElement->mediaSettings()->videoTrack().trackId());
            playback.setAttribute("subtitlesTrack", playbackElement->mediaSettings()->subtitlesTrack().trackId());
            playback.setAttribute("testPattern", playbackElement->mediaSettings()->testPattern());
            playback.setAttribute("inMark", playbackElement->mediaSettings()->inMark());
            playback.setAttribute("outMark", playbackElement->mediaSettings()->outMark());
            playback.setAttribute("gain", playbackElement->mediaSettings()->gain());
            playlist.appendChild(playback);
        }
    }


    /*List of schedules*/
    QDomElement schedule;
    const QList<Schedule*>& scheduleList = _scheduleListModel->scheduleList();
    foreach(Schedule* scheduleElement, scheduleList)
    {
        schedule= doc.createElement("schedule");
        for(int i=0;i<_playlistModelList.size();i++)
        {
            if(_playlistModelList.at(i)->playlist()==scheduleElement->playlist())
                schedule.setAttribute("playlist-id", i);
        }
        schedule.setAttribute("launchAt", scheduleElement->launchAt().toString("dd/MM/yyyy hh:mm:ss"));
        if(scheduleElement->wasTriggered())
            schedule.setAttribute("wasTriggered", 1);
        else
            schedule.setAttribute("wasTriggered", 0);
        schedules.appendChild(schedule);
    }

    /*Save*/
    file.open(QIODevice::WriteOnly);
    QTextStream ts(&file);
    int indent = 2;
    doc.save(ts, indent);

    qDebug()<<"End save";
}

void DataStorage::load(const QFile &file)
{
    // Remove all data from models
    _scheduleListModel->removeAll();
    foreach(PlaylistModel *model, _playlistModelList)
        model->removeAll();
    _mediaListModel->removeAll();

    _playlistModelList.clear();

    // open xml document
    QFile myFile("/Users/floomoon/test.opp");
    myFile.open(QIODevice::ReadWrite);

    QDomDocument doc;

    if (!doc.setContent(&myFile)) {
        // error
        return;
    }

    QDomElement root = doc.documentElement();

    QDomNodeList mediaNodeList = root.elementsByTagName("media");
    QDomNodeList playlistNodeList = root.elementsByTagName("playlist");
    QDomNodeList scheduleNodeList = root.elementsByTagName("schedule");

    // load media
    for (int i = 0; i < mediaNodeList.length(); i++) {
        QDomNamedNodeMap mediaAttributes = mediaNodeList.at(i).attributes();

        for (int k = 0; k < mediaAttributes.length(); k++) {
            Media *media = new Media(mediaAttributes.item(i).nodeValue(), _app->vlcInstance());
            if (media->exists())
                _mediaListModel->addMedia(media);
            else
                delete media;
        }
    }

    // load playlist
    for (int i = 0; i < playlistNodeList.length(); i++) {
        QDomNode playlistNode = playlistNodeList.at(i);
        QDomNamedNodeMap playlistAttributes = playlistNode.attributes();
        QDomNodeList playbackNodeList = playlistNode.childNodes();

        Playlist *playlist = new Playlist(_app->vlcInstance(), playlistAttributes.namedItem("title").nodeValue());
        PlaylistModel *model = new PlaylistModel(playlist, _mediaListModel, _scheduleListModel);

        // FIX : ref 0000001
        _win->restorePlaylistTab(model);

        // load playbacks
        for (int j = 0; j < playbackNodeList.count(); j++) {
            QDomNode playbackNode = playbackNodeList.at(j);
            QDomNamedNodeMap playbackAttributes = playbackNode.attributes();

            const int mediaId = playbackAttributes.namedItem("media-id").nodeValue().toInt();

            Media *media = _mediaListModel->findById(mediaId);
            if (!media)
                continue;

            Playback *playback = new Playback(media);
            model->addPlayback(playback);
        }
    }

    // load schedule
    for (int i = 0; i < scheduleNodeList.length(); i++) {

    }

    myFile.close();
}
