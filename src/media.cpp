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

#include "media.h"

#include <string.h>

#include <QStringList>

#include <vlc/vlc.h>

#include "application.h"

int Media::s_instanceCount = 0;

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent , bool isFile) :
    QObject(parent),
    _usageCount(0)
{
    _id = Media::s_instanceCount++;
    initMedia(location);
    if(isFile)
        _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());
    else
        _vlcMedia = libvlc_media_new_location(vlcInstance, location.toLocal8Bit().data());

    parseMediaInfos();
}

Media::Media(const Media &media)
{
    initMedia(media._location);
    _usageCount = media._usageCount;
    _vlcMedia = libvlc_media_duplicate(media._vlcMedia);

    _id = Media::s_instanceCount++;
}

Media::~Media()
{
    libvlc_media_release(_vlcMedia);
}

void Media::parseMediaInfos()
{
    libvlc_media_parse(_vlcMedia);

    libvlc_media_track_info_t* tracks;
    int tracksCount = libvlc_media_get_tracks_info(_vlcMedia, &tracks);

    for (int track = 0; track < tracksCount; track++) {
        switch (tracks[track].i_type)
        {
            case libvlc_track_audio:
                _audioTracks << AudioTrack( &tracks[track] );
                break;
            case libvlc_track_video:
                _videoTracks << VideoTrack( &tracks[track] );
                break;
            case libvlc_track_text:
                _subtitlesTracks << Track( &tracks[track] );
                break;
            default:
                break;
        }
    }
    _originalDuration = libvlc_media_get_duration(_vlcMedia);
    _duration = _originalDuration;
}

void Media::setImageTime(QString time){
    libvlc_media_add_option(_vlcMedia,(QString(":image-duration=") + time).toLocal8Bit().data());
}

void Media::initMedia(const QString &location)
{
    _location = location;
    _fileInfo = QFileInfo(location);
}

Media & Media::operator=(const Media &media)
{
    if (this != &media) {
        _location = media._location;
        _fileInfo = QFileInfo(_location);
        _vlcMedia = libvlc_media_duplicate(media._vlcMedia);
    }
    return *this;
}

bool Media::operator==(const Media &media) const
{
    return _location == media.location();
}

void Media::setId(int id)
{
    _id = id;
}

uint Media::duration() const
{
    return _duration;
}

void Media::setDuration(QString &time){
    _duration = time.toInt();
}

uint Media::getOriginalDuration(){
    return _originalDuration;
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::exists() const
{
    return _fileInfo.exists();
}

void Media::usageCountAdd(int count)
{
    _usageCount += count;
    emit usageCountChanged();
}

bool Media::isImage() const
{
    return _audioTracks.count() == 0 && _videoTracks.count() == 1 && _subtitlesTracks.count() == 0;
}

bool Media::isUsed() const
{
    return _usageCount > 0;
}

QList<AudioTrack> Media::audioTracks() const
{
    return _audioTracks;
}

QList<VideoTrack> Media::videoTracks() const
{
    return _videoTracks;
}

QList<Track> Media::subtitlesTracks() const
{
    return _subtitlesTracks;
}

QStringList Media::audioTracksName() const
{
    QStringList list;

    foreach (const AudioTrack &track, _audioTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::videoTracksName() const
{
    QStringList list;

    foreach (const VideoTrack &track, _videoTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::subtitlesTracksName() const
{
    QStringList list;

    foreach (const Track &track, _subtitlesTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::audioExtensions()
{
    return QString(EXTENSIONS_AUDIO).split(";");
}

QStringList Media::videoExtensions()
{
    return QString(EXTENSIONS_VIDEO).split(";");
}

QStringList Media::imageExtensions()
{
    return QString(EXTENSIONS_IMAGE).split(";");
}

QStringList Media::mediaExtensions()
{
    return Media::audioExtensions() + Media::videoExtensions() + Media::imageExtensions();
}

QString Media::getLocation(){
    return _location;
}
