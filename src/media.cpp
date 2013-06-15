#include "media.h"

#include <unistd.h>
#include <string.h>

#include <QStringList>

#include "global.h"
#include "application.h"

int Media::s_instanceCount = 0;

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _usageCount(0)
{
    _id = Media::s_instanceCount++;
    initMedia(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());
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
        }
    }
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
    return libvlc_media_get_duration(_vlcMedia);
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
    return _audioTracks.count() == 1 && _videoTracks.count() == 2 && _subtitlesTracks.count() == 1;
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
