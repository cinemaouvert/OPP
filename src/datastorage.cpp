/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
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
}

void DataStorage::addPlaylistModel(PlaylistModel* model)
{
    _playlistModelList.append(model);
}
void DataStorage::setScheduleListModel(ScheduleListModel *model)
{
    _scheduleListModel = model;
}
void DataStorage::setProjectTitle(const QString &title)
{
    _projectTitle = title;
}
void DataStorage::setProjectNotes(const QString &notes)
{
    _projectNotes = notes;
}

void DataStorage::save(QFile &file)
{
    QDomDocument doc;
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());
    QDomElement opp = doc.createElement("opp");
    opp.setAttribute("title", _projectTitle);
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
    foreach(Media* mediaElement, mediaList)
    {
        media= doc.createElement("media");
        media.setAttribute("id",mediaElement->id());
        media.setAttribute("location",mediaElement->location());
        medias.appendChild(media);
    }

    /*List of playlists*/
    QDomElement playlist;
    int playlistId=0;
    foreach(PlaylistModel* playlistElement, _playlistModelList)
    {
        qDebug() << "save playlist " << playlistElement->playlist()->title();
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
            playback.setAttribute("media-id", playbackElement->media()->id());
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
            playback.setAttribute("audioTrack", playbackElement->mediaSettings()->audioTrack());
            playback.setAttribute("videoTrack", playbackElement->mediaSettings()->videoTrack());
            playback.setAttribute("subtitlesTrack", playbackElement->mediaSettings()->subtitlesTrack());
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
        schedule.setAttribute("canceled", scheduleElement->canceled());
        schedules.appendChild(schedule);
    }

    /*Save*/
    QTextStream ts(&file);
    int indent = 2;
    doc.save(ts, indent);

    _playlistModelList.clear();
    qDebug()<<"End save";
}

void DataStorage::load(QFile &file)
{
    // FIX : ref 0000001
    int oldPlaylistCount = _win->playlistTabWidget()->count();

    // Remove all data from models
    clear();

    // open xml document
    QDomDocument doc;

    if (!doc.setContent(&file)) {
        qDebug()<< "load error";
        return;
    }

    QDomElement root = doc.documentElement();

    setProjectTitle(root.attribute("title"));
    setProjectNotes(root.attribute("notes"));

    QDomNodeList mediaNodeList = root.elementsByTagName("media");
    QDomNodeList playlistNodeList = root.elementsByTagName("playlist");
    QDomNodeList scheduleNodeList = root.elementsByTagName("schedule");

    // load media
    for (uint i = 0; i < mediaNodeList.length(); i++) {
        QDomNamedNodeMap mediaAttributes = mediaNodeList.at(i).attributes();

        Media *media = new Media(mediaAttributes.namedItem("location").nodeValue(), _app->vlcInstance());
        media->setId(mediaAttributes.namedItem("id").nodeValue().toInt());

        if (media->exists())
            _mediaListModel->addMedia(media);
        else
            delete media;
    }

    // load playlist
    for (uint i = 0; i < playlistNodeList.length(); i++) {
        QDomNode playlistNode = playlistNodeList.at(i);
        QDomNamedNodeMap playlistAttributes = playlistNode.attributes();
        QDomNodeList playbackNodeList = playlistNode.childNodes();

        Playlist *playlist = new Playlist(playlistAttributes.namedItem("title").nodeValue());
        playlist->setId(playlistAttributes.namedItem("id").nodeValue().toInt());

        PlaylistModel *model = new PlaylistModel(playlist, _mediaListModel, _scheduleListModel);

        // FIX : ref 0000001
        _win->restorePlaylistTab(model);

        // load playbacks
        for (int j = 0; j < playbackNodeList.count(); j++) {
            QDomNode playbackNode = playbackNodeList.at(j);
            QDomNamedNodeMap playbackAttributes = playbackNode.attributes();

            const int mediaId = playbackAttributes.namedItem("media-id").nodeValue().toInt();

            Media *media = findMediaById(mediaId);
            if (!media)
                continue;

            Playback *playback = new Playback(media);
            MediaSettings *settings = playback->mediaSettings();

            settings->setRatio( (Ratio) playbackAttributes.namedItem("ratio").nodeValue().toInt() );
            settings->setScale( (Scale) playbackAttributes.namedItem("scale").nodeValue().toInt() );
            settings->setDeinterlacing( (Deinterlacing) playbackAttributes.namedItem("deinterlacing").nodeValue().toInt() );
            settings->setSubtitlesSync( playbackAttributes.namedItem("subtitlesSync").nodeValue().toDouble() );
            settings->setGamma( playbackAttributes.namedItem("gamma").nodeValue().toFloat() );
            settings->setContrast( playbackAttributes.namedItem("contrast").nodeValue().toFloat() );
            settings->setBrightness( playbackAttributes.namedItem("brightness").nodeValue().toFloat() );
            settings->setSaturation( playbackAttributes.namedItem("saturation").nodeValue().toFloat() );
            settings->setHue( playbackAttributes.namedItem("hue").nodeValue().toInt() );
            settings->setAudioSync( playbackAttributes.namedItem("audioSync").nodeValue().toDouble() );
            settings->setAudioTrack( playbackAttributes.namedItem("audioTrack").nodeValue().toInt() );
            settings->setVideoTrack( playbackAttributes.namedItem("videoTrack").nodeValue().toInt() );
            settings->setSubtitlesTrack( playbackAttributes.namedItem("subtitlesTrack").nodeValue().toInt() );
            settings->setTestPattern( playbackAttributes.namedItem("testPattern").nodeValue().toInt() );
            settings->setInMark( playbackAttributes.namedItem("outMark").nodeValue().toInt() );
            settings->setOutMark( playbackAttributes.namedItem("inMark").nodeValue().toInt() );
            settings->setGain( playbackAttributes.namedItem("gain").nodeValue().toFloat() );

            model->addPlayback(playback);
            this->_playlistModelList.push_back(model);
        }

    }

    // FIX : ref 0000001
    // remove old tabs
    while(oldPlaylistCount > 0) {
        _win->playlistTabWidget()->removeTab(0);
        oldPlaylistCount--;
    }

    // load schedule
    for (uint i = 0; i < scheduleNodeList.length(); i++) {
        QDomNode scheduleNode = scheduleNodeList.at(i);
        QDomNamedNodeMap scheduleAttributes = scheduleNode.attributes();

        Playlist *playlist = findPlaylistById(scheduleAttributes.namedItem("playlist-id").nodeValue().toInt());

        if (playlist == NULL)
            continue;

        Schedule *schedule = new Schedule(playlist, QDateTime::fromString(scheduleAttributes.namedItem("launchAt").nodeValue(), "dd/MM/yyyy hh:mm:ss"));

        if (scheduleAttributes.namedItem("canceled").nodeValue().toInt())
            schedule->cancel();

        _scheduleListModel->addSchedule(schedule);
    }

    _playlistModelList.clear();


}

void DataStorage::clear()
{
    _scheduleListModel->removeAll();

    foreach(PlaylistModel *model, _playlistModelList)
        model->removeAll();

    _mediaListModel->removeAll();
    _playlistModelList.clear();

    _projectTitle.clear();
    _projectNotes.clear();
}

Playlist* DataStorage::findPlaylistById(int id) const
{
    foreach(PlaylistModel *m, _playlistModelList) {
        if (m->playlist()->id() == id)
            return m->playlist();
    }
    return 0;
}

Media* DataStorage::findMediaById(int id) const
{
    foreach(Media *m, _mediaListModel->mediaList()) {
        if (m->id() == id)
            return m;
    }
    return 0;
}

